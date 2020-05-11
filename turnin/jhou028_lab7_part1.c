/*	Author: houjiacheng
 *	Lab Section: 022
 *	Assignment: Lab 7  Exercise 1
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo Link: https://drive.google.com/open?id=17-qX18nKKklhSsp3u1rXkdgJBb42w-aQ

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "io.h"
#include "timer.h"

enum States {Start, init, wait, increment, decrement, reset} state;

unsigned char tmpA0, tmpA1;
unsigned char dispNum;

void Tick() {

    switch (state)      // Transitions
    {
    case Start:
        state = init;
        break;

    case init:
        dispNum = 0;
        state = wait;
        break;

    case wait:
        if (tmpA0 && tmpA1)
        {
            state = reset;
        }
        else if (!tmpA0 && !tmpA1)
        {
            state = wait;
        }
        else if (tmpA0 && !tmpA1)
        {
            if (dispNum < 9) dispNum++;
            state = increment;
        }
        else // (!tmpA0 && tmpA1)
        {
            if (dispNum > 0) dispNum--;
            state = decrement;
        }
        break;        

    case increment:
        if (tmpA0 && tmpA1)
        {
            state = reset;
        }
        else if (tmpA0 && !tmpA1)
        {
            state = increment;
        }
        else // (!tmpA0)
        {
            state = wait;
        }
        break;
        
    case decrement:
        if (tmpA0 && tmpA1)
        {
            state = reset;
        }
        else if (!tmpA0 && tmpA1)
        {
            state = decrement;
        }
        else // (!tmpA1)
        {
            state = wait;
        }
        break;

    case reset:
        if (!tmpA0 && !tmpA1)
        {
            state = wait;
        }
        else // !(!tmpA0 && !tmpA1)
        {
            state = reset;
        }
        break;
    
    default:
        state = init;
        break;
    }

    switch (state)      // State Actions
    {
    case Start:     break;
    
    case init:      break;

    case wait:      break;

    case increment: break;
    
    case decrement: break;

    case reset:
        dispNum = 0;
        break;

    default:        break;
    }
}

void CntuIncrement() {
    if (dispNum < 9) dispNum++;
}

void CntuDecrement() {
    if (dispNum > 0) dispNum--;
}

void UpdateLCD() {
    LCD_ClearScreen();
    LCD_WriteData(dispNum + '0');
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; PORTC = 0x00; // LCD Data
    DDRD = 0xFF; PORTD = 0x00; // LCD Control

    DDRA = 0x00; PORTA = 0xFF; // Buttons

    /* Insert your solution below */
    unsigned long tick_elapsedTime = 0;
    unsigned long cntuInc_elapsedTime = 0;
    unsigned long cntuDec_elapsedTime = 0;

    const unsigned long TIMER_PERIOD = 1;

    // Init LCD
    LCD_init();

    // Timer
    TimerSet(TIMER_PERIOD);
    TimerOn();

    state = Start;

    while (1) {
        tmpA0 =  ~PINA & 0x01;
        tmpA1 = (~PINA & 0x02) >> 1;

        if (tick_elapsedTime >= 100) 
        {
            Tick();
            UpdateLCD();

            tick_elapsedTime = 0;
        }

        if (cntuInc_elapsedTime >=1000)
        {
            CntuIncrement();
            UpdateLCD();

            cntuInc_elapsedTime = 0;
        }

        if (cntuDec_elapsedTime >=1000)
        {
            CntuDecrement();
            UpdateLCD();

            cntuDec_elapsedTime = 0;
        }
        

        while (!TimerFlag); // Wait 1000 ms
        TimerFlag = 0;

        tick_elapsedTime += TIMER_PERIOD;
        cntuInc_elapsedTime = tmpA0 ? cntuInc_elapsedTime + 1 : 0;
        cntuDec_elapsedTime = tmpA1 ? cntuDec_elapsedTime + 1 : 0;

    }
    return 1;
}
