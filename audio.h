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
void stopAudio(void);



#ifdef __cplusplus
}
#endif

#endif // AUDIO_H