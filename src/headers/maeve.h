#ifndef MaeveHeader
#define MaeveHeader

#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_WALKING,
    STATE_JUMPING,
    STATE_SHOOTING,
    STATE_WALKING_SHOOTING,
    STATE_JUMPING_SHOOTING
};

void SetupMaeve(void);
uint8_t UpdateMaeve(void);
void LoadSprite(void);
void setAnimation(enum State state);

extern uint16_t maeveX, maeveY;

#endif