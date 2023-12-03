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

    servo_move(0);

	// TODO:: get rid of this step, just go into manual mode
    if (strcmp(uart_receive_server(), "drive") == 0) drive(sensor);

    oi_free(sensor);
    return 0;
}
