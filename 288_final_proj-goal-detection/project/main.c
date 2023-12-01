/**
 * main.c
 */
#include "helpers.h"
#include <stdbool.h>
#include "uart.h"
#include <math.h>
#include "servo.h"
#include "adc.h"
#include "ping.h"
#include "movement.h"
#include "manual_control.h"
volatile  char uart_data;
volatile  char flag;


int main(void)
{
    adc_init();
    servo_init();
    uart_init(115200);
    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    ping_init();
    timer_init();
    lcd_init();
    lcd_clear();
    directions revDirs[15];
    move_forward(sensor, 100);
    turn_counter_clockwise(sensor, 45);
    move_forward(sensor, 100);
    turn_clockwise(sensor,45);
    move_forward(sensor, 100);
    turn_counter_clockwise(sensor, 45);
    move_forward(sensor, 100);

    reverseDirections(revDirs);
    int numDirs = 7;
    turn_counter_clockwise(sensor, 180);
    followDirections(sensor, revDirs, numDirs);
    if (strcmp(uart_receive_server(), "drive") == 0) drive(sensor);

    if(uart_receive_blocking() =='c'){
        findRC(sensor);
    }
    float vals[90];
    float IRvals[90];
    object results[5];

    // findRC()
    scan180(vals,IRvals);
    scnrAnalysis(vals, results,IRvals);
    oi_free(sensor);
    return 0;
}
