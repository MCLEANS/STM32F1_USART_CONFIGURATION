/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "USART.h"

custom_libraries::USART	Serial(USART1,GPIOA,10,9,9600);

extern "C" void USART1_IRQHandler(){
	//Check if we are here because USART1 received data
	if(USART1->SR & USART_SR_RXNE){
		//Disable the flag
		USART1->SR &= ~USART_SR_RXNE;
		//read USART->DR into receive buffer
		Serial.read_string();

	}
}

int main(void)
{
	Serial.initialize();

	//Setup UART NVIC
	NVIC_SetPriority(USART1_IRQn,0x03);
	NVIC_EnableIRQ(USART1_IRQn);

	while(1){
		Serial.println(Serial.receive_buffer);
		for(int i=0;i<100000;i++){}
	}
}
