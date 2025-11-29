#include "audio.h"
#include "hardware_stm_timer2.h"
#include "hardware_stm_gpio.h" 

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
    // --- PHASE 1: IGNITION (Stretched to ~1.95 seconds) ---
    // Durations multiplied by 3 to extend time without slowing the hum later
    {2000, 5}, {1500, 5}, {1000, 5}, {800, 5}, // High "Snap"
    {500, 10},  {400, 10},  {300, 10},  {200, 25}, // The "Hiss" down
    {150, 50}, {120, 50}, {110, 50}, {100, 50}, // Settling (Longer tail)

    // --- PHASE 2: IDLE HUM (Unchanged - Fast & Throbbing) ---
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {95, 50}, {100, 50}, {105, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},

    // --- PHASE 3: THE SWING (Doppler Effect) ---
    {110, 30}, {120, 30}, {140, 30}, {160, 30}, 
    {180, 40}, {200, 40}, {220, 40},            
    {180, 30}, {160, 30}, {140, 30}, {120, 30}, 
    {100, 100},                                 

    // --- PHASE 4: IDLE HUM (Short) ---
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},

    // --- PHASE 5: THE CLASH (Impact) ---
    {800, 10}, {1500, 10}, {600, 10}, {2000, 10}, 
    {500, 10}, {1200, 10}, {700, 10}, {1800, 10},
    {300, 20}, {150, 50}, 

    // --- PHASE 6: IDLE HUM (Stabilizing) ---
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {95, 50}, {100, 50}, {105, 50}, {100, 50},

    // --- PHASE 7: POWER DOWN (Retraction) ---
    {90, 50}, {80, 50}, {70, 50}, 
    {60, 40}, {50, 40}, {40, 30}, 
    {30, 30}, {20, 20}, {10, 20},
    {0, 500} // Silence
};

static int lightsaber_off_effect[][2] = {

    // --- PHASE 7: POWER DOWN ---
    {90, 50}, {80, 50}, {70, 50}, 
    {60, 40}, {50, 40}, {40, 30}, 
    {30, 30}, {20, 20}, {10, 20},
    {0, 500} 
};

// C. Main Theme
static int main_theme[][2] = {
    {392, 350}, {392, 350}, {392, 350}, 
    {523, 1500}, {784, 1500}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 1500}, {784, 1000}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 1500}, {784, 1000}, 
    {698, 250}, {659, 250}, {698, 250}, {587, 1500}, 
    {0, 250},
    {392, 250}, {392, 250}, {392, 250}, 
    {523, 2500}, {784, 2500}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 2500}, {784, 1250}, 
    {698, 250}, {659, 250}, {587, 250}, {1047, 2500}, {784, 1250}, 
    {698, 250}, {659, 250}, {698, 250}, {587, 2000} 
};

// LED Cycling bip sound
static int LED_scan[][2] = {
    {2000, 50}, // High pitch tone
    {0, 10}     // Tiny silence buffer 
};
// --- 2. Configuration (Counts & Tempos) ---

// Note Counts
#define COUNT_MAIN      (sizeof(main_theme) / sizeof(main_theme[0]))
#define COUNT_IMPERIAL  (sizeof(imperial_march_analog) / sizeof(imperial_march_analog[0]))
#define COUNT_SABER     (sizeof(lightsaber_effect) / sizeof(lightsaber_effect[0]))
#define COUNT_SCAN      (sizeof(LED_scan) / sizeof(LED_scan[0]))
#define COUNT_SABER_OFF (sizeof(lightsaber_off_effect) / sizeof(lightsaber_off_effect[0]))

// Tempo Dividers (Lower = Slower, Higher = Faster)
#define TEMPO_MAIN      1  // Slow, cinematic
#define TEMPO_IMPERIAL  1.0  // Standard march speed
#define TEMPO_SABER     1  // Fast enough to make "hum" sound continuous
#define TEMPO_SCAN      1
#define TEMPO_SABER_OFF 1

// State Counters (Separate for each song)
static int count_main = 0;
static int count_imperial = 0;
static int count_saber = 0;
static int count_scan = 0;
static int count_saber_off = 0;


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
        *ccr_reg = new_arr / 8; // 4 for high volume, 8 for lower volume (hopefully)
    }
}

// --- 4. Public Functions ---

// Call this when entering a new State to ensure songs start at the beginning
void resetMusicCounter(void) {
    count_main = 0;
    count_imperial = 0;// count for imperial
    count_saber = 0;
    count_scan = 0;
    count_saber_off = 0;
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

uint16_t playLED_Scan(void){
    if (count_scan >= COUNT_SCAN) {
        count_scan = 0; 
    }

    int freq = LED_scan[count_scan][0];
    int duration = LED_scan[count_scan][1];

    setHardwareTone(freq);
    count_saber++;

    return (uint16_t)(duration / TEMPO_SCAN);
}

uint16_t playSABER_off(void){
    if (count_saber_off >= COUNT_SABER_OFF) {
        count_saber_off = 0; 
    }

    int freq = lightsaber_off_effect[count_saber_off][0];
    int duration = lightsaber_off_effect[count_saber_off][1];

    setHardwareTone(freq);
    count_saber_off++;

    return (uint16_t)(duration / TEMPO_SCAN);
}

void init_speaker(void){
        initGpioBxAsAF1(3); //PB3 as AF1 connects to CH2
        initTimer2_CH2_PWM(); // timer2 as PWM  
}