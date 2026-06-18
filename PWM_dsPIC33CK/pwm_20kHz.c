/*
 * File   : pwm_20kHz.c
 * Device : dsPIC33CK (e.g., dsPIC33CK256MP508)
 * Tool   : MPLAB XC16 Compiler
 *
 * Generates 20 kHz, 50% duty cycle PWM on PWM1H pin (RB14).
 *
 * Clock assumptions:
 *   FRC = 8 MHz  →  PLL  →  Fosc = 200 MHz  →  Fcy = 100 MHz
 *   PWM clock source = Fosc / 2 = 100 MHz
 *
 * PWM math (left-aligned / independent edge mode):
 *   MPER = (PWM_CLK / Fpwm) - 1 = (100e6 / 20e3) - 1 = 4999
 *   MDC  = (MPER + 1) / 2        = 2500   →  50 % duty cycle
 */

// ---------------------------------------------------------------------------
// Configuration Bits
// ---------------------------------------------------------------------------
#pragma config FNOSC    = FRCPLL    // Oscillator: FRC with PLL
#pragma config FCKSM    = CSECMD    // Clock switching enabled, Fail-Safe Monitor disabled
#pragma config JTAGEN   = OFF       // JTAG disabled
#pragma config ICS      = PGD1      // ICD comm channel: PGD1/PGC1
#pragma config PWMLOCK  = OFF       // PWM registers not write-protected

#include <xc.h>
#include <stdint.h>

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
#define FCY          100000000UL    // Instruction cycle freq (Hz)

#define PWM_FREQ_HZ  20000UL        // Target PWM frequency
#define PWM_CLK_HZ   100000000UL    // PWM module clock after PCLKCON divider

#define PWM_PERIOD   (PWM_CLK_HZ / PWM_FREQ_HZ)        // 5000 counts
#define PWM_MPER     (uint16_t)(PWM_PERIOD - 1U)        // 4999  → period register
#define PWM_MDC      (uint16_t)(PWM_PERIOD / 2U)        // 2500  → 50 % duty cycle

// ---------------------------------------------------------------------------
// Clock Initialisation — FRC + PLL → Fosc = 200 MHz, Fcy = 100 MHz
//
//   N1  = PLLPRE  = 1   →  FPLLI = 8 MHz
//   M   = PLLFBDIV= 100 →  FVCO  = 800 MHz
//   N2  = POST1DIV= 2   |
//   N3  = POST2DIV= 2   →  Fosc  = 800 / (2×2) = 200 MHz
//   Fcy = Fosc / 2      = 100 MHz
// ---------------------------------------------------------------------------
void Clock_Init(void)
{
    CLKDIVbits.PLLPRE      = 1;    // N1 = 1
    PLLFBDbits.PLLFBDIV    = 100;  // M  = 100
    PLLDIVbits.POST1DIV    = 2;    // N2 = 2
    PLLDIVbits.POST2DIV    = 2;    // N3 = 2

    // Request switch to FRCPLL
    __builtin_write_OSCCONH(0x01);              // NOSC = FRCPLL
    __builtin_write_OSCCONL(OSCCON | 0x01);     // OSWEN = 1: initiate switch

    while (OSCCONbits.OSWEN);   // Wait for switch to complete
    while (!OSCCONbits.LOCK);   // Wait for PLL to lock
}

// ---------------------------------------------------------------------------
// PWM Initialisation — PWM Generator 1, 20 kHz, 50 % duty, left-aligned
// ---------------------------------------------------------------------------
void PWM1_Init(void)
{
    // Disable PG1 during configuration
    PG1CONLbits.ON = 0;

    // --- Master PWM Clock ---
    // MCLKSEL = 10 → FOSC (200 MHz)
    // DIVSEL  = 01 → ÷ 2  →  PWM clock = 100 MHz
    PCLKCONbits.MCLKSEL = 0b10;
    PCLKCONbits.DIVSEL  = 0b01;

    // --- Master Period & Duty Cycle ---
    MPER = PWM_MPER;    // 4999  →  T_pwm = 5000 × 10 ns = 50 µs = 20 kHz
    MDC  = PWM_MDC;     // 2500  →  50 % duty cycle

    // --- PG1CONL ---
    PG1CONLbits.CLKSEL = 0b00;   // Master clock (from PCLKCON)
    PG1CONLbits.MODSEL = 0b000;  // Independent edge PWM (left-aligned)
    PG1CONLbits.TRGCNT = 0b000;  // Trigger every PWM cycle

    // --- PG1CONH ---
    PG1CONHbits.MDCSEL  = 1;        // Use MDC (master duty cycle register)
    PG1CONHbits.MPERSEL = 1;        // Use MPER (master period register)
    PG1CONHbits.MPHSEL  = 0;        // Use PG1PHASE (individual phase)
    PG1CONHbits.MSTEN   = 0;        // Not a master to other generators
    PG1CONHbits.SOCS    = 0b0000;   // Self-triggered: runs immediately
    PG1CONHbits.UPDMOD  = 0b000;    // Update at start-of-cycle (SOC)

    // --- PG1IOCONH: Independent output mode — only PWM1H used ---
    PG1IOCONHbits.PMOD  = 0b01;  // Independent outputs

    // --- PG1IOCONL: output polarity & override ---
    PG1IOCONLbits.POLH   = 0;    // PWM1H active-high
    PG1IOCONLbits.POLL   = 0;    // PWM1L active-high
    PG1IOCONLbits.OVRENH = 0;    // PWM1H controlled by PWM module
    PG1IOCONLbits.OVRENL = 1;    // PWM1L overridden to driven-low (unused)

    // --- Phase offset (none) ---
    PG1PHASE = 0;

    // --- Dead-time (not needed for single output; set to zero) ---
    PG1DTL = 0;
    PG1DTH = 0;

    // --- Enable PWM Generator 1 ---
    PG1CONLbits.ON = 1;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(void)
{
    Clock_Init();

    // PWM1H is mapped to RB14 on most dsPIC33CK packages
    ANSELBbits.ANSB14 = 0;   // Disable analog on RB14
    TRISBbits.TRISB14 = 0;   // RB14 as digital output

    PWM1_Init();

    // PWM output runs autonomously via hardware.
    // Modify MDC at runtime to change duty cycle, e.g.:
    //   MDC = (uint16_t)((PWM_PERIOD * duty_pct) / 100U);
    while (1)
    {
        // Application code here
    }

    return 0;
}
