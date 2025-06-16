#ifndef CommonHeader
#define CommonHeader

#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

void UpdateFourFrameCounter(void);

extern uint8_t joypadCurrent, joypadPrevious, fourFrameRealValue, fourFrameChanged;

#endif