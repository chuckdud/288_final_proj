#include "helpers.h"
#include <stdbool.h>
#include "uart.h"
#include <math.h>
#include "servo.h"
#include "adc.h"
#include "ping.h"
#include "movement.h"
#define _OPEN_SYS_ITOA_EXT

extern int hitSomething;

double r = -0.5421420953;
double c = 1565.11855;
short numDirs = 0;
directions givenDirs[60];

void findRC(oi_t *sensor){
    servo_move(90);
    int x = 0;
    double ir1;
    double ir2;
    double ping1;
    double ping2;
    while (x < 16)
    {
        adc_read();
        x++;
    }
    ir1 = adc_read();

    ping1 = ping_read();
    move_backward(sensor, 300);
    x = 0;
    adc_init();
    while (x < 16)
    {
        adc_read();
        x++;
    }
    ir2 = adc_read();

    ping2 = ping_read();

    r = irFindR(ping1, ping2, ir1, ir2);
    c = irFindC(r, ping1, ir1);
    lcd_printf("C = %.10lf\nR = %.10lf", c, r);
}


void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(int num, char *str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

void send180(int *pings, float *IRvals) {
    int i = 0;
    char data[10];
    sprintf(data, "%d", 1); //TODO:: deleteme
    scan180(pings, IRvals);
    for (i = 0; i < 90; i++) {
        sprintf(data, "%d", pings[i]);
        uart_sendStr(data);
        sprintf(data, "%.1f", IRvals[i]);
        uart_sendStr(data);
    }
}

void scan180(int pings[], float IRvals[]){
    servo_move(0);
    timer_waitMillis(750);
    int angle = 0;
    int i = 0;
    for(angle = 0; angle <= 180; angle += 2){
        servo_move(angle);
        pings[i] = ping_read();
        if (pings[i] < 0) pings[i] = 100; // garbage negative ping interpreted as no object
        IRvals[i] = convertRawIR(adc_read(),r,c);
        i++;

    }
}

int scnrAnalysis(float vals[], object *results, float IRvals[])
{
    int numResult = 0;
    int numAdded = 0;
    int i = 0;
    int angleCount = 0;
    int startAngle = 0;
    int onObj = 0;
    int adjPnts = 0;

    for (i = 0; i < 89; i++)
    {
        double slope = (IRvals[i + 1] - IRvals[i]) / 2.0;

        if ((IRvals[i + 1] > 75) && onObj == 0) //If something is not close and not on object
        {
            adjPnts = 0;
        }
        else if (((IRvals[i + 1] < 60) && (onObj == 0) && slope <= -5)
                || ((abs(slope) < 10) && (adjPnts > 0) && (onObj == 0))) //finding object conditional

        {
            if (adjPnts < 2)  // Make sure that it's not noise
            {
                adjPnts++;
            }
            else if (adjPnts >= 2) // If dist is close for multiple == new object
            {
                numResult++;
                startAngle = (i - 1) * 2;
                angleCount = 2;
                onObj = 1;
                adjPnts = 0;
            }
        }
        else if ((slope >= 5) && (onObj == 1) && (IRvals[i + 1] > 65)) //End of object
        {
            if (numResult <= 1)
            {
                angleCount++;
                results->angle = startAngle;
                results->width = 2 * angleCount;
                float temp;
                //cyBOT_Scan((startAngle + angleCount), &temp);
                results->distance = vals[(startAngle + angleCount) / 2];
                //Store all data and reset vars

                results->linWidth = abs(
                        sin(results->distance / 2) * results->distance);
                numAdded++;
                onObj = 0;
                angleCount = 0;
                adjPnts = 0;

            }
            else
            {
                results++;
                angleCount++;
                angleCount++;
                results->angle = startAngle;
                results->width = 2 * angleCount;
                float temp;
                //cyBOT_Scan((startAngle + angleCount), &temp);
                results->distance = vals[(startAngle + angleCount) / 2];
                //Store all data and reset vars

                results->linWidth = abs(
                        sin(results->distance / 2) * results->distance);
                numAdded++;
                onObj = 0;
                angleCount = 0;
                adjPnts = 0;
            }
        }
        else if ((onObj == 1)) //continue counting angles
        {
            angleCount++;

        }

        else
        {
            continue;
        }
    }
    if ((numAdded < numResult) && (onObj == 1))
    { //If data ended while on obj store obj da
        results++;
        angleCount++;
        results->angle = startAngle;
        results->width = 2 * angleCount;
        float temp;
        //cyBOT_Scan((startAngle + angleCount), &temp);
        results->distance = vals[(startAngle + angleCount) / 2];

        results->linWidth = abs(sin(results->distance / 2) * results->distance);
        numAdded++;
        onObj = 0;
        angleCount = 0;
        adjPnts = 0;
    }
    else
    {

    }
    return numResult;
}

void trackAngles(float angle) {
	givenDirs[numDirs].distance = 0;
	givenDirs[numDirs].angle = angle;
	numDirs++;
}

void trackDistance(float distance) {

	if (distance > 0) {
		givenDirs[numDirs].distance = distance;
		givenDirs[numDirs].angle = 0;
		numDirs++;
	}
}

void reverseDirections(directions *revDirs) {
	int i;
	for (i = numDirs; i >= 0; i--) {
		if (givenDirs[i].distance == 0) {
//			givenDirs[i].angle *= -1;
			revDirs->angle = givenDirs[i].angle * -1;
			revDirs->distance = 0;
		} else {
			revDirs->distance = givenDirs[i].distance;
			revDirs->angle = 0;
		}
		revDirs++;
	}
}

int getNumMoves() {
	return numDirs;
}

