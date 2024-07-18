/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with the this file, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "vl53l5cx_api.h"
#include "platform.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define VL53L5CX_DEFAULT_ADDRESS 0x30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
VL53L5CX_Configuration dev;
VL53L5CX_ResultsData results;
uint8_t sensor_address = 0x31; // New address for the sensor
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Scan_I2C_Devices();
void Initialize_Sensor(void);
void ProcessData(VL53L5CX_ResultsData *results);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int _write(int file, char* ptr, int len){
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  // Test UART
  printf("UART działa poprawnie\r\n");

  Scan_I2C_Devices();

  // Initialize the sensor
  Initialize_Sensor();

  // Start measurements for the sensor
  printf("Rozpoczynanie pomiarów...\r\n");
  int status = vl53l5cx_start_ranging(&dev);
  if (status == VL53L5CX_STATUS_OK) {
      printf("Pomiary rozpoczęte pomyślnie\r\n");
  } else {
      printf("Błąd rozpoczynania pomiarów, kod błędu: %d\r\n", status);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint8_t isReady;
    if (vl53l5cx_check_data_ready(&dev, &isReady) == VL53L5CX_STATUS_OK && isReady) {
        vl53l5cx_get_ranging_data(&dev, &results);
        ProcessData(&results);
    } else {
        printf("Czujnik nie ma nowych danych do odczytu\r\n");
    }
    HAL_Delay(5000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void ProcessData(VL53L5CX_ResultsData *results) {
    printf("Sensor:\n\r");
    for (int i = 0; i < 16; i += 4) { // Displaying 4 measurements per line
        printf("Distance %d-%d: %d mm, %d mm, %d mm, %d mm\n\r",
               i, i+3,
               results->distance_mm[i],
               results->distance_mm[i+1],
               results->distance_mm[i+2],
               results->distance_mm[i+3]);
    }
}

void Initialize_Sensor(void) {
    uint8_t status;
    uint8_t is_alive = 0;

    for (int attempts = 0; attempts < 3; attempts++) {
        printf("Inicjalizacja czujnika, próba %d...\r\n", attempts + 1);

        // Reset I2C from sensor library
        VL53L5CX_Reset_Sensor(&dev.platform);

        dev.platform.address = VL53L5CX_DEFAULT_ADDRESS << 1;

        // Check if sensor is alive at default address
        status = vl53l5cx_is_alive(&dev, &is_alive);
        printf("Status czujnika (domyślny adres): %d, is_alive: %d\r\n", status, is_alive);

        if (status == VL53L5CX_STATUS_OK && is_alive) {
            // Change sensor I2C address
            status = vl53l5cx_set_i2c_address(&dev, sensor_address << 1);
            if (status == VL53L5CX_STATUS_OK) {
                dev.platform.address = sensor_address << 1;
                printf("Adres I2C czujnika zmieniony na 0x%02X\r\n", sensor_address);
            } else {
                printf("Błąd ustawienia adresu dla czujnika, kod błędu: %d\r\n", status);
                continue;
            }

            // Check if sensor is alive at new address
            status = vl53l5cx_is_alive(&dev, &is_alive);
            printf("Status czujnika (nowy adres): %d, is_alive: %d\r\n", status, is_alive);
            if (status != VL53L5CX_STATUS_OK || !is_alive) {
                printf("Czujnik nie jest żywy po zmianie adresu, status: %d, is_alive: %d\r\n", status, is_alive);
                continue;
            }

            // Initialize sensor
            status = vl53l5cx_init(&dev);
            if (status == VL53L5CX_STATUS_OK) {
                printf("Czujnik zainicjalizowany z adresem 0x%02X\r\n", sensor_address);
                break; // Initialization successful, break the loop
            } else {
                printf("Błąd inicjalizacji czujnika, kod błędu: %d\r\n", status);
            }
        } else {
            printf("Czujnik nie jest żywy przy domyślnym adresie, status: %d, is_alive: %d\r\n", status, is_alive);
        }

        // Delay before retrying
        HAL_Delay(1000);
    }
}

void Scan_I2C_Devices() {
    printf("Skanowanie urządzeń I2C...\r\n");
    HAL_StatusTypeDef result;
    uint8_t i;
    for (i = 1; i < 128; i++) {
        result = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i << 1), 3, 5);
        if (result == HAL_OK) {
            printf("Urządzenie znalezione pod adresem: 0x%02X\r\n", i);
        }
    }
    printf("Skanowanie zakończone.\r\n");
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
