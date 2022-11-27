
#include "MKL25Z4.h"
#include <stdint.h>
#include <string.h>

#define RX 1                  //PTA1
#define TX 2                  //PTA2
#define OSR 15                //oversample rate(like a prescaler)
#define BAUD_RATE 9600       //
#define SYS_CLOCK 4e6        //system clock

#define MASK(x) (1UL << (x))
void uart_init();
void uart_print(uint8_t data);
void uart_print_string(char* string);



int main (void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[RX] |=  PORT_PCR_MUX(2) | PORT_PCR_ISF_MASK ;
	PORTA->PCR[TX] |=  PORT_PCR_MUX(2) | PORT_PCR_ISF_MASK;
	uart_init();

	uart_print_string("\033[3B");
	uart_print_string("\033[5C");
	uart_print_string("All good students read the");
	uart_print_string("\033[1B");
	uart_print_string("\033[21D");
	uart_print_string("\033[5m");
	uart_print_string("Reference Manual");
	uart_print_string("\033[H");
	uart_print_string("\033[0m");
	uart_print_string("Input");

	while (1) {


	}
}


void uart_init(){
	    // clock gate UART0
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
		UART0->C2  &= ~ UART_C2_RE_MASK &~ UART_C2_TE_MASK ;

	    //select 4MHz clock for uart0
		SIM->SOPT2 |= SIM_SOPT2_UART0SRC(3);
		MCG->SC &= 0x00;
		MCG->C1 |= 0x02;
		MCG->C2 |= 0x01;

		//compute set baud rate (SBR), choosing baud rate of 9600 for BT
		uint16_t sbr = (uint16_t)((SYS_CLOCK)/((OSR+1) *BAUD_RATE ));	//default OSR is 15, 	sbr=26.0417 if SYS_CLOCK = 4e6
		//BAUD_RATE = (SYS_CLOCK)/[(OSR+1) * BR]

		UART0->BDL &= ~UART0_BDH_SBR_MASK;
		UART0->BDH |=  UART0_BDH_SBR(sbr>>8); 	//generic. set only bottom 5 bits
		UART0->BDL= UART0_BDL_SBR(sbr);		    //26 for this calculation
		UART0->C4 |= UART0_C4_OSR(OSR);

        //Disable Interrupts for RX active edge and LIN break detect, select one stop bit
		UART0->BDH |= UART0_BDH_RXEDGIE(0)| UART0_BDH_SBNS(0)| UART0_BDH_LBKDIE(0);

		//Don't enable loopback mode, use 8 data bit mode, don't use parity
		UART0-> C1 = UART0_C1_LOOPS(0)| UART0_C1_M(0)| UART0_C1_PE(0);

		//Don't invert transmit data, do enable interrupts for errors
		UART0-> C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(1) | UART0_C3_NEIE(1) | UART0_C3_FEIE(1);

		//Clear error flags
		UART0 -> S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1);

		//Send LSB first, do not invert received data
		UART0 -> S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

		//Enable UART transmitter and receiver
		UART0->C2  |= UART_C2_RE_MASK | UART_C2_TE_MASK ;


		NVIC_SetPriority(UART0_IRQn, 3);
		NVIC_ClearPendingIRQ(UART0_IRQn);
		NVIC_EnableIRQ(UART0_IRQn);

		UART0->C2  |= UART_C2_RIE(1);          // Rx Interrupt enabled, only RX
}

void uart_print(uint8_t data){                //print character in terminal
	while (!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0 ->D = data;
}

void uart_print_string(char* string){        //print string in terminal
	for (int i=0; i< strlen(string);i++){
		uart_print(string[i]);
	}
}


void UART0_IRQHandler(void) {
	uint8_t ch;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
			ch = UART0->D;
	}
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		// received a character
		ch = UART0->D;
		switch(ch){
		case 'R':
			uart_print_string("\033[31m");  //change text color to RED
			break;
		case 'B':
			uart_print_string("\033[34m");  //change text color to BLUE
			break;
		case 'G':
			uart_print_string("\033[32m");  //change text color to GREEN
			break;
		case 'W':
			uart_print_string("\033[37m");  //change text color to WHITE

		default:
			uart_print(ch);

		}

	}

}
