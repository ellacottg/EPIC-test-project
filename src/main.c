#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "maeve.h"
#include "common.h"

void main(void)
{
    DISPLAY_ON;
    SHOW_SPRITES;

    uint8_t lastSprite = 0;

    SetupMaeve();

    // Loop forever
    while(1) {
        // Save the current state of the joypad
        // it's important NOT to call the joypad function more than once
        joypadCurrent = joypad();

        UpdateFourFrameCounter();

        lastSprite += UpdateMaeve();

        hide_sprites_range(lastSprite, 40);

		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}