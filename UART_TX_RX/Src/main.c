
#include <stdint.h>
#include"stm32f4xx.h"
#define GPIOAEN (1U<<0)
#define UART2EN (1U<<17)

#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE 115200
#define CR1_TE  (1U<<3)
#define CR1_UE  (1U<<13)

#define CR1_RE  (1U<<2)


#define SR_TXE  (1U<<7)
#define SR_RXNE  (1U<<5)


void uart2_rxtx_init(void);
void uart2_write(int ch);
static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk,uint32_t Baudrate);
char uart2_read(void);

static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t Baudrate);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

int main(void)
{
char key;
	uart2_rxtx_init();

	while(1)
	{
		key=uart2_read();
		if (key=='1')
		{
			printf("1 is pressed \n \r");
		}
		else
		{
			printf("2 is pressed \n \r");
		}

		//uart2_write('A');
	}


}

void uart2_rxtx_init(void)
{
	RCC->AHB1ENR |=GPIOAEN;
///PA2  -> TRANSMIT
	GPIOA->MODER &=~(1U <<4);
	GPIOA->MODER |= (1U <<5);
///PA3  -> RECEIVE
	GPIOA->MODER &=~(1U <<6);
    GPIOA->MODER |= (1U <<7);


    ///PA2  -> ALTERNATE


	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);

    ///PA3  -> ALTERNATE

	GPIOA->AFR[0] |=(1U<<12);
	GPIOA->AFR[0] |=(1U<<13);
	GPIOA->AFR[0] |=(1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);



	RCC->APB1ENR |=UART2EN;
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	USART2->CR1 =CR1_TE |CR1_RE;
	USART2->CR1 |=CR1_UE;


}


static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t Baudrate)
{
	return ((PeriphClk+(Baudrate/2U))/Baudrate);
}


char uart2_read(void)
{
	while(!(USART2->SR  & SR_RXNE)){}	//make sure the data register is not empty
	return USART2->DR;
}

void uart2_write(int ch)
{
	while(!(USART2->SR  & SR_TXE)){}	// make sure the transmit data register is empty
	USART2->DR =(ch&0XFF) ;
}

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk,uint32_t Baudrate)
{
USARTx->BRR =compute_uart_bd(PeriphClk,Baudrate);
}
