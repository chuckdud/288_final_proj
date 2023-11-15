

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
volatile  char uart_data;
volatile  char flag;
int main(void)
{    adc_init();
     servo_init();
     uart_init(115200);
     oi_t *sensor = oi_alloc();
     ping_init();
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
//	return 0;
}
