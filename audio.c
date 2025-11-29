#include "audio.h"
#include "hardware_stm_timer2.h" 

// --- 1. Sound Data Arrays ---

// A. Imperial March
static int imperial_march_analog[][2] = {
  {440, 500}, {440, 500}, {440, 500}, {349, 350}, {523, 150},
  {440, 500}, {349, 350}, {523, 150}, {440, 650}, {0, 150},
  {659, 500}, {659, 500}, {659, 500}, {698, 350}, {523, 150},
  {415, 500}, {349, 350}, {523, 150}, {440, 650}, {0, 150}
};

// B. Lightsaber Effect
static int lightsaber_effect[][2] = {
    // --- PHASE 1: IGNITION ---
    {2000, 20}, {1500, 20}, {1000, 20}, {800, 20}, 
    {500, 30},  {400, 30},  {300, 30},  {200, 50}, 
    {150, 50},  {120, 50},  {110, 100}, {100, 100}, 

    // --- PHASE 2: IDLE HUM ---
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {95, 50}, {100, 50}, {105, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},

    // --- PHASE 3: THE SWING ---
    {110, 30}, {120, 30}, {140, 30}, {160, 30}, 
    {180, 40}, {200, 40}, {220, 40},            
    {180, 30}, {160, 30}, {140, 30}, {120, 30}, 
    {100, 100},                                 

    // --- PHASE 4: IDLE HUM ---
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},

    // --- PHASE 5: THE CLASH ---
    {800, 10}, {1500, 10}, {600, 10}, {2000, 10}, 
    {500, 10}, {1200, 10}, {700, 10}, {1800, 10},
    {300, 20}, {150, 50}, 

    // --- PHASE 6: IDLE HUM ---
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {95, 50}, {100, 50}, {105, 50}, {100, 50},

    // --- PHASE 7: POWER DOWN ---
    {90, 50}, {80, 50}, {70, 50}, 
    {60, 40}, {50, 40}, {40, 30}, 
    {30, 30}, {20, 20}, {10, 20},
    {0, 500} 
};

// C. Main Theme
static int main_theme[][2] = {
    {392, 350}, {392, 350}, {392, 350}, 
    {523, 2500}, {784, 2500}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 2500}, {784, 1250}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 2500}, {784, 1250}, 
    {698, 250}, {659, 250}, {698, 250}, {587, 2500}, 
    {0, 250},
    {392, 250}, {392, 250}, {392, 250}, 
    {523, 2500}, {784, 2500}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 2500}, {784, 1250}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 2500}, {784, 1250}, 
    {698, 250}, {659, 250}, {698, 250}, {587, 2000} 
};

// --- 2. Configuration (Counts & Tempos) ---

// Note Counts
#define COUNT_MAIN      (sizeof(main_theme) / sizeof(main_theme[0]))
#define COUNT_IMPERIAL  (sizeof(imperial_march_analog) / sizeof(imperial_march_analog[0]))
#define COUNT_SABER     (sizeof(lightsaber_effect) / sizeof(lightsaber_effect[0]))

// Tempo Dividers (Lower = Slower, Higher = Faster)
#define TEMPO_MAIN      0.5  // Slow, cinematic
#define TEMPO_IMPERIAL  1.0  // Standard march speed
#define TEMPO_SABER     0.7  // Fast enough to make "hum" sound continuous

// State Counters (Separate for each song)
static int count_main = 0;
static int count_imperial = 0;
static int count_saber = 0;

// --- 3. Hardware Helper (Private) ---
static void setHardwareTone(uint32_t freq) {
    uint32_t * arr_reg = (uint32_t*)TIM2_AUTORELOAD_REGISTER;
    uint32_t * ccr_reg = (uint32_t*)TIM2_COMPARE_2_REGISTER;
    
    if (freq == 0) {
        *ccr_reg = 0; 
    } else {
        // Assumes 1MHz Timer Clock. Adjust 1000000 if Prescaler changes.
        uint32_t new_arr = (1000000 / freq) - 1; 
        *arr_reg = new_arr;        
        *ccr_reg = new_arr / 4; 
    }
}

// --- 4. Public Functions ---

// Call this when entering a new State to ensure songs start at the beginning
void resetMusicCounter(void) {
    count_main = 0;
    count_imperial = 0;// count for imperial
    count_saber = 0;
}

// Call this to silence the speaker immediately
void stopAudio(void) {
    uint32_t * ccr_reg = (uint32_t*)TIM2_COMPARE_2_REGISTER;
    *ccr_reg = 0; 
}

// --- Playback Functions (Return duration in ms) ---

uint16_t playMainTheme(void) {
    if (count_main >= COUNT_MAIN) {
        count_main = 0; 
    }

    int freq = main_theme[count_main][0];
    int duration = main_theme[count_main][1];

    setHardwareTone(freq);
    count_main++;

    return (uint16_t)(duration / TEMPO_MAIN);
}

uint16_t playImperialMarch(void) {
    if (count_imperial >= COUNT_IMPERIAL) {
        count_imperial = 0; 
    }

    int freq = imperial_march_analog[count_imperial][0];
    int duration = imperial_march_analog[count_imperial][1];

    setHardwareTone(freq);
    count_imperial++;

    return (uint16_t)(duration / TEMPO_IMPERIAL);
}

uint16_t playLightsaberEffect(void) {
    if (count_saber >= COUNT_SABER) {
        count_saber = 0; 
    }

    int freq = lightsaber_effect[count_saber][0];
    int duration = lightsaber_effect[count_saber][1];

    setHardwareTone(freq);
    count_saber++;

    return (uint16_t)(duration / TEMPO_SABER);
}