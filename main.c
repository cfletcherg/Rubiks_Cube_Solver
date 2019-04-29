/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void SystemClock_Config(void);
void SendChar(char);
void SendStr(char*);
void RunMotor(int, int, char);
void LightFunction(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
	
int input1 = 0;
char input1_motor = '0';


int main(void)
{
  HAL_Init();
  SystemClock_Config();
	
	//////////  GPIOB  //////////
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Enable GPIOB system clock
	GPIOB->MODER |= (0xA << 20); // Set 10 and 11 to Alternate function mode 10
	GPIOB->MODER |= GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0; // PB3 and PB4 to input/output
	GPIOB->MODER |= GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0; // PB5 and PB6 to input/output
	GPIOB->MODER |= GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0; // PB7 and PB8 to input/output
	
	
	//////////  USART3  //////////
	RCC->APB1ENR |= (1 << 18); // USART3 clock enable
	GPIOB->AFR[1] |=  (1 << 14) | (1 << 10); //(0x44 << 8); // Set AFSEH10 and 11 to 0100 for AF4
	USART3->CR1 |= (1 << 5); // Enable the receiver interrupt
	NVIC_EnableIRQ(USART3_4_IRQn); // pass USART3 into NVIC_EnableIRQ
	NVIC_SetPriority(USART3_4_IRQn, 2); // Set USART3 NVIC Priority
	USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200; // Get the BRR from frequency/Baud rate
	USART3->CR1 |= (1 << 2) | (1 << 3); // Enable transmitter and receiver
	USART3->CR1 |= 0x1; // Enable USART
	
	
	//////////  GPIOC  //////////
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Enable GPIOC system clock
	GPIOC->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0; // Set the red LED to Alternate Function mode
	GPIOC->AFR[0] &= ~(0xFU << 24); // Set AFSEL6 to 0000 for AF0
	GPIOC->ODR |= (1 << 7) | (1 << 8) | (1 << 9);
	
	
	//////////  GPIOA  //////////
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable GPIOA system clock
	GPIOA->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0; // set PA8, PA9 and PA10 to GPIO mode
	GPIOA->ODR |= (1 << 8) | (1 << 9) | (1 << 10);
	
	
	//////////  Timer 3 Configure  //////////
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable timer 3
	TIM3->CR1 &= ~(0x3 << 5); // set CMS to edge aligned mode
	TIM3->CR1 |= (1 << 3); // counter is put on pulse mode 
	TIM3->PSC = 3999; // Sets PSC to 7999 to set clock to .001s intervals
	TIM3->ARR = 2; // Sets ARR to 10 so the interrupt happens after .01s
	TIM3->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk); // Inverse mask to set CC1S to output mode 00
	TIM3->CCMR1 |= (0x6 << 4); // Set OC1M to PWM mode 1 110
	TIM3->CCMR1 |= (1 << 3); // Preload value active
	TIM3->CCER |= TIM_CCER_CC1E; // Enable output to pin PC6
	TIM3->CCR1 = 1; // Set Preload to 5
	

  while (1)
  {

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void USART3_4_IRQHandler(void)
{

	if (input1 == 0)
	{
		char input = (uint8_t)(USART3->RDR); /* Receive data, clear flag */
		
		input1 = 1;
		input1_motor = input;
		SendChar(input);
		
	}
	
	else
	{
		char input = (uint8_t)(USART3->RDR); /* Receive data, clear flag */
		
		if (input == '1')
			RunMotor(1, 0, input1_motor);
		else if (input == '2')
			RunMotor(2, 0, input1_motor);
		else
			RunMotor(1, 1, input1_motor);
		
		input1 = 0;
		SendChar(input);
	}
	
}
void SendStr(char* x)
{
	while(*x != 0)
	{
		SendChar(*x);
		x+=1;
	}
}

void SendChar(char x)
{
	while(!(USART3->ISR & (1 << 7))){}
	USART3->TDR = x;
	
}

void RunMotor(int rot, int dir, char face)
{
	int steps = 50;
	if(face == 'U')
	{
		steps = 51;
		if(dir == 1)
			GPIOB->ODR |= GPIO_ODR_3; // turn on the direction to ccw
		GPIOC->BRR |= (1 << 7);
		
	}
	else if(face == 'R')
	{
		if(dir == 1)
		{
			GPIOB->ODR |= GPIO_ODR_4; // turn on the direction to ccw
			steps = 50;
		}
		GPIOC->BRR |= (1 << 8);
	}
	else if(face == 'F')
	{
		steps = 50;
		if(dir == 1)
		{
			GPIOB->ODR |= GPIO_ODR_5; // turn on the direction to ccw
			steps = 53;
		}
			
		GPIOC->BRR |= (1 << 9);
	}
	else if(face == 'D')
	{
		steps = 50;
		if(dir == 1)
		{
			GPIOB->ODR |= GPIO_ODR_6; // turn on the direction to ccw
			steps = 52;
		}
		GPIOA->BRR |= (1 << 8);
	}
	else if(face == 'L')
	{
		steps = 51;
		if(dir == 1)
		{
			GPIOB->ODR |= GPIO_ODR_7; // turn on the direction to ccw
			steps = 51;
		}
		
		GPIOA->BRR |= (1 << 9);
	}
	else if(face == 'B')
	{
		steps = 52;
		if(dir == 1)
		{
			GPIOB->ODR |= GPIO_ODR_8; // turn on the direction to ccw
			steps = 50;
		}
		GPIOA->BRR |= (1 << 10);
	}

		
	for(int i=0; i<steps*rot; i++)
		{
			TIM3->CR1 |= TIM_CR1_CEN; // Counter enable
			while(TIM3->CR1 & (1 << 0)){}
		}
		
	GPIOB->BRR |= (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8); // turn off the direction to ccw
	GPIOC->ODR |= (1 << 7) | (1 << 8) | (1 << 9);
	GPIOA->ODR |= (1 << 8) | (1 << 9) | (1 << 10);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/