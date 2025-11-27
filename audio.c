#include "audio.h"
#include "hardware_stm_timer2.h" 

// // Imperial March Data
// // Format: {Frequency (Hz), Duration (ms)}
// static int imperial_march_analog[][2] = {
//   {440, 500}, {440, 500}, {440, 500}, {349, 350}, {523, 150},
//   {440, 500}, {349, 350}, {523, 150}, {440, 650}, {0, 150},
//   {659, 500}, {659, 500}, {659, 500}, {698, 350}, {523, 150},
//   {415, 500}, {349, 350}, {523, 150}, {440, 650}, {0, 150}
// };
// --- 1. The Music Data (New Version) ---
// Format: {Frequency (Hz), Duration (ms)}
static int imperial_march_analog[][2] = {
    {392, 350}, // G4
    {392, 350}, // G4
    {392, 350}, // G4
    
    // --- Main Theme Section A ---
    {523, 2500}, // C5 (Long)
    {784, 2500}, // G5 (Long)
    
    {698, 250}, // F5 (Triplet)
    {659, 250}, // E5 (Triplet)
    {587, 250}, // D5 (Triplet)
    {1047, 2500}, // C6 (High, Long)
    {784, 1250}, // G5 (Middle)
    
    {698, 250}, // F5 (Triplet)
    {659, 250}, // E5 (Triplet)
    {587, 250}, // D5 (Triplet)
    {1047, 2500}, // C6 (High, Long)
    {784, 1250}, // G5 (Middle)
    
    {698, 250}, // F5 (Triplet)
    {659, 250}, // E5 (Triplet)
    {698, 250}, // F5 (Triplet)
    {587, 2500}, // D5 (Long End)
    
    // --- Slight Pause ---
    {0, 250},
    
    // --- Repeat / Variation ---
    {392, 250}, // G4 
    {392, 250}, // G4 
    {392, 250}, // G4 
    
    {523, 2500}, // C5
    {784, 2500}, // G5
    
    {698, 250}, // F5
    {659, 250}, // E5
    {587, 250}, // D5
    {1047, 2500}, // C6
    {784, 1250}, // G5
    
    {698, 250}, // F5
    {659, 250}, // E5
    {587, 250}, // D5
    {1047, 2500}, // C6
    {784, 1250}, // G5

    {698, 250}, // F5
    {659, 250}, // E5
    {698, 250}, // F5
    {587, 2000}  // D5
};

// Calculate total number of notes
#define NOTES_COUNT (sizeof(imperial_march_analog) / sizeof(imperial_march_analog[0]))

// --- 2. Tempo Settings ---
// Change this to speed up or slow down the song globally!
// 1 = Original Speed (Slow)
// 5 = 5x Faster (Good for this array)
#define TEMPO_DIVIDER 0.5 

static int count = 0;

static void setHardwareTone(uint32_t freq) {
    uint32_t * arr_reg = (uint32_t*)TIM2_AUTORELOAD_REGISTER;
    uint32_t * ccr_reg = (uint32_t*)TIM2_COMPARE_2_REGISTER;
    
    if (freq == 0) {
        *ccr_reg = 0; 
    } else {
        // Frequency Math (Adjust 1000000 if your Prescaler is different)
        uint32_t new_arr = (1000000 / freq) - 1; 
        *arr_reg = new_arr;        
        *ccr_reg = new_arr / 4; 
    }
}

// --- Public Functions ---

void resetMusicCounter(void) {
    count = 0;
}

void stopAudio(void) {
    uint32_t * ccr_reg = (uint32_t*)TIM2_COMPARE_2_REGISTER;
    *ccr_reg = 0; 
}

uint16_t playMusicFunction(void) {
    
    if (count >= NOTES_COUNT) {
        count = 0; 
    }

    int freq = imperial_march_analog[count][0];
    int duration = imperial_march_analog[count][1];

    setHardwareTone(freq);

    count++;


    // Divide the duration to speed up the song
    return (uint16_t)(duration / TEMPO_DIVIDER);
}