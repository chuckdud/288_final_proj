#include "adc.h"
#include <math.h>





void adc_init(void){
    SYSCTL_RCGCADC_R |= 0b01;
    SYSCTL_RCGCGPIO_R |= 0b0001000;
    GPIO_PORTB_DEN_R &= 0b11101111;
    GPIO_PORTB_AMSEL_R |= 0b0001000;
    ADC0_ACTSS_R &= 0b0000;
    ADC0_EMUX_R |= 0xF;
    ADC0_SSMUX0_R &= 0b0000;
    ADC0_SSMUX0_R |= 0b1010;
    ADC0_SSCTL0_R &= 0b000;
    ADC0_SSCTL0_R |= 0b110;
    ADC0_ACTSS_R |= 0b0001;
    ADC0_SAC_R &= ~(0b111);
    ADC0_SAC_R |= 0x4;
    return;
}


int adc_read(void){
    int sensorVal;
    ADC0_SSPRI_R |= 0b0001;
    while((ADC0_RIS_R & 0b1) == 0);
    sensorVal = ADC0_SSFIFO0_R;
    ADC0_ISC_R |= 0b001;
    return sensorVal;
}

double convertRawIR(int irVal, double r, double c)
{
    double result  = c * pow(irVal, r); // #12
    return result;

}

double irFindR(double dist1, double dist2, int ir1, int ir2){
    double result = 0;

    result = ((log(dist2)-log(dist1))/(log(ir2)-log(ir1)));

    return result;

}

double irFindC(double r, double dist, int ir){
    double result = 0;
    result = dist/pow(ir , r);
    return result;
}

