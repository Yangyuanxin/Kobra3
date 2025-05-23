// STM32 serial
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_SERIAL_BAUD
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/Debug_serial_irq.h" // Debug_serial_rx_byte
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

// Select the configured serial port
#if 0//CONFIG_STM32_SERIAL_USART1
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA10,PA9");
  #define GPIO_Rx GPIO('A', 10)
  #define GPIO_Tx GPIO('A', 9)
  #define USARTx USART1
  #define USARTx_IRQn USART1_IRQn
#elif 0 //CONFIG_STM32_SERIAL_USART1_ALT_PB7_PB6
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PB7,PB6");
  #define GPIO_Rx GPIO('B', 7)
  #define GPIO_Tx GPIO('B', 6)
  #define USARTx USART1
  #define USARTx_IRQn USART1_IRQn
#elif CONFIG_STM32_SERIAL_USART2
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PA3,PA2");
  #define GPIO_Rx GPIO('A', 3)
  #define GPIO_Tx GPIO('A', 2)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
#elif CONFIG_STM32_SERIAL_USART2_ALT_PD6_PD5
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD6,PD5");
  #define GPIO_Rx GPIO('D', 6)
  #define GPIO_Tx GPIO('D', 5)
  #define USARTx USART2
  #define USARTx_IRQn USART2_IRQn
#elif 1//CONFIG_STM32_SERIAL_USART3
  //DECL_CONSTANT_STR("RESERVE_PINS_serial", "PB11,PB10");
  #define Debug_GPIO_Rx GPIO('B', 11)
  #define Debug_GPIO_Tx GPIO('B', 10)
  #define Debug_USARTx USART3
  #define Debug_USARTx_IRQn USART3_IRQn
#elif CONFIG_STM32_SERIAL_USART3_ALT_PD9_PD8
  DECL_CONSTANT_STR("RESERVE_PINS_serial", "PD9,PD8");
  #define GPIO_Rx GPIO('D', 9)
  #define GPIO_Tx GPIO('D', 8)
  #define USARTx USART3
  #define USARTx_IRQn USART3_IRQn
#endif

#define CR1_FLAGS (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE   \
                   | USART_CR1_RXNEIE)

void
Debug_USARTx_IRQHandler(void)
{
    uint32_t sr = Debug_USARTx->SR;
    if (sr & (USART_SR_RXNE | USART_SR_ORE)) {
        // The ORE flag is automatically cleared by reading SR, followed
        // by reading DR.
        Debug_serial_rx_byte(Debug_USARTx->DR);
    }
    if (sr & USART_SR_TXE && Debug_USARTx->CR1 & USART_CR1_TXEIE) {
        uint8_t data;
        int ret = Debug_serial_get_tx_byte(&data);
        if (ret)
            Debug_USARTx->CR1 = CR1_FLAGS;
        else
            Debug_USARTx->DR = data;
    }
}

void
Debug_serial_enable_tx_irq(void)
{
    Debug_USARTx->CR1 = CR1_FLAGS | USART_CR1_TXEIE;
}

void
Debug_serial_init(void)
{
    enable_pclock((uint32_t)Debug_USARTx);

    uint32_t pclk = get_pclock_frequency((uint32_t)Debug_USARTx);
    uint32_t div = DIV_ROUND_CLOSEST(pclk, 576000);
    Debug_USARTx->BRR = (((div / 16) << USART_BRR_DIV_Mantissa_Pos)
                   | ((div % 16) << USART_BRR_DIV_Fraction_Pos));
    Debug_USARTx->CR1 = CR1_FLAGS;
    armcm_enable_irq(Debug_USARTx_IRQHandler, Debug_USARTx_IRQn, 0);

    gpio_peripheral(Debug_GPIO_Rx, GPIO_FUNCTION(7), 1);
    gpio_peripheral(Debug_GPIO_Tx, GPIO_FUNCTION(7), 0);
}
DECL_INIT(Debug_serial_init);
