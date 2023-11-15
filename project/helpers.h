

#ifndef HELPERS_H_
#define HELPERS_H_
#include"open_interface.h"

typedef struct object
{
    int angle;
    float distance;
    int width;
    double linWidth;
} object;


void reverse(char str[], int length);
char* itoa(int num, char *str, int base);

char getSocket();

int scnrAnalysis(float vals[], object *results, float IRvals[]);

void scan180(float vals[],float IRvals[]);
void findRC(oi_t *sensor);
#endif
