

#ifndef HELPERS_H_
#define HELPERS_H_


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
#endif
