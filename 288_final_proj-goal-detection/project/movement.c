/*
 * movment.c
 *
 */
#include "movement.h"
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "helpers.h"

int bumped(oi_t *sensor){
    if (sensor->bumpLeft || sensor->bumpRight) return 1;
    return 0;
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
    if ((oi->cliffLeftSignal >= 2900) || (oi->cliffFrontLeftSignal >= 2900))
    {
        result = -1;
    }
    else if ((oi->cliffRightSignal >= 2900)|| (oi->cliffFrontRightSignal >= 2900))
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
    oi_update(oi);
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
        move_backward(oi, 100);
        turn_counter_clockwise(oi, 90);
    } else if (hole == -1){
        move_backward(oi, 100);
        turn_clockwise(oi, 90);
    } else {
        return;
    }
}

void avoid_obstacle(oi_t *sensor) {
    int left_bump = sensor->bumpLeft;
    int backtrack = 100;
    move_backward(sensor, backtrack);
//    if (left_bump) {
//        move_backward(sensor, 100);
//    } else {
//        move_backward(sensor, 100);
//    }
    //move_forward(sensor, backtrack);
    oi_setWheels(0, 0);
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
    trackDistance((milimeters * -1.0));
}

void move_forward(oi_t *sensor, int milimeters){
    if (milimeters < 0) return;
    oi_init(sensor);
    double sum = 0;
    int forward_speed = 150;
    oi_setWheels(forward_speed, forward_speed);
    while (sum < milimeters) {
        oi_update(sensor);
        if (bumped(sensor)) {
            oi_setWheels(0, 0);
            avoid_obstacle(sensor);
            oi_setWheels(forward_speed, forward_speed);
            oi_init(sensor);
            break;
        }
        if (boundDetect(sensor)) {
            boundAvoid(sensor);
            break;
        }
        if (holeDetect(sensor)) {
            holeAvoid(sensor);
            break;
        }
        sum += sensor->distance;
//        oi_init(sensor);
    }
    oi_setWheels(0, 0); // stop
    trackDistance(sum);
}

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


void followDirections(oi_t *oi, directions dirs[], int numDirs){
    int i;
    for(i = 0; i < numDirs; i++){
        if(dirs[i].angle == 0){
            if(dirs[i].distance > 0){
                move_forward(oi, dirs[i].distance);
            } else{
                move_backward(oi, (dirs[i].distance * -1));
            }
        }else{
            if(dirs[i].angle < 0){
                turn_clockwise(oi, (dirs[i].angle * -1));
            }else{
                turn_counter_clockwise(oi, dirs[i].angle);
            }

        }

    }

}
