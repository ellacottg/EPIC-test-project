#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "common.h"

uint8_t joypadCurrent = 0;
uint8_t joypadPrevious = 0;

uint8_t fourFrameCounter = 0;
uint8_t fourFrameRealValue = 0;

void UpdateFourFrameCounter(void){
    fourFrameCounter += 3;
    fourFrameRealValue = fourFrameCounter >> 4;

    // Stop & reset if the value is over 4
    if(fourFrameRealValue >= 4){
        fourFrameRealValue = 0;
        fourFrameCounter = 0;
    }
}