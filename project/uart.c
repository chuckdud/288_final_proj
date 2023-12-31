/*
 *
 *   uart_extra_help.c
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code, and followig the Datasheet.
 */

#include "timer.h"
#include <math.h>
#include <uart.h>
#include <string.h>
#define REPLACE_ME 0x00

void uart_init(int baud)
{
    SYSCTL_RCGCGPIO_R |= 0b000010;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b0000010;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R |= 0b00000011; // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R |= 0x00000011; // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R |= 0b00000011;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R |= 0b00000001;        // sets pb0 as output, pb1 as input
    GPIO_PORTB_DIR_R &= ~0b00000010;

    //compute baud values [UART clock= 16 MHz] 
    double fbrd;
    int ibrd;

    fbrd = 44;
    ibrd = 8;

//    fbrd = 16000000 / (16 * 115200); // page 903 // 44
//    ibrd = (int) fbrd; // 8
//    fbrd = (int) (fbrd - ibrd * 64 + 0.5);

    UART1_CTL_R &= ~0x01;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R &= ~0b10011111;
    UART1_LCRH_R |= 0b01100000; // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R &= ~0b1111;        // use system clock as clock source (page 939)
    UART1_CTL_R |= 0x01;        // enable UART1

}

void uart_sendChar(char data)
{
    while (UART1_FR_R & 0b100000);

    if(data == '\r'){
        UART1_DR_R = data;
        uart_sendChar('\n');
    }else{
    UART1_DR_R = data;
    }
    return;
}

char uart_receive(void)
{
    while (UART1_FR_R & 0b10000);

    char result = (char) (UART1_DR_R & 0x7F);
    UART1_DR_R &= ~(0xFF);
    return result;
}

char uart_receive_blocking(void)
{
    while (UART1_FR_R & 0b10000);

    char result = (char) (UART1_DR_R & 0x7F);
    return result;
}

char *uart_receive_server(void)
{
    char tmp;
    static char data[50];
    int index = 0;
    tmp = uart_receive(); // Get first byte of the command from the Client

    // Get the rest of the command until a newline byte (i.e., '\n') received
    while(tmp != '\n' )
    {
      data[index] = tmp;  // Place byte into the command buffer
      index++;
      tmp = uart_receive(); // Get the next byte of the command
    }

    return data;
}

void uart_sendStr(const char *data)
{
    int i = 0;
    while (i < strlen(data))
    {
        uart_sendChar(data[i]);
        i++;

    }
    uart_sendChar('\r'); // TODO:: (Why) Do we need to attach this '\n' to every string? - Charlie
                         /**
                          * clear to remove the \r, but will have to manually send \n with each string
                          *     python client DOES require \n at end of each 'line'
                          *     (send_char() above automatically sends \n if it sends a \r)
                          */
}

// _PART3

void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0b10000; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0b1000000; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler()
{
// STEP1: Check the Masked Interrup Status

//STEP2:  Copy the data 

//STEP3:  Clear the interrup   

   uart_data = uart_receive(); //only a receive interrupt currently
   flag = 1;
    UART1_RIS_R &= ~0b10000;
    return;

}
