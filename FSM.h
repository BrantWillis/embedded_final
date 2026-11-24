#ifndef FSM_H_
#define FSM_H_

#include <ti/devices/msp/msp.h>
#include "../LP_MSPM0G3507/bsp.h"
#include "music.h"

// Defines for pushbutton values
#define INACTIVE 0
#define ACTIVE 1

#define JOYSTICK_LEFT 0
#define JOYSTICK_RIGHT 1
#define JOYSTICK_CENTER 2

typedef enum {
    Stop, Start
} FSMState;

typedef struct {
    uint32_t Input1;                      // Current input of the FSM
    uint32_t Input2;                      // Current input of the FSM
    FSMState CurrentState;                      // Current state of the FSM
    FSMState (*NextStateFuncPtr)(void *FSM);    // Next-state function pointer
    uint32_t IntervalCounter;
    uint32_t SongIndex; //index of song chosen from library
    uint32_t CurrentNote;
    uint32_t JoystickDirection; //0 = left, 1 = right, 2 = center
} FSMType;

typedef struct FSMPtrStruct {
    const FSMState CurrentState;
    const struct FSMPtrStruct *NextState[3];   // Next state of the FSM
} FSMPtrType;

// Function Prototypes
FSMState NextStateFunction(void *FSM);

#endif /* FSM_H_ */
