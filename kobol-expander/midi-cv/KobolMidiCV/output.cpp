#include "output.h"

// L'API LEDC a changé entre le core ESP32 2.x et 3.x :
//   2.x : ledcSetup(canal, freq, bits) + ledcAttachPin(gpio, canal)
//         puis ledcWrite(canal, duty)
//   3.x : ledcAttach(gpio, freq, bits) -> bool
//         puis ledcWrite(gpio, duty)
// On s'aligne sur la version installée plutôt que d'imposer la nôtre.
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  #define LEDC_API_V3 1
#else
  #define LEDC_API_V3 0
#endif

// L'ESP32-S3 n'a que 8 canaux LEDC (4 timers, pas de mode grande vitesse).
static const uint8_t LEDC_CHANNELS_MAX = 8;

static bool    s_live[PARAM_COUNT];
static bool    s_pitch_live = false;
static uint8_t s_next_channel = 0;

// Réserve un canal pour un GPIO. Renvoie false si le GPIO n'est pas câblé
// ou si les canaux LEDC sont épuisés.
static bool attachPwm(uint8_t gpio) {
  if (gpio == GPIO_UNASSIGNED) return false;
  if (s_next_channel >= LEDC_CHANNELS_MAX) return false;

#if LEDC_API_V3
  if (!ledcAttach(gpio, PWM_FREQ_HZ, PWM_BITS)) return false;
#else
  ledcSetup(s_next_channel, PWM_FREQ_HZ, PWM_BITS);
  ledcAttachPin(gpio, s_next_channel);
#endif
  s_next_channel++;
  return true;
}

static void writePwm(uint8_t gpio, uint8_t channel, uint16_t duty) {
#if LEDC_API_V3
  (void)channel;
  ledcWrite(gpio, duty);
#else
  (void)gpio;
  ledcWrite(channel, duty);
#endif
}

// Canal LEDC attribué à chaque paramètre, dans l'ordre d'initialisation.
static uint8_t s_channel[PARAM_COUNT];
static uint8_t s_pitch_channel = 0;

uint16_t mvToDuty(int32_t mv, int16_t cond_min_mv, int16_t cond_max_mv) {
  if (mv <= cond_min_mv) return 0;
  if (mv >= cond_max_mv) return PWM_MAX;
  const int32_t span = (int32_t)cond_max_mv - (int32_t)cond_min_mv;
  return (uint16_t)(((int64_t)(mv - cond_min_mv) * PWM_MAX) / span);
}

uint8_t outputBegin() {
  uint8_t live_count = 0;

  // Le pitch passe en premier : c'est le seul paramètre sans lequel
  // l'interface n'a aucun intérêt musical.
  s_pitch_channel = s_next_channel;
  s_pitch_live = attachPwm(GPIO_PITCH);
  if (s_pitch_live) {
    live_count++;
    outputWritePitchMv(CAL_NEUTRAL_MV);
  } else {
    Serial.println(F("[SORTIE] PITCH (pin 11) : AUCUN CANAL — interface muette"));
  }

  for (uint8_t i = 0; i < PARAM_COUNT; i++) {
    const KobolParam& p = PARAMS[i];

    if (p.state == PARAM_BLOCKED) {
      s_live[i] = false;
      Serial.printf("[SORTIE] %-14s CC%-3u  bloqué (mesure manquante)\n", p.name, p.cc);
      continue;
    }

    s_channel[i] = s_next_channel;
    s_live[i] = attachPwm(p.gpio);

    if (s_live[i]) {
      live_count++;
      Serial.printf("[SORTIE] %-14s CC%-3u  pin P1 %-2u  GPIO %-2u %s\n",
                    p.name, p.cc, p.p1_pin, p.gpio,
                    p.state == PARAM_CHECK ? " (plage a confirmer)" : "");
    } else if (p.gpio == GPIO_UNASSIGNED) {
      Serial.printf("[SORTIE] %-14s CC%-3u  pin P1 %-2u  pas de GPIO -> DAC externe requis\n",
                    p.name, p.cc, p.p1_pin);
    } else {
      Serial.printf("[SORTIE] %-14s CC%-3u  pin P1 %-2u  CANAUX LEDC EPUISES\n",
                    p.name, p.cc, p.p1_pin);
    }
  }

  pinMode(GPIO_GATE, OUTPUT);
  digitalWrite(GPIO_GATE, LOW);

  return live_count;
}

bool outputIsLive(uint8_t param_index) {
  if (param_index >= PARAM_COUNT) return false;
  return s_live[param_index];
}

void outputWriteMv(uint8_t param_index, int32_t mv) {
  if (param_index >= PARAM_COUNT || !s_live[param_index]) return;
  const KobolParam& p = PARAMS[param_index];
  writePwm(p.gpio, s_channel[param_index],
           mvToDuty(mv, p.cond_min_mv, p.cond_max_mv));
}

void outputWritePitchMv(int32_t mv) {
  if (!s_pitch_live) return;
  // Butées dures : la calibration ne doit jamais pousser hors de la plage
  // relevée au test d'injection.
  if (mv < PITCH_MIN_MV) mv = PITCH_MIN_MV;
  if (mv > PITCH_MAX_MV) mv = PITCH_MAX_MV;
  writePwm(GPIO_PITCH, s_pitch_channel,
           mvToDuty(mv, COND_B_MIN_MV, COND_B_MAX_MV));
}
