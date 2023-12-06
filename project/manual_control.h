/*
 * manual_control.h
 *
 *  Created on: Nov 28, 2023
 *      Author: cddudley
 */

#ifndef MANUAL_CONTROL_H_
#define MANUAL_CONTROL_H_

int receive_number();

void set_location();

void user_turn_counterclockwise(oi_t *sensor);

void user_turn_clockwise(oi_t *sensor);

void user_move_forward(oi_t *sensor);

void user_move_backward(oi_t *sensor);

void drive(oi_t *sensor);


#endif /* MANUAL_CONTROL_H_ */
