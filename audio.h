#ifndef AUDIO_H
#define AUDIO_H
#include <cstdint>

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Macros for Everyone--------------------------------------------------------*/

/*Function definitions---------------------------------------------------------*/

// Function to play the specific song
// void playImperialMarch(void);

// void playTone(uint32_t freq, uint32_t duration);
// extern volatile uint16_t capturedValue_TIM3CH1;
void resetMusicCounter(void);
// uint16_t playMusicFunction(void);
uint16_t playMainTheme(void);
uint16_t playImperialMarch(void);
uint16_t playLightsaberEffect(void);
uint16_t playSABER_off(void);
uint16_t playLED_Scan(void);
uint16_t playVictory(void);
uint16_t playGameOVer(void);
void stopAudio(void);
void init_speaker1(void);
void init_speaker2(void);






#ifdef __cplusplus
}
#endif

#endif // AUDIO_H