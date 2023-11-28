//#include "open_interface.h"
//#include <stdbool.h>
//#include "lcd.h"
//#include "driverlib/interrupt.h"
//#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
//#include <inc/tm4c123gh6pm.h>
//#include "Timer.h"
//#include "open_interface.h"
//#include "uart.h"
//#include "movement.h"
//#include "adc.h"
//#include "servo.h"
//#include "ping.h"
//#include "helpers.h"
//volatile char uart_data;  // Your UART interupt code can place read data here
//volatile char flag;       // Your UART interupt can update this flag
//// to indicate that it has placed new data
//// in uart_data
//
//int main(void)
//{
////Cybot 05: -135 == 0 true deg.
////Cybot 05: 219 == 180 true
////sevo calibration values:
//    int true0 = -108;
//    int true180 = 219;
//    float range = (true180 - true0) * 1.0;
//    float scalar = range / 180.0;
//    int angle;
//    float servoAngle = (angle * scalar) + true0;
//
//    char str[100], strx[10];
//    int rawIR;
//    float sonarVals[91];
//    float calIR[91];
//    short i = 0, mode = 0; //0 == auto, 1 == man
//    timer_init();
//    lcd_init();
//    ping_init();
//    servo_init();
//    char input;
//    float scnr;
//    uart_init(115200);
//    oi_t *oi = oi_alloc();
//    oi_init(oi);
//    adc_init();
//    double distTravelled = 0;
//
//    double irR = -0.8905959977;
//    double irC = 7270.8668356444;
//    int ir1, ir2;
//    double snr1, snr2;
//    int cal;
//
//    while (cal = 0)
//    {
//        servo_move((90 * scalar) + true0);
//        uart_sendStr(
//                "In calibration Mode. Takes 2 measurements at different distances. Press c to take measurements.\n\r Calibration will end when 2 points have been logged or the user presses t\n");
//        input = uart_receive();
//        if (input == 'c')
//        {
//            int x = 0;
//            while (x < 16)
//            {
//                adc_read();
//                x++;
//            }
//            ir1 = adc_read();
//
//            snr1 = ping_read();
//            move_backward(oi, -30);
//            x = 0;
//            adc_init();
//            while (x < 16)
//            {
//                adc_read();
//                x++;
//            }
//                ir2 = adc_read();
//
//                snr2 = ping_read();
//
//            irR = irFindR(snr1, snr2, ir1, ir2);
//            irC = irFindC(irR, snr1, ir1);
//            lcd_printf("C = %.10lf\nR = %.10lf", irC, irR);
//
//            cal = 0;
//        }
//        else if (input == 't')
//        {
//            cal = 0;
//        }
//        adc_init();
//    }
//    uart_sendStr(
//            "Begins in auto Mode. Press t to change modes. Or press H to begin AutoScan\n");
//    while (0)
//    {
//        rawIR = adc_read();
//        itoa(rawIR, str, 10);
//        strcat(str, "\t\t\t");
//        float IR2 = convertRawIR(rawIR, irR, irC);
//        itoa(IR2, strx, 10);
//        strcat(str, strx);
//        uart_sendStr(str);
//    }
//
//    while (1)
//    {
//        input = uart_receive();
//        if (input == 't')
//        {
//            if (mode == 1)
//            {
//                mode = 0;
//                uart_sendStr("Entering Auto Mode\n");
//            }
//            else
//            {
//                mode = 1;
//                uart_sendStr("Entering Manual Mode\n");
//            }
//
//        }
//
//        while (!mode)
//        {
//            uart_sendStr("Press H to AutoScan\n");
//            input = uart_receive();
//            if (input == 'h')
//            {
//                uart_sendStr("Beginning auto scan\n");
//                for (angle = 0; angle <= 180; angle += 2) //Loop to iterate through scan angles
//                {
//                    servoAngle = (angle * scalar) + true0;
//                    servo_move(servoAngle);
//                    rawIR = adc_read();
//                    sonarVals[angle / 2] = ping_read();
//                    calIR[angle / 2] = convertRawIR(rawIR, irR, irC);
//                }
//                    object objects[6];
//
//                    int numObj = scnrAnalysis(sonarVals, objects, calIR); //send array to function to analyze for objects
//
//                    int numMin = 0;
//
//                    numMin = 0;
//                    for (i = 0; i < numObj; ++i)
//                    {
//                        if (objects[numMin].width > objects[i].width)
//                        {
//                            numMin = i;
//                        }
//                    }
//                    int angleMove = (objects[numMin].angle) - 90;
//                    char angle[5], distance[10];
//                    itoa(angleMove, angle, 10);
//                    itoa((objects[numMin].distance - 10), distance, 10);
//
//                    uart_sendStr("Going to turn ");
//                    uart_sendStr(angle);
//                    uart_sendStr(" degrees\n");
//                    uart_sendStr("Going to drive ");
//                    uart_sendStr(distance);
//                    uart_sendStr(" centimeters\n");
//
//                    uart_sendStr(
//                            "Press h to perform action. Any other key will decline the movement\n");
//                    input = '\00';
//                    input = uart_receive_blocking();
//                    if (input == 'h')
//                    {
//                        uart_sendStr("Executing Movement\n");
//                        oi_init(oi);
//                        if (angleMove <= 0)
//                        {
//                            turn_clockwise(oi, angleMove);
//                        }
//                        else
//                        {
//                            turn_counterclockwise(oi, angleMove);
//                        }
//                        distTravelled = 0;
//                        while ((distTravelled + 1
//                                < ((int) (objects[numMin].distance - 10))))
//                        {
//
//                           distTravelled += move_forward(
//                                    oi,
//                                    (((int)objects[numMin].distance - 10
//                                            - distTravelled)));
//                            if (oi->bumpLeft)
//                            {
//                                uart_sendStr(
//                                        "Object struck left bumper. Attempting to move around\n");
//                                move_backward(oi, -10);
//                                turn_clockwise(oi, -90);
//                                move_forward(oi, 10);
//                                turn_counterclockwise(oi, 90);
//                                distTravelled = 100000;
//
//                            }
//                            else if (oi->bumpRight)
//                            {
//                                uart_sendStr(
//                                        "Object struck right bumper. Attempting to move around\n");
//                                move_backward(oi, -10);
//                                turn_counterclockwise(oi, 90);
//                                move_forward(oi, 10);
//                                turn_clockwise(oi, -90);
//                                distTravelled = 1000000;
//                            }
//                        }
//
//                    }
//
//                }
//                if (input == 't')
//                {
//                    mode = 1;
//                    uart_sendStr("Entering Manual Mode\n");
//                }
//                distTravelled = 0;
//            }
//
//            while (mode)
//            {
//                input = uart_receive();
//                if (input == 'm')
//                {
//                    uart_sendStr("Beginning Manual Scan");
//                    for (angle = 0; angle <= 180; angle += 2) //Loop to iterate through scan angles
//                    {
//                        servoAngle = (angle * scalar) + true0;
//                        //servo_init();
//                        servo_move(servoAngle);
//                        //adc_init();
//                        rawIR = adc_read();
//                        sonarVals[angle / 2] = ping_read();
//                        calIR[angle / 2] = convertRawIR(rawIR, irR, irC);
//                    }
//
//                    char str2[25];
//
//                    uart_sendStr(
//                            "Object#        Angle         Distance        Angular Width         Linear Width\n");
//
//                    object objects[6];
//
//                    int numObj = scnrAnalysis(sonarVals, objects, calIR); //send array to function to analyze for objects
//
//                    int numMin = 0;
//                    for (i = 0; i < numObj; i++)
//                    {
//
//                        itoa((i + 1), str, 10);
//                        itoa((int)objects[i].angle, str2, 10);
//                        strcat(str, "\t\t");
//                        strcat(str, str2);
//                        itoa(objects[i].distance, str2, 10);
//                        strcat(str, "\t\t");
//                        strcat(str, str2);
//                        itoa(objects[i].width, str2, 10);
//                        strcat(str, "\t\t");
//                        strcat(str, str2);
//                        itoa(objects[i].linWidth, str2, 10);
//                        strcat(str, "\t\t");
//                        strcat(str, str2);
//
//                        uart_sendStr(str);
//                    }
//                    objects[numObj - 1].width += 4;
//                    numMin = 0;
//                    for (i = 0; i < numObj; ++i)
//                    {
//                        if (objects[numMin].width > objects[i].width)
//                        {
//                            numMin = i;
//                        }
//                    }
//                }
//
//                else if (input == 'w')
//                {
//                    while (distTravelled < 5)
//                    {
//                        distTravelled += move_forward(oi, 5 - distTravelled);
//                        if (oi->bumpLeft)
//                        {
//                            move_backward(oi, -15);
//                            turn_clockwise(oi, -90);
//                            move_forward(oi, 15);
//                            turn_counterclockwise(oi, 90);
//                            distTravelled -= 10;
//                        }
//                        else if (oi->bumpRight)
//                        {
//                            move_backward(oi, -15);
//                            turn_counterclockwise(oi, 90);
//                            move_forward(oi, 15);
//                            turn_clockwise(oi, -90);
//                            distTravelled -= 10;
//                        }
//                    }
//                    distTravelled = 0;
//                }
//
//                else if (input == 's')
//                {
//                    move_backward(oi, -5);
//                }
//
//                else if (input == 'a')
//                {
//                    turn_counterclockwise(oi, 10);
//                }
//
//                else if (input == 'd')
//                {
//                    turn_clockwise(oi, -10);
//
//                }
//                else if (input == 't')
//                {
//                    mode = 0;
//                    uart_sendStr("Entering Auto Mode\n");
//                }
//
//            }
//
//        }
//
//        return 0;
//    }
//
