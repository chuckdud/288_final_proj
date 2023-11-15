
#define adc_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>

void adc_init(void);
int adc_read(void);
double convertRawIR(int irVal, double r, double c);
double irFindR(double dist1, double dist2, int ir1, int ir2);
double irFindC(double r, double dist, int ir);
