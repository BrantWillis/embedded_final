# include "FSM.h"

FSMState NextStateFunction(void *FSM)
{
    FSMType * FSM_l = (FSMType *) FSM;
    FSMState NextState = FSM_l->CurrentState;

    // Callback function to next-state function in main.c.
    NextState = FSM_l->NextStateFuncPtr(FSM);

    return NextState;
}