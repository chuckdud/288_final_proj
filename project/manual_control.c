/*
 * simple_mission.c
 *
 *  Created on: Nov 8, 2023
 *      Author: cddudley
 */

#include "movement.h"
#include "uart.h"
#include "ping.h"
#include "servo.h"
#include "adc.h"
#include "lcd.h"
#include "open_interface.h"
#include "stdlib.h"
#include "Timer.h"
#include "manual_control.h"

char *command;
char response[50];

/**
 * Receive a numerical command from server for distance or angle. Should be moved elsewhere.
 */
int receive_number(char *command) {
    command = uart_receive_server();
    while (!atoi(command)) {
        uart_sendStr("Invalid input. Please input a number. Enter 0 to cancel.");
        command = uart_receive_server();
    }
    return atoi(command);
}

void user_turn_counterclockwise(char *command) {
    uart_sendStr("Turning counterclockwise. Please enter angle (degrees).");
    int number = receive_number(command);
    sprintf(response, "Turning %d degrees counterclockwise.", number);
    uart_sendStr(response);
    turn_counter_clockwise(sensor, number);
}

void user_turn_clockwise(char *command) {
    uart_sendStr("Turning clockwise. Please enter angle (degrees).");
    int number = receive_number(command);
    sprintf(response, "Turning %d degrees clockwise.", number);
    uart_sendStr(response);
    turn_clockwise(sensor, number);
}

void user_move_forward(char *command) {
    uart_sendStr("Moving forward. Please input distance (cm).");
    number = receive_number(command);
    sprintf(response, "Moving %d cm forward.", number);
    uart_sendStr(response);
    move_forward(sensor, number);
}

void user_move_backward(char *command) {
    uart_sendStr("Moving backward. Please input distance (cm).");
    number = receive_number(command);
    sprintf(response, "Moving %d cm backward.", number);
    uart_sendStr(response);
    move_backward(sensor, number);
}

void drive(oi_t *sensor)
{
    while (1)
    {
        uart_sendStr("Please select drive mode (manual, auto).");
        command = uart_receive_server();
        if (strcmp(command, "manual") == 0)
        {
            uart_sendStr("Entering manual mode.");
            while (strcmp(command, "auto") != 0)
            {
                uart_sendStr("Please input direction for manual control (w, a, s, d).");
                command = uart_receive_server();
                if (strcmp(command, "w") == 0)
                {
                    uart_sendStr("Moving forward. Please input distance (cm).");
                    number = receive_number(command);
                    sprintf(response, "Moving %d cm forward.", number);
                    uart_sendStr(response);
                    move_forward(sensor, number);
                }
                else if (strcmp(command, "s") == 0)
                {
                    uart_sendStr("Moving backward. Please input distance (cm).");
                    number = receive_number(command);
                    sprintf(response, "Moving %d cm backward.", number);
                    uart_sendStr(response);
                    move_backward(sensor, number);
                }
                else if (strcmp(command, "a") == 0)
                {
                    uart_sendStr("Turning counterclockwise. Please enter angle (degrees).");
                    number = receive_number(command);
                    sprintf(response, "Turning %d degrees counterclockwise.", number);
                    uart_sendStr(response);
                    turn_counter_clockwise(sensor, number);
                }
                else if (strcmp(command, "d") == 0)
                {
                    uart_sendStr("Turning clockwise. Please enter angle (degrees).");
                    number = receive_number(command);
                    sprintf(response, "Turning %d degrees clockwise.", number);
                    uart_sendStr(response);
                    turn_clockwise(sensor, number);
                }
                else if (strcmp(command, "scan") == 0)
                {
                    // scan, gather info on objects
                }
            }
        }
        if (strcmp(command, "auto") == 0)
        {
            uart_sendStr("Entering autonomous mode.");
            while (strcmp(command, "manual") != 0) command = uart_receive_server();

        }
        if (strcmp(command, "quit") == 0) break;
        else uart_sendStr("Invalid command.");
    }
    oi_free(sensor);
}

