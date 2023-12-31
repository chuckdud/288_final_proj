

#ifndef HELPERS_H_
#define HELPERS_H_
#include"open_interface.h"

typedef struct object {
    int angle;
    float distance;
    int width;
    double linWidth;
} object;

typedef struct directions {
	float angle;
	float distance;
} directions;

void reverse(char str[], int length);

char* itoa(int num, char *str, int base);

char getSocket();

int scnrAnalysis(float vals[], object *results, float IRvals[]);

void scan180(int pings[],float IRvals[]);

void send180(int pings[], float IRvals[]);

void findRC(oi_t *sensor);

void trackAngles(float angle);

void trackDistance(float distance);

void reverseDirections(directions *revDirs);

int getNumMoves();

#endif
