/*
 * movment.c
 *
 */
#include "movement.h"
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "helpers.h"
#include "uart.h"

int hitSomething = 0;

int bumped(oi_t *sensor) {
    if (sensor->bumpLeft || sensor->bumpRight) return 1;
    return 0;
}

void avoid_obstacle(oi_t *sensor) {
    int left_bump = sensor->bumpLeft;
    int backtrack = 150;
    if (left_bump) {
		uart_sendStr("Left bump sensor has been hit. Moving backward 10 cm.");
    } else {
		uart_sendStr("Right bump sensor has been hit. Moving backward 10 cm.");
    }
    move_backward(sensor, backtrack);
}

short boundDetect(oi_t *oi){ //0 == no edge, -1 == left edge, 1 == right edge
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

    if (bound == 1){
        move_backward(oi, 100);
        turn_counter_clockwise(oi, 90);
    } else if (bound == -1){
        move_backward(oi, 100);
        turn_clockwise(oi, 90);
    } else {
        return;
    }
}

short shinyDetect(oi_t *oi){ //0 == no edge, -1 == left edge, 1 == right edge
    short result = 0;
    oi_update(oi);
    if ((oi->cliffLeftSignal >= 2900) || (oi->cliffFrontLeftSignal >= 2900) || (oi->cliffRightSignal >= 2900)|| (oi->cliffFrontRightSignal >= 2900))
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}

short holeDetect(oi_t *oi){ //0 == no edge, -1 == left edge, 1 == right edge
    short result = 0;
//    oi_update(oi);
    if ((oi->cliffLeftSignal <= 1000) || (oi->cliffFrontLeftSignal <= 1000))
    {
        result = -1;
    }
    else if ((oi->cliffRightSignal <= 1000)|| (oi->cliffFrontRightSignal <= 1000))
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}

void holeAvoid(oi_t *oi){
    short hole = holeDetect(oi);

    if (hole == 1){
        uart_sendStr("Right cliff sensor has found a hole. Moving backward 10 cm. ");
        move_backward(oi, 100);
    } else if (hole == -1){
        uart_sendStr("Left cliff sensor has found a hole. Moving backward 10 cm. ");
        move_backward(oi, 100);
    }
}


void move_backward(oi_t *sensor, int milimeters){
    if (milimeters < 0) return;
    oi_init(sensor);
    double sum = 0;
    int backward_speed = -50;
    oi_setWheels(backward_speed, backward_speed);
    while ((-1 * sum) < milimeters) {
        oi_update(sensor);
        sum += sensor->distance;
    }
    oi_setWheels(0, 0); // stop
	// trackDistance((milimeters * -1.0)); // TODO:: why is this commented out?
}

void move_forward(oi_t *sensor, int milimeters){
    if (milimeters < 0) return;
    oi_init(sensor);
	hitSomething = 0;
	int hitDistance = 0;
    double sum = 0;
    int forward_speed = 100;
    oi_setWheels(forward_speed, forward_speed);
    while (sum < milimeters) {
        oi_update(sensor);
		// TODO:: refactor to remove duplication
        if (bumped(sensor)) {
			// these three lines are common to every obj detection
			// break out to function
			// also - should move printing into this method, rather than in each xAvoid() func
			hitSomething = 1;
			hitDistance = sensor->distance;
            oi_setWheels(0, 0);
            avoid_obstacle(sensor);
			break;
        }
		if (bound_detect(sensor)) {
			hitSomething = 1;
			hitDistance = sensor->distance;
			boundAvoid(sensor);
			break;
		}
		if (holeDetect(sensor)) {
			hitSomething = 1;
			hitDistance = sensor->distance;
			holeAvoid(sensor);
			break;
		}
		if (shinyDetect(sensor)) {
			hitSomething = 2;
			hitDistance = sensor->distance;
			oi_setWheels(0,0);
			uart_sendStr("Reached Destination!");
		}
        sum += sensor->distance;
    }
    oi_setWheels(0, 0); // stop
	// TODO:: are we accounting for -10cm backup on object bump?
	if (hitSomething) {
		trackDistance(hitDistance);
	} else {
		trackDistance(sum);
		uart_sendStr("didn't hit any objects");
	}
}

// TODO:: these two turn funcs are completely duplicates except for multiplying by -1
// 			good spot to refactor
void turn_clockwise(oi_t *sensor, int degrees){
   if (degrees < 0) return;
   oi_init(sensor);
   double turned = 0;
   oi_setWheels(-40, 40);

   while(degrees-4 > (-1 * turned)){
       oi_update(sensor);
       turned += sensor->angle;
   }
   oi_setWheels(0, 0);

   trackAngles((degrees*(-1.0)));
}

void turn_counter_clockwise(oi_t *sensor, int degrees){
   if (degrees < 0) return;
   oi_init(sensor);
   double turned = 0;
   oi_setWheels(40, -40);

   while(turned < degrees-4){
       oi_update(sensor);
       turned += sensor->angle;
   }
   oi_setWheels(0, 0);
   trackAngles((degrees*(1.0)));
}

void folowDirections(oi_t *oi, directions dirs[], int numDirs) {
	int i;
	turn_counter_clockwise(oi,180);
	for (i = 0; i < numDirs; i++) {
		if (dirs[i].angle == 0 && dirs[i].distance > 0) {
			move_forward(oi, dirs[i].distance);
		} else {
			if (dirs[i.angle] < 0) {
				turn_clockwise(oi, dirs[i].angle * -1);
			} else {
				turn_counter_clockwise(oi, dirs[i].angle);
			}
		}
	}
	uart_sendStr("Mission Complete!");
	unsigned char song[14] = {69,76,74,76,69,77,76,77,76,74,77,76,77,69};
    unsigned char duration[14] = {16,8,8,32,112,8,8,16,16,112,8,8,32,32};
    oi_loadSong(0, 14, song, duration);
    oi_play_song(0);
}
