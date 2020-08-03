/*
 * USART.cpp
 *
 *  Created on: Jul 20, 2020
 *      Author: MCLEANS
 */

#include "USART.h"

namespace custom_libraries {

USART::USART(USART_TypeDef *_USART,
				GPIO_TypeDef *GPIO,
				uint8_t RX_PIN,
				uint8_t TX_PIN,
				int baudrate):_USART(_USART),
								GPIO(GPIO),
								RX_PIN(RX_PIN),
								TX_PIN(TX_PIN),
								baudrate(baudrate){



}

void USART::initialize(){
	//ENABLE USART RCC
	if(_USART == USART1) RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	if(_USART == USART2) RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	if(_USART == USART3) RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

	//ENABLE GPIO RCC
	if(GPIO == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	if(GPIO == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	if(GPIO == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	if(GPIO == GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	if(GPIO == GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	//SET TX PIN TO OUTPUT
	if(TX_PIN < 8){
		GPIO->CRL |= (1<<((TX_PIN*4)));
		GPIO->CRL |= (1<<((TX_PIN*4)+1));
	}
	else{
		GPIO->CRH |= (1<<(((TX_PIN-8)*4)));
		GPIO->CRH |= (1<<(((TX_PIN-8)*4)+1));
	}
	//SET TX PIN TO ALTERNATE FUNCTION PUSH PULL
	if(TX_PIN < 8){
		GPIO->CRL &= ~(1 << (((TX_PIN*4)+2)));
		GPIO->CRL |= (1 << (((TX_PIN*4)+2)+1));
	}
	else{
		GPIO->CRH &= ~(1 << ((((TX_PIN-8)*4)+2)));
		GPIO->CRH |= (1 << ((((TX_PIN-8)*4)+2)+1));
	}

	GPIO->BSRR |= (1<<TX_PIN);

	//RX_PIN IS LEFT AT DEFAULT STATE

	//SET THE USART BAUDRATE
	if(_USART == USART1){
		uint16_t usart_div_value = (CLOCK_FREQ_APB2/(baudrate));
		_USART->BRR = ( ( ( usart_div_value / 16 ) << 4) |
		            ( ( (usart_div_value % 16 )<< 0) ) );
	}
	else{
		uint16_t usart_div_value = (CLOCK_FREQ_APB1/(baudrate));
		_USART->BRR = ( ( ( usart_div_value / 16 ) << 4) |
					( ( (usart_div_value % 16 )<< 0) ) );
	}


	//Enable RX, TX and UART
    _USART->CR1 |= USART_CR1_TE;
	_USART->CR1 |= USART_CR1_RE;
	_USART->CR1 |= USART_CR1_UE;
	_USART->CR1 |= USART_CR1_RXNEIE;
}

char USART::read_char(){
	char byte;
	byte = _USART->DR;
	return byte;
}

void USART::read_string(){
	receive_buffer[buffer_position] = read_char();
	buffer_position++;

	if(buffer_position >= BUFFER_SIZE) buffer_position = 0;
}

void USART::flush_buffer(){
	for(int i = 0; i < BUFFER_SIZE; i++) receive_buffer[i] = 0x00;
	buffer_position = 0;
}

void USART::print_char(char byte){
	while(!(_USART->SR & USART_SR_TXE)){}
	_USART->DR = byte;
}

void USART::print(char *byte){
	for(;*byte;byte++) print_char(*byte);
}

void USART::println(char *byte){
	for(;*byte;byte++) print_char(*byte);
	print_char('\n');
}


USART::~USART() {
	// TODO Auto-generated destructor stub
}

} /* namespace custom_libraries */
