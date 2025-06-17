#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "camera.h"
#include "maeve.h"
#include "common.h"

extern uint16_t cameraX, cameraY, maeveX, maeveY;

void main(void)
{
    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;

    uint8_t lastSprite = 0;

    init_camera(0, 0);

    SetupMaeve();

    // Loop forever
    while(1) {
        lastSprite = 0;

        // Save the current state of the joypad
        // it's important NOT to call the joypad function more than once
        joypadCurrent = joypad();

        UpdateFourFrameCounter();

        // center the camera on Maeve
        cameraX = (maeveX >> 4) - (DEVICE_SCREEN_WIDTH << 2);
        cameraY = (maeveY >> 4) - (DEVICE_SCREEN_HEIGHT * 6);

        set_camera();

        lastSprite += UpdateMaeve();

        hide_sprites_range(lastSprite, 40);

		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}