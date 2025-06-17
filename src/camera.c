#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <stdlib.h>
#include "camera.h"
#include "levels/tileset.h"
#include "levels/level0.h"
#include "common.h"

uint16_t cameraX = 0, cameraY = 0;
uint16_t oldCameraX = 0, oldCameraY = 0;

uint8_t mapPosX = 0, mapPosY = 0;
uint8_t oldMapPosX = 0, oldMapPosY = 0;

#define level_mapWidth (level0_WIDTH / level0_TILE_W)
#define level_mapHeight (level0_HEIGHT / level0_TILE_H)

#define cameraMaxY ((level_mapHeight - DEVICE_SCREEN_HEIGHT) * 8) 
#define cameraMaxX ((level_mapWidth - DEVICE_SCREEN_WIDTH) * 8) 

#define WRAP_SCROLL_Y(y) ((y) % (DEVICE_SCREEN_BUFFER_HEIGHT * 8))

uint8_t redraw = FALSE;


void init_camera(uint8_t x, uint8_t y) {

    // Set up tile data
    set_native_tile_data(0, tileset_TILE_COUNT, tileset_tiles);
    
    // Set up color palettes
    set_bkg_palette(0, tileset_PALETTE_COUNT, tileset_palettes);


    // Initial camera position in pixels set here.
    cameraX = x;
    cameraY = y;
    // Enforce map limits on initial camera position
    if (cameraX > cameraMaxX) cameraX = cameraMaxX;
    if (cameraY > cameraMaxY) cameraY = cameraMaxY;
    oldCameraX = cameraX; oldCameraY = cameraY;

    mapPosX = cameraX >> 3;
    mapPosY = cameraY >> 3;
    oldMapPosX = oldMapPosY = 255;
    move_bkg(cameraX, WRAP_SCROLL_Y(cameraY));

    // Draw the initial map view for the whole screen
    set_bkg_submap(
        mapPosX,
        mapPosY,
        MIN(DEVICE_SCREEN_WIDTH + 1u, level_mapWidth - mapPosX),
        MIN(DEVICE_SCREEN_HEIGHT + 1u, level_mapHeight - mapPosY),
        level0_map,
        level_mapWidth);

    redraw = FALSE;

    move_bkg(cameraX, WRAP_SCROLL_Y(cameraY));
    HIDE_LEFT_COLUMN;
}

void set_camera(void)
{
    //clamp the camera position to the map limits
    if (cameraX > 1 << 15) cameraX = 0;
    if (cameraY > 1 << 15) cameraY = 0;
    if (cameraX > cameraMaxX) cameraX = cameraMaxX;
    if (cameraY > cameraMaxY) cameraY = cameraMaxY;

    // update hardware scroll position
    move_bkg(cameraX, WRAP_SCROLL_Y(cameraY));
    // up or down
    mapPosY = (uint8_t)(cameraY >> 3u);
    if (mapPosY != oldMapPosY)
    {
        if (cameraY < oldCameraY)
        {
            set_bkg_submap(
                mapPosX,
                mapPosY,
                MIN(DEVICE_SCREEN_WIDTH + 1, level_mapWidth-mapPosX),
                1,
                level0_map,
                level_mapWidth);
        }
        else
        {
            if ((level_mapHeight - DEVICE_SCREEN_HEIGHT) > mapPosY)
            {
                set_bkg_submap(
                    mapPosX,
                    mapPosY + DEVICE_SCREEN_HEIGHT,
                    MIN(DEVICE_SCREEN_WIDTH + 1, level_mapWidth-mapPosX),
                    1,
                    level0_map,
                    level_mapWidth);
            }
        }
        oldMapPosY = mapPosY; 
    }
    // left or right
    mapPosX = (uint8_t)(cameraX >> 3u);
    if (mapPosX != oldMapPosX)
    {
        if (cameraX < oldCameraX)
        {
            set_bkg_submap(
                mapPosX,
                mapPosY,
                1,
                MIN(DEVICE_SCREEN_HEIGHT + 1, level_mapHeight - mapPosY),
                level0_map,
                level_mapWidth);
        }
        else
        {
            if ((level_mapWidth - DEVICE_SCREEN_WIDTH) > mapPosX)
            {
                set_bkg_submap(
                    mapPosX + DEVICE_SCREEN_WIDTH,
                    mapPosY,
                    1,
                    MIN(DEVICE_SCREEN_HEIGHT + 1, level_mapHeight - mapPosY),
                    level0_map,
                    level_mapWidth);
            }
        }
        oldMapPosX = mapPosX;
    }
    // set old camera position to current camera position
    oldCameraX = cameraX, oldCameraY = cameraY;
}