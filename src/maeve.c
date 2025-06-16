#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include <stdlib.h>
#include "maeve.h"
#include "common.h"
#include "graphics/maeveSprite.h"
#include "graphics/maeveSpriteLayout.h"

#define MAEVE_MAX_SPEED 40
#define MAEVE_ACCEL 1
#define MAEVE_DECEL 3

#define MAEVE_JUMP_HEIGHT 40
#define GRAVITY 3

#define GROUND_HEIGHT 120 << 4

#define MAEVE_SHOOT_ANIM_START 0
#define MAEVE_IDLE_ANIM_START 1
#define MAEVE_WALK_ANIM_START 2
#define MAEVE_JUMP_ANIM_START 3
#define MAEVE_WALK_SHOOT_ANIM_START 6
#define MAEVE_JUMP_SHOOT_ANIM_START 7

#define MAEVE_SPRITE_COUNT 7

extern uint8_t joypadCurrent, joypadPrevious, fourFrameRealValue;

uint16_t maeveX, maeveY;
uint8_t maeveFlip = FALSE;

enum State maeveState = STATE_IDLE;
enum State previousMaeveState = STATE_IDLE;

int8_t maeveXSpeed = 0;
int8_t maeveYSpeed = 0;
uint8_t maeveMoving = FALSE;
uint8_t maeveShooting = FALSE;

uint8_t maeveGrounded = FALSE;

metasprite_t const *maeveMetaSprite;
metasprite_t const *layout;

void SetupMaeve(void)
{
    set_sprite_palette(0, maeveSprite_PALETTE_COUNT, maeveSprite_palettes);

    maeveX = 80 << 4;
    maeveY = 0 << 4;

    maeveMetaSprite = maeveSprite_metasprites[0];

    LoadSprite();

    //set OAM sprites
    for (uint8_t i = 0; i < MAEVE_SPRITE_COUNT; i++)
    {
        set_sprite_tile(i, i);
    }
}

void LoadSprite(void)
{
    metasprite_t *current_sprite = maeveMetaSprite;
    uint8_t count = 0;

    while (current_sprite->dy != metasprite_end)
    {
        set_sprite_data(count, 1, maeveSprite_tiles + current_sprite->dtile * 16);
        current_sprite++;
        count++;
    }
}

uint8_t UpdateMaeve(void)
{
    previousMaeveState = maeveState;
    maeveMoving = FALSE;

    // get the inputs and accelerate
    if (joypadCurrent & J_LEFT)
    {
        maeveFlip = TRUE;
        maeveXSpeed -= MAEVE_ACCEL;
        maeveMoving = TRUE;
    }
    if (joypadCurrent & J_RIGHT)
    {
        maeveFlip = FALSE;
        maeveXSpeed += MAEVE_ACCEL;
        maeveMoving = TRUE;
    }

    // clamp her run speed to the max speed
    if (abs(maeveXSpeed) >= MAEVE_MAX_SPEED)
    {
        maeveXSpeed = (maeveXSpeed < 0) ? -MAEVE_MAX_SPEED : MAEVE_MAX_SPEED;
    }

    // decelerate if she isn't moving and is on the ground
    if (!maeveMoving && maeveGrounded)
    {
        if (maeveXSpeed > 0)
        {
            maeveXSpeed -= MAEVE_DECEL;
            if (maeveXSpeed < 0)
            {
                maeveXSpeed = 0;
            }
        }
        if (maeveXSpeed < 0)
        {
            maeveXSpeed += MAEVE_DECEL;
            if (maeveXSpeed > 0)
            {
                maeveXSpeed = 0;
            }
        }
    }

    // move her based on how much she is accelerating
    maeveX += maeveXSpeed;

    if (maeveGrounded && (joypadCurrent & J_A))
    {
        maeveYSpeed = -MAEVE_JUMP_HEIGHT;
    }

    // gravity
    if (maeveY + maeveYSpeed < GROUND_HEIGHT)
    {
        // check if the jump is being held and maeve has upward momentum
        if ((joypadCurrent & J_A) && maeveYSpeed < 0)
        {
            maeveYSpeed += GRAVITY / 2;
        }
        else
        {
            maeveYSpeed += GRAVITY;
        }
        maeveGrounded = FALSE;
    }
    else
    {
        maeveYSpeed = 0;
        maeveY = GROUND_HEIGHT;
        maeveGrounded = TRUE;
    }

    // move maeve according to gravity
    maeveY += maeveYSpeed;

    //check if shoot has been pressed
    if (joypadCurrent & J_B)
    {
        // if maeve is not already shooting, set the shooting flag
        if (!maeveShooting)
        {
            maeveShooting = TRUE;
            //this is where we create a bullet
        }
    }
    else
    {
        // if maeve is shooting and the button is not pressed, stop shooting
        if (maeveShooting)
        {
            maeveShooting = FALSE;
        }
    }

    //set the state based on the inputs
    if(maeveShooting){
        if (!maeveGrounded) maeveState = STATE_JUMPING_SHOOTING;
        else if (maeveMoving) maeveState = STATE_WALKING_SHOOTING;
        else maeveState = STATE_SHOOTING;
    }
    else
    {
        if (!maeveGrounded) maeveState = STATE_JUMPING;
        else if (maeveMoving) maeveState = STATE_WALKING;
        else maeveState = STATE_IDLE;
    }

    // check if animations need to be changed
    if (maeveState != previousMaeveState || fourFrameChanged)
    {
        // if the state has changed, set the new animation
        setAnimation(maeveState);
    }

    //We're going to draw only one of the first two sprites, depending on if she's shooting or not
    layout = maeveSprite_layout[0];
    if (maeveShooting) layout = maeveSprite_layout[1];

    if(maeveFlip){
        return move_metasprite_flipx(layout, 0, 0, 0, maeveX >> 4, maeveY >> 4);
    }else{
        return move_metasprite_ex(layout, 0, 0, 0, maeveX >> 4, maeveY >> 4);
    }
}

void setAnimation(enum State state)
{
    switch (state)
    {
        case STATE_IDLE:
            maeveMetaSprite = maeveSprite_metasprites[MAEVE_IDLE_ANIM_START];
            break;
        case STATE_WALKING:
            maeveMetaSprite = maeveSprite_metasprites[MAEVE_WALK_ANIM_START + fourFrameRealValue];
            break;
        case STATE_JUMPING:
            maeveMetaSprite = maeveSprite_metasprites[MAEVE_JUMP_ANIM_START];
            break;
        case STATE_SHOOTING:
            maeveMetaSprite = maeveSprite_metasprites[MAEVE_SHOOT_ANIM_START];
            break;
        case STATE_WALKING_SHOOTING:
            maeveMetaSprite = maeveSprite_metasprites[MAEVE_WALK_SHOOT_ANIM_START + fourFrameRealValue];
            break;
        case STATE_JUMPING_SHOOTING:
            maeveMetaSprite = maeveSprite_metasprites[MAEVE_JUMP_SHOOT_ANIM_START];
    }

    LoadSprite();
}