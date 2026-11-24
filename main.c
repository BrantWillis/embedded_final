#include <ti/devices/msp/msp.h>

//button interrupts (placed here to not have to resubmit MKII.h)
#define BP_S1_POL_RISING ((uint32_t) (GPIO_POL_RISING << (2*(BP_S1_PIN))))
#define BP_S1_POL_FALLING ((uint32_t) (GPIO_POL_FALLING << (2*(BP_S1_PIN))))
#define BP_S1_INTERRUPT GPIOA_INT_IRQn

#define BP_S2_POL_RISING ((uint32_t) (GPIO_POL_RISING << (2*(BP_S2_PIN))))
#define BP_S2_POL_FALLING ((uint32_t) (GPIO_POL_FALLING << (2*(BP_S2_PIN))))
#define BP_S2_INTERRUPT GPIOA_INT_IRQn

#define PROJECT_CLOCK_FREQUENCY 32000000
#define PROJECT_ONE_MICROSECOND_DELAY 32
#define PROJECT_ONE_MILLISECOND_DELAY 32000

#define FALSE 0
#define TRUE 1

#include "../LP_MSPM0G3507/bsp.h"
#include "../LP_MSPM0G3507/timer.h"
#include "../LP_MSPM0G3507/mspm0g350x_int.h"
#include "FSM.h"
#include "tone_pitch.h"
#include "../LP_MSPM0G3507/MKII.h"
#include "music.h"
#include "song_lib.h"
#include "../LP_MSPM0G3507/LCD.h"

#include <stdio.h>

// Function prototypes
FSMState NextStateDiagram(void *FSM);

// Expected 2x4 Transition Table - row 1 stop, row 2 start
// col 1 - s1 off, s2 off
// col 2 - s1 off, s2 on
// col 3 - s1 on, s2 off
// col 4 - s1 on, s2 on
static const FSMState ExpectedTransitions[2][4] = {
    {Stop, Stop, Start, Stop}, //stop
    {Start, Stop, Start, Stop} //start
};

const Song MaryHadALittleLambSong = {
    .notes = MaryHadALittleLamb,
    .length = 26,
    .name = "Mary Had a Little Lamb"
};

const Song HappyBirthdaySong = {
    .notes = HappyBirthday,
    .length = 25,
    .name = "Happy Birthday       "
};

const Song MyFavoriteThingsSong = {
    .notes = MyFavoriteThings,
    .length = 42,
    .name = "My Favorite Things   "
};

int refresh_flag = FALSE;

// Global variable for Timer G8 interrupt callback function.
void TimerG8CallbackFunction(void *CallbackObject);
Callback_s TimerG8 = {(void *) 0,TimerG8CallbackFunction};

void Group1CallbackFunction(void *CallbackObject);

uint32_t ConvertFrequency(uint32_t NoteFrequency);

// Boolean structure to determine if pushbutton has been pushed.
typedef struct {
    uint32_t S1;
    uint32_t S2;
} Pushed;

// Global variables
Callback_s Group1 = {(void *) 0,Group1CallbackFunction};

// Global variables for timer configurations.
// Timer 8 used for time delay.
Timer_ClockConfig TimerG8ClockConfig =
{
 .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
 .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
 .prescale = 255
};
Timer_TimerConfig TimerG8TimerConfig =
{
 .period = 1953,
 .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1)
};
Timer_InterruptConfig TimerG8InterruptConfig = 
{
    .imask = GPTIMER_CPU_INT_IMASK_Z_SET,
    .IRQnum = TIMG8_INT_IRQn,
    .priority = 2
};

Timer_ClockConfig TimerA1ClockConfig =
{
 .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
 .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
 .prescale = 31
};
Timer_TimerConfig TimerA1TimerConfig =
{
 .period = 1000,
 .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1)
};
// Timer A1 used to produce PWM on buzzer. Buzzer output is PB4,
// connected to Timer A1, capture/compare register C0 (mode 4)
Timer_PWMConfig TimerA1PWMConfig =
{
 .index = PB4INDEX,
 .iomuxMode = (IOMUX_PINCM_PC_CONNECTED | IOMUX_MODE4),
 .ccr = 0,
 .ccpd = GPTIMER_CCPD_C0CCP0_OUTPUT,
 .ccctl = GPTIMER_CCCTL_01_COC_COMPARE,
 .octl = GPTIMER_OCTL_01_CCPO_FUNCVAL,
 .ccact = (GPTIMER_CCACT_01_CDACT_CCP_LOW | GPTIMER_CCACT_01_LACT_CCP_HIGH),
 .duty = 1000,
};

int main (void) {
    FSMType Buzzer_Pushbutton_FSM = {
        .Input1 = INACTIVE,
        .Input2 = INACTIVE,
        .CurrentState = Stop,
        .NextStateFuncPtr = NextStateDiagram,
        .FailedTest = FALSE,
        .IntervalCounter = 0,
        .CurrentSong = MyFavoriteThingsSong,
        .CurrentNote = NOTE_SILENT,
        .OscillateLED = 0,
        .JoystickDirection = JOYSTICK_CENTER
    };

    InitializeBoosterpack(CLOCK_FREQUENCY);

    LCD_Init();
    LCD_FillScreen(LCD_BLACK);
    LCD_SetTextColor(LCD_CYAN);

    Group1.CallbackObject = (void *) &Buzzer_Pushbutton_FSM;

    __disable_irq();

    BP_S1_PORT->POLARITY15_0 = BP_S1_POL_RISING;   // Rising-edge interrupt
    BP_S1_PORT->CPU_INT.ICLR = BP_S1_MASK;            // Clear interrupt bit
    BP_S1_PORT->CPU_INT.IMASK = BP_S1_MASK;           // Enable the S1 interrupt.

    BP_S2_PORT->POLARITY15_0 |= BP_S2_POL_RISING;    // Rising-edge interrupt
    BP_S2_PORT->CPU_INT.ICLR |= BP_S2_MASK;            // Clear interrupt bit
    BP_S2_PORT->CPU_INT.IMASK |= BP_S2_MASK;           // Enable the S2 interrupt (using OR so both interrupts are acitve)

    // Enable NVIC interrupt and set interrupt priority.
    __NVIC_EnableIRQ(BP_S1_INTERRUPT);

    __NVIC_EnableIRQ(BP_S2_INTERRUPT);
    __NVIC_SetPriority(BP_S2_INTERRUPT,0);

    __enable_irq();

    // Configure timer A1 to generate the PWM output.
    InitializeTimerClock(TIMA1,&TimerA1ClockConfig);
    InitializeTimerCompare(TIMA1,&TimerA1TimerConfig);
    InitializeTimerPWM(TIMA1,&TimerA1PWMConfig);
    EnableTimer(TIMA1);

    // Configure timer G8 to use for the time delay.
    __disable_irq();
    TimerG8.CallbackObject = (void *) &Buzzer_Pushbutton_FSM;
    InitializeTimerClock(TIMG8,&TimerG8ClockConfig);
    InitializeTimerCompare(TIMG8,&TimerG8TimerConfig);
    InitializeTimerInterrupt(TIMG8,&TimerG8InterruptConfig);
    EnableTimer(TIMG8);
    __enable_irq();

    char OutputBuffer[200];

    //Text on the LCD: the song name, the name of each note as it plays,
    //and the player status (playing or stopped), updating as S1 starts
    //and S2 stops the song

    char *what;
    char *what2;

    while (1) {
        __WFI(); // wait for interrupt
        //LCD_FillScreen(LCD_BLACK);
        LCD_SetCursor(0, 0);
        what = (Buzzer_Pushbutton_FSM.CurrentState ? "Playing" : "Paused");
        if(Buzzer_Pushbutton_FSM.JoystickDirection == JOYSTICK_LEFT) what2 = "Left";
        else if (Buzzer_Pushbutton_FSM.JoystickDirection == JOYSTICK_RIGHT) what2 = "Right";
        else what2 = "Center";
        snprintf(OutputBuffer, sizeof(OutputBuffer), "%s\nNote: %s\n%s\nJoystick: %s\n",
            Buzzer_Pushbutton_FSM.CurrentSong.name,
            Buzzer_Pushbutton_FSM.CurrentSong.notes[Buzzer_Pushbutton_FSM.CurrentNote].name,
            what,
            what2
            );
        LCD_OutString(OutputBuffer);
        /*if (refresh_flag == TRUE) {
            UpdateDisplay();
            refresh_flag = FALSE;
        }*/
    }

    return 0;
}

#define STOP               &TransitionDiagram[0]
#define START           &TransitionDiagram[1]

const FSMPtrType TransitionDiagram[2] = {
    //   Current State          Next State: s2 on, s1 on, s1 off
    {Stop, {STOP, START, STOP}},
    {Start, {STOP, START, START}},
};

FSMState NextStateDiagram(void *FSM)
{
    FSMType * FSM_l = (FSMType *) FSM;
    FSMState NextState = FSM_l->CurrentState;

    //index 0 if s2 on, otherwise 1 if s1 on, 2 if s1 off
    uint32_t CallIndex = 0;
    if (FSM_l->Input2 == 0) {
        CallIndex++;
        if(FSM_l->Input1 == 0) CallIndex++;
    }

    // TODO: Look up entry in TransitionDiagram and dereference the NextState pointer
    NextState = TransitionDiagram[NextState].NextState[CallIndex]->CurrentState;

    return NextState;
}

void PlayNote(uint32_t frequency) {
    uint32_t newPer = ConvertFrequency(frequency);
    
    //50% duty cycle, period from converted frequency
    TimerA1PWMConfig.duty = newPer / 2;
    UpdateDutyCycle(TIMA1,&TimerA1PWMConfig);
    
    TimerA1TimerConfig.period = newPer;
    TIMA1->COUNTERREGS.LOAD = TimerA1TimerConfig.period;
}

//note change timer callback
void TimerG8CallbackFunction(void *CallbackObject) {
    FSMType *FSM_l = (FSMType *)CallbackObject;

    if (FSM_l->CurrentState == Stop) {
        PlayNote(NOTE_SILENT);
        refresh_flag = TRUE;
    }
    else if (TIMG8->CPU_INT.IIDX == GPTIMER_CPU_INT_IIDX_STAT_Z && FSM_l->CurrentState == Start) {
        FSM_l->IntervalCounter += 1;
        Note note = FSM_l->CurrentSong.notes[FSM_l->CurrentNote];
        if(FSM_l->IntervalCounter == note.delay) {
            FSM_l->CurrentNote += 1;
            FSM_l->IntervalCounter = 0;
            PlayNote(NOTE_SILENT);
            refresh_flag = TRUE;
        }
        else {
            PlayNote(note.note_frequency);
        }

        //loop around functionality
        if(FSM_l->CurrentNote == FSM_l->CurrentSong.length) {
            FSM_l->CurrentNote = 0;
            FSM_l->CurrentSong = HappyBirthdaySong;
        }
    }
}

void Group1CallbackFunction(void *CallbackObject) {
    FSMType *FSM_l = (FSMType *)CallbackObject;

    //two reads from the same port, each could be either S1 or S2
    uint32_t read1 = BP_S1_PORT->CPU_INT.IIDX;
    uint32_t read2 = BP_S2_PORT->CPU_INT.IIDX;

    FSM_l->Input1 = FALSE;
    FSM_l->Input2 = FALSE;
    if (read1 == (BP_S1_PIN+1) || read2 == (BP_S1_PIN+1)) {
        FSM_l->Input1 = TRUE;
    }
    if (read1 == (BP_S2_PIN+1) || read2 == (BP_S2_PIN+1)) {
        FSM_l->Input2 = TRUE;
    }

    FSM_l->CurrentState = NextStateFunction(FSM_l);
}

//use 0Hz as silence, otherwise use formula
uint32_t ConvertFrequency(uint32_t NoteFrequency) {
    if(NoteFrequency == 0) return 0;
    return (PROJECT_CLOCK_FREQUENCY / ((32) * NoteFrequency));
}