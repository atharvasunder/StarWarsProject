/**
  ******************************************************************************
  * @file    audio.c 
  * @author  Group 3 (dseong, paussava, vkenkre, asramdas, kadikpet)
  * @version 1.0
  * @date    November-2025
  * @brief   Contains functions for reasoning about state transitions
  ******************************************************************************
*/

#include "audio.h"
#include "hardware_stm_timer2_and_11.h"
#include "hardware_stm_gpio.h" 

/*
 * ======================================================================================
 * AUDIO SYSTEM ARCHITECTURE
 * ======================================================================================
 * * 1. DATA STRUCTURE (The "Sheet Music")
 * -------------------------------------
 * Songs are stored as 2D arrays: static int song_name[][2]
 * - Column 0: Frequency (Hz). 0 Hz represents a rest (silence).
 * - Column 1: Duration (ms). How long the note should play.
 * - Example: {523, 400} plays High C for 400 milliseconds.
 * * 2. PLAYBACK LOGIC (The "Musician")
 * ----------------------------------
 * Each song has a dedicated function (e.g., playMarioGameOver, playRockyTheme).
 * These functions are designed to be Non-Blocking.
 * - Counters: Each song has a static 'count' variable tracking the current note index.
 * - Tempo:    A macro (e.g., TEMPO_GAMEOVER) divides the duration to speed up/slow down playback.
 * - Logic:    When called, the function:
 * 1. Reads the Frequency/Duration at the current 'count'.
 * 2. Calls setHardwareTone() to update the physical Timer registers.
 * 3. Increments 'count' for the next call.
 * 4. Returns the Duration (in ms) to the State Machine.
 * * 3. HARDWARE INTERFACE (The "Instrument")
 * ----------------------------------------
 * - setHardwareTone_TIM11(freq): 
 * Converts a Frequency (Hz) into Timer Auto-Reload (ARR) and Capture Compare (CCR) values.
 * It drives the PWM signal on the speaker pin.
 * If freq == 0, it sets Duty Cycle to 0% (Silence).
 * * 4. ROLE IN STATE MACHINE (The "Conductor")
 * ------------------------------------------
 * The Audio System is event-driven to allow multitasking (LEDs + Audio).
 * * Step A: State Entry
 * - Call resetAudioCounters() to ensure the song starts from the beginning.
 * - Call playSong() once to start the first note.
 * - Use the returned duration to Enqueue a START_TIMEOUT event.
 * * Step B: TIMEOUT Event
 * - When the timeout expires, the State Machine receives a TIMEOUT event (param 2).
 * - It calls playSong() again to get the NEXT note.
 * - It enqueues a new START_TIMEOUT for the new duration.
 * * Step C: Song Completion
 * - If the counter reaches the end of the array, playSong() returns 0 or stops audio.
 * - The State Machine transitions to a new state (e.g., IN_GAME) or stops the loop.
 * * ======================================================================================
 */


// --- 1. Configuration (Counts & Tempos) ---

// Note Counts
#define COUNT_MAIN      (sizeof(main_theme) / sizeof(main_theme[0]))
#define COUNT_IMPERIAL  (sizeof(imperial_march_analog) / sizeof(imperial_march_analog[0]))
#define COUNT_SABER     (sizeof(lightsaber_effect) / sizeof(lightsaber_effect[0]))
#define COUNT_SCAN      (sizeof(LED_scan) / sizeof(LED_scan[0]))
#define COUNT_SABER_OFF (sizeof(lightsaber_off_effect) / sizeof(lightsaber_off_effect[0]))
#define COUNT_ROCKY     (sizeof(victory_song) / sizeof(victory_song[0]))
#define COUNT_GAMEOVER  (sizeof(mario_game_over) / sizeof(mario_game_over[0]))


// Tempo Dividers (Lower = Slower, Higher = Faster)
#define TEMPO_MAIN          1  
#define TEMPO_IMPERIAL      1.0
#define TEMPO_SABER         1  
#define TEMPO_SCAN          1
#define TEMPO_SABER_OFF     1
#define TEMPO_VICTORY       0.25
#define TEMPO_GAME_OVER     0.75

// --- 2. Sound Data Arrays ---

// A. Imperial March
static int imperial_march_analog[][2] = {
  {440, 500}, {440, 500}, {440, 500}, {349, 350}, {523, 150},
  {440, 500}, {349, 350}, {523, 150}, {440, 650}, {0, 150},
  {659, 500}, {659, 500}, {659, 500}, {698, 350}, {523, 150},
  {415, 500}, {349, 350}, {523, 150}, {440, 650}, {0, 150}
};

// B. Lightsaber Effect

static int lightsaber_effect[][2] = {


    {2000, 5}, {1500, 5}, {1000, 5}, {800, 5}, // High "Snap"
    {500, 10},  {400, 10},  {300, 10},  {200, 25}, // The "Hiss" down
    {150, 50}, {120, 50}, {110, 50}, {100, 50}, // Settling (Longer tail)

    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {95, 50}, {100, 50}, {105, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},

    {110, 30}, {120, 30}, {140, 30}, {160, 30}, 
    {180, 40}, {200, 40}, {220, 40},            
    {180, 30}, {160, 30}, {140, 30}, {120, 30}, 
    {100, 100},                                 

    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {98, 50}, {100, 50}, {102, 50}, {100, 50},


    {800, 10}, {1500, 10}, {600, 10}, {2000, 10}, 
    {500, 10}, {1200, 10}, {700, 10}, {1800, 10},
    {300, 20}, {150, 50}, 


    {98, 50}, {100, 50}, {102, 50}, {100, 50},
    {95, 50}, {100, 50}, {105, 50}, {100, 50},

    {90, 50}, {80, 50}, {70, 50}, 
    {60, 40}, {50, 40}, {40, 30}, 
    {30, 30}, {20, 20}, {10, 20},
    {0, 500} 
};

// Light saber turing off
static int lightsaber_off_effect[][2] = {

    // --- PHASE 7: POWER DOWN ---
    {90, 200}, {80, 200}, {70, 200}, 
    {60, 100}, {50, 100}, {40, 60}, 
    {30, 60}, {20, 50}, {10, 50},
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

// Format: {Frequency (Hz), Duration (ms)}
static int victory_song[][2] = {
    
    {392, 300}, // G4 (Low)

    // Main Theme Phrase 1
    {523, 1000},// C5 (Long)
    {392, 300}, // G4
    {349, 150}, // F4 (Fast)
    {330, 150}, // E4 (Fast)
    {294, 150}, // D4 (Fast)
    {523, 1000},// C5 (High again)

    {392, 300}, // G4
    {349, 150}, // F4
    {330, 150}, // E4
    {349, 150}, // F4
    {294, 1200},// D4 (Resolution, Low)

    // --- SECTION 2: THE REPEAT (Higher Octave / Variation) ---
    {392, 300}, // G4 (Pickup)

    {523, 1000},// C5
    {392, 300}, // G4
    {349, 150}, // F4
    {330, 150}, // E4
    {349, 150}, // F4
    {294, 1000},// D4 
    
    // The "Force" Swell
    {392, 300}, // G4
    {392, 150}, // G4 (Quick repeat)
    {523, 300}, // C5
    {587, 300}, // D5
    {659, 300}, // E5
    {523, 300}, // C5
    {392, 300}, // G4
    {440, 1200},// A4 (The grand chord change)

    // --- FINISH ---
    {0, 500}
};

static int mario_game_over[][2] = {
    // --- PHRASE 1: C Major (Descending) ---
    // "Ba - da - da"
    {523, 400}, // C5
    {392, 400}, // G4
    {330, 400}, // E4

    // --- PHRASE 2: B (Shift Down) ---
    // "Ba - da - da" (The part that was wrong in your code)
    {494, 400}, // B4
    {370, 400}, // F#4 (Gb4)
    {311, 400}, // D#4 (Eb4)

    // --- PHRASE 3: Bb (Shift Down Again) ---
    // "Ba - da - da"
    {466, 400}, // Bb4 (A#4)
    {349, 400}, // F4
    {294, 400}, // D4

    // --- PHRASE 4: Resolution ---
    // The final slow drop
    {262, 1200},// C4 (Long low note)
    
    // --- FINISH ---
    {0, 500}
};
// LED Cycling bip sound
static int LED_scan[][2] = {
    {2000, 190} // High pitch tone
         // Tiny silence buffer 
};
// Timer bips
static int BIP_count[][2] = {
    {2000, 95} // High pitch tone
         // Tiny silence buffer 
};


// State Counters (Separate for each song)
static int count_main = 0;
static int count_imperial = 0;
static int count_saber = 0;
static int count_scan = 0;
static int count_saber_off = 0;
static int count_rocky = 0;
static int count_game_over = 0;
static int count_bip = 0;
static int bips_played_in_sequence = 0;



/* --- 3. Hardware Helper  -
 * Purpose: 
 * Manipulates the Timer Registers to generate a specific audio frequency.
 * It translates a musical note (Hz) into hardware Timer Counts
 * How it works:
 * 1. FREQUENCY: Controlled by the ARR
 * - The Timer counts up to ARR and then resets.
 * - Smaller ARR = Faster Reset = Higher Pitch.
 * - Math: (Timer Clock / Target Freq) - 1.
 * 2. VOLUME (Loudness): Controlled by the CCR
 * - This determines the "Duty Cycle" (how long the pin stays HIGH per cycle).
*/
static void setHardwareTone(uint32_t freq) {
    // Map pointers to the physical memory addresses of the Timer 2 registers
    uint32_t * arr_reg = (uint32_t*)TIM2_AUTORELOAD_REGISTER;
    uint32_t * ccr_reg = (uint32_t*)TIM2_COMPARE_2_REGISTER;

    // Case 1: Silence (Rest)
    if (freq == 0) {
        // Set Duty Cycle to 0. The Pin stays LOW (0V). No sound is produced.
        *ccr_reg = 0; 
    }
    // Case 2: Play a Note 
    else {
       
        uint32_t new_arr = (1000000 / freq) - 1; 
        *arr_reg = new_arr; 
        // --- Volume Calculation ---
        // Set the switch-off point. 
        // / 2 = 50% Duty Cycle (Loudest)
        // / 4 = 25% Duty Cycle
        // / 8 = 12.5% Duty Cycle (Quieter/Softer tone)       
        *ccr_reg = new_arr / 8; // 4 for high volume, 8 for lower volume (hopefully)
    }
}
// TIMER 11 hardware tone set-up
static void setHardwareTone_TIM11(uint32_t freq) {
    uint32_t * arr_reg_2 = (uint32_t*)TIM11_AUTORELOAD_REGISTER;
    uint32_t * ccr_reg_2 = (uint32_t*)TIM11_CCR1_REGISTER;
    
    if (freq == 0) {
        *ccr_reg_2 = 0; 
    } else {
        
        uint32_t new_arr = (1000000 / freq) - 1; 
        *arr_reg_2 = new_arr;        
        *ccr_reg_2 = new_arr / 8; // 4 for high volume, 8 for lower volume (hopefully)
    }
}

// --- 4. Public Functions ---

// Ensure songs start at the beginning by starting at the begging of each array
void resetMusicCounter(void) {
    count_main = 0;
    count_imperial = 0;// count for imperial
    count_saber = 0;
    count_scan = 0;
    count_saber_off = 0;
    count_rocky = 0;
    count_game_over =0;
    // Bip counters
    count_bip = 0;
    bips_played_in_sequence = 0;
}

// Stops audio
void stopAudio(void) {

    uint32_t * ccr_reg_2 = (uint32_t*)TIM11_CCR1_REGISTER;
    uint32_t * ccr_reg = (uint32_t*)TIM2_COMPARE_2_REGISTER;
    *ccr_reg = 0; 
    *ccr_reg_2 = 0;
}



// --- Playback Functions 

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

    setHardwareTone_TIM11(freq);
    count_saber++;

    return (uint16_t)(duration / TEMPO_SABER);
}

uint16_t playLED_Scan(void){
    if (count_scan >= COUNT_SCAN) {
        count_scan = 0; 
    }

    int freq = LED_scan[count_scan][0];
    int duration = LED_scan[count_scan][1];

    setHardwareTone_TIM11(freq);
    // count_scan++;

    return (uint16_t)(duration / TEMPO_SCAN);
}

uint16_t playSABER_off(void){
    if (count_saber_off >= COUNT_SABER_OFF) {
        count_saber_off = 0; 
    }

    int freq = lightsaber_off_effect[count_saber_off][0];
    int duration = lightsaber_off_effect[count_saber_off][1];

    setHardwareTone_TIM11(freq);
    count_saber_off++;

    return (uint16_t)(duration / TEMPO_SCAN);
}

uint16_t playVictory(void) {
    if (count_rocky >= COUNT_ROCKY) {
        count_rocky = 0; 
    }

    int freq = victory_song[count_rocky][0];
    int duration = victory_song[count_rocky][1];

    setHardwareTone_TIM11(freq);
    count_rocky++;

    return (uint16_t)(duration / TEMPO_VICTORY);
}

uint16_t playGameOVer(void) {
    if (count_game_over >= COUNT_GAMEOVER) {
        count_game_over = 0; 
    }

    int freq = mario_game_over[count_game_over][0];
    int duration = mario_game_over[count_game_over][1];

    setHardwareTone_TIM11(freq);
    count_game_over++;

    return (uint16_t)(duration / TEMPO_GAME_OVER);
}

uint16_t playTimeTeller(int total_seconds) {
    // 1. Calculate how many bips we need
    int target_bips = total_seconds / 5;

    // If time is less than 5s, or we finished the sequence, stop.
    if (target_bips == 0 || bips_played_in_sequence >= target_bips) {
        bips_played_in_sequence = 0; // Reset for next safety
        return 0; 
    }

    // 2. Get Note Data from the existing Scanner Bip array
    int freq = BIP_count[count_bip][0];
    int duration = BIP_count[count_bip][1];

    // 3. Play the Tone
    setHardwareTone(freq); // Using your TIM11 function
    count_bip++;

    // 4. Check if we reached the end of the "Bip" array
    if (count_bip >= COUNT_SCAN) {
        count_bip = 0;              // Reset array to start
        bips_played_in_sequence++;  // Mark one bip as complete
        
        // 5. Inject a Gap
        // The array only has 10ms silence. We override this to 200ms
        // so the user hears distinct counts: "Beep... Beep..."
        return 200; 
    }

    return (uint16_t)duration;
}

void init_speaker1(void){
        initGpioBxAsAF1(3); //PB3 as AF1 connects to CH2
        initTimer2_CH2_PWM(); // timer2 as PWM  
}

void init_speaker2(void){
        initGpioFxAsAF3(7);// PF7 as AF3 connected to CH1
        initTimer11_CH1_PWM(); // timer 11 as PWM
}