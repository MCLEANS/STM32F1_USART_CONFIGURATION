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
			

int main(void)
{
	Serial.initialize();

	char junk[] = "Hello World";
	Serial.println(junk);
	Serial.println(junk);
	Serial.println(junk);

	while(1){

	}
}
