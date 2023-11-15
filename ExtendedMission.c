#include "open_interface.h"
#include <stdbool.h>
#include "lcd.h"
#include "driverlib/interrupt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "open_interface.h"
#include "uart.h"
#include "movement.h"
#include "adc.h"
#include "servo.h"
#include "ping.h"
#include "helpers.h"
/**
 * main.c
 */

short boundDetect(oi_t *oi)
{ //0 == no edge, -1 == left edge, 1 == right edge
    short result = 0;
    oi_update(oi);
    if ((oi->cliffLeftSignal >= 2700) || (oi->cliffFrontLeftSignal >= 2700))
    {
        result = -1;
    }
    else if ((oi->cliffRightSignal >= 2700)|| (oi->cliffFrontRightSignal >= 2700))
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}

void boundAvoid(oi_t *oi){
    short bound = boundDetect(oi);

    if(bound == 1){
        move_backward(oi, 100);
        turn_counter_clockwise(oi, 90);
    }else if(bound == -1){
        move_backward(oi, 100);
        turn_clockwise(oi, 90);
    }else{
        return;
    }

}

int main(void)
{
    oi_t *oi = oi_alloc();
    oi_init(oi);
    while (1)
    {
        oi_update(oi);
        move_forward(oi, 50);
        boundAvoid(oi);

    }

}

