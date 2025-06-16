#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <stdlib.h>
#include "camera.h"
#include "levels/level.h"
#include "common.h"

uint16_t CameraX = 0, CameraY = 0;
uint16_t OldCameraX = 0, OldCameraY = 0;

uint8_t MapPosX = 0, MapPosY = 0;
uint8_t OldMapPosX = 0, OldMapPosY = 0;

#define level_mapWidth (level_WIDTH / level_TILE_W)
#define level_mapHeight (level_HEIGHT / level_TILE_H)

#define CameraMaxY ((level_mapHeight - DEVICE_SCREEN_HEIGHT) * 8) 
#define CameraMaxX ((level_mapWidth - DEVICE_SCREEN_WIDTH) * 8) 

#define WRAP_SCROLL_Y(y) ((y) % (DEVICE_SCREEN_BUFFER_HEIGHT * 8))

uint8_t redraw = FALSE;


void init_camera(uint8_t x, uint8_t y) {

    // Set up tile data
    set_native_tile_data(0, level_TILE_COUNT, level_tiles);
    
    // Set up color palettes
    set_bkg_palette(BKGF_CGB_PAL0, level_PALETTE_COUNT, level_palettes);


    // Initial camera position in pixels set here.
    CameraX = x;
    CameraY = y;
    // Enforce map limits on initial camera position
    if (CameraX > CameraMaxY) CameraX = CameraMaxY;
    if (CameraY > CameraMaxY) CameraY = CameraMaxY;
    OldCameraX = CameraX; OldCameraY = CameraY;

    MapPosX = CameraX >> 3;
    MapPosY = CameraY >> 3;
    OldMapPosX = OldMapPosY = 255;
    move_bkg(CameraX, WRAP_SCROLL_Y(CameraY));

    // Draw the initial map view for the whole screen
    set_bkg_submap(
        MapPosX,
        MapPosY,
        MIN(DEVICE_SCREEN_WIDTH + 1u, level_mapWidth - MapPosX),
        MIN(DEVICE_SCREEN_HEIGHT + 1u, level_mapHeight - MapPosY),
        level_map,
        level_mapWidth);

    set_bkg_submap_attributes(
        MapPosX,
        MapPosY,
        MIN(DEVICE_SCREEN_WIDTH + 1u, level_mapWidth - MapPosX),
        MIN(DEVICE_SCREEN_HEIGHT + 1u, level_mapHeight - MapPosY),
        level_map_attributes,
        level_mapWidth);

    redraw = FALSE;

    move_bkg(CameraX, WRAP_SCROLL_Y(CameraY));
    HIDE_LEFT_COLUMN;
}