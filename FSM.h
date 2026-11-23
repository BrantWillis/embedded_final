#ifndef FSM_H_
#define FSM_H_

#include <ti/devices/msp/msp.h>
#include "../LP_MSPM0G3507/bsp.h"
#include "music.h"

// Defines for pushbutton values
#define INACTIVE 0
#define ACTIVE 1

typedef enum {
    Stop, Start
} FSMState;

typedef struct {
    uint32_t Input1;                      // Current input of the FSM
    uint32_t Input2;                      // Current input of the FSM
    FSMState CurrentState;                      // Current state of the FSM
    FSMState (*NextStateFuncPtr)(void *FSM);    // Next-state function pointer
    void (*OutputFuncPtr)(void *FSM);           // Output function pointer
    uint32_t IntervalCounter;
    Song CurrentSong;
    uint32_t CurrentNote;
    uint32_t FailedTest; // to hold LED1 state until Start state begins
    uint32_t OscillateLED;
} FSMType;

typedef struct FSMPtrStruct {
    const FSMState CurrentState;
    const struct FSMPtrStruct *NextState[3];   // Next state of the FSM
} FSMPtrType;

// Function Prototypes
FSMState NextStateFunction(void *FSM);
void OutputFunction(void *FSM);

#endif /* FSM_H_ */
