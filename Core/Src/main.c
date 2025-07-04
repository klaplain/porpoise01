/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "wav.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* ADC -----------------------------------------------------------------------*/
#define ADCBUFLEN 32768
#define BUFFER_FULL 1
#define BUFFER_EMPTY 0
#define DATABLOCK_SIZE 4096   // For transferring data to and from raspi
#define MAX_LENGTH_FILENAME 250

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi6;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart3;

MDMA_HandleTypeDef hmdma_mdma_channel0_dma1_stream0_tc_0;
/* USER CODE BEGIN PV */
/* ADC Variables--------------------------------------------------------------*/
uint16_t adc_buf[ADCBUFLEN];
uint16_t adc_lower_status = BUFFER_EMPTY;
uint16_t adc_upper_status = BUFFER_EMPTY;
uint32_t millisecs_to_record=0;
uint32_t end_acq_ms;
uint32_t bytes_written=0;
uint32_t bytes_read=0;
uint32_t total_bytes_written=0;
uint32_t total_blocks_written=0;
uint32_t read_value=0;
uint32_t adcbuf_index=0;

uint16_t max_save_time_ms;
uint16_t start_time_ms,ms_taken;
int ADC_overrun =0; // set as FALSE
int overrun_count;

/* SD Card FatFS Variables-----------------------------------------------------*/
FRESULT result; /* FatFs function common result code */
FILINFO fno;

FATFS *fs;
DWORD fre_clust, fre_sect, tot_sect;

BYTE work[_MAX_SS]; // for formatting SD

char *SD_Directory2;
int directory_lines=6;
char SD_space_description[150];

/* SPI Data transfer arrays ---------------------------------------------------------------*/
uint8_t SPI_input_buffer[DATABLOCK_SIZE];
uint8_t SPI_output_buffer[DATABLOCK_SIZE];

uint16_t SPI6_NCS;

/* WAV File--------------------------------------------------------------------*/
/*
 * Structure of Header and header variable
 */
typedef struct wavfile_header_s
{
	char    ChunkID[4];     /*  4   */
	uint32_t ChunkSize;      /*  4   */
	char    Format[4];      /*  4   */

	char    Subchunk1ID[4]; /*  4   */
	uint32_t Subchunk1Size;  /*  4   */
	uint16_t AudioFormat;    /*  2   */
	uint16_t NumChannels;    /*  2   */
	uint32_t SampleRate;     /*  4   */
	uint32_t ByteRate;       /*  4   */
	uint16_t BlockAlign;     /*  2   */
	uint16_t BitsPerSample;  /*  2   */

	char    Subchunk2ID[4];
	uint32_t Subchunk2Size;
} wavfile_header_t;

/* RTC Variables---------------------------------------------------------------*/
RTC_DateTypeDef gDate;
RTC_TimeTypeDef gTime;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_MDMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_UART4_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI6_Init(void);
/* USER CODE BEGIN PFP */
int send_to_raspi(uint8_t* dataBlock);
int read_from_raspi(uint8_t* dataBlock);
void datetime_request_handler(char* datablock);
void directory_request_handler();
FRESULT get_SD_directory (char* path);
void recording_request_handler();
int write_wav_header(int32_t SampleRate,int32_t FrameCount);
void delete_file_handler();
void transfer_request_handler();
void set_ADC_clock_prescalar(int sampling_fr);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	/* ADC Testing and writing to SD Card*/
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_MDMA_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_SDMMC1_SD_Init();
  MX_UART4_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  MX_SPI6_Init();
  /* USER CODE BEGIN 2 */
  int HAL_Status;
  const char delimiter[2] = ",";
  char *token;

	printf("\r\nProgram Start21\r\n");

	HAL_GPIO_WritePin(GAINB0_GPIO_Port, GAINB0_Pin, GPIO_PIN_SET);  /* Set get to 1 */
	HAL_GPIO_WritePin(GAINB1_GPIO_Port, GAINB1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GAINB2_GPIO_Port, GAINB2_Pin, GPIO_PIN_RESET);

	// Let's check the file system and mount it
	f_mount(0, "", 0);
	HAL_Delay(500);
	if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
	{
		printf("PANIC: Cannot mount SD Card\r\n");
		for(;;){}
	}

	HAL_GPIO_WritePin(Acq_Busy_GPIO_Port, Acq_Busy_Pin, GPIO_PIN_SET);  // Tell the RASPI we are Busy

	printf("\r\nLooping forever\r\n");
	for(;;){
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);

		do{
			HAL_Status = read_from_raspi(SPI_input_buffer);
		}
		while (HAL_Status != HAL_OK);
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		token = strtok((char *)SPI_input_buffer, delimiter);

		printf("%s\r\n",token);
		if(strcmp(">DIR",token) == 0){
			printf("Doing Directory Command\r\n");
		    directory_request_handler();
		}
		else if(strcmp(">TIM",token) == 0){
			printf("Doing Date Time Command\r\n");
			datetime_request_handler(SPI_input_buffer);
		}

		else if(strcmp(">REC",token) == 0){
			printf("Doing Record Command\r\n");
			recording_request_handler();
		}

		else if(strcmp(">FMT",token) == 0){
			printf("Doing Format Command\r\n");
			result= f_mkfs("", FM_ANY, 0, work, sizeof(work));
			directory_lines=6;
		}

		else if(strcmp(">DEL",token) == 0){
			printf("Doing Delete File Command\r\n");
			delete_file_handler();
		}

		else if(strcmp(">XFR",token) == 0){
			printf("Doing Transfer File Command\r\n");
			transfer_request_handler();
		}


		else
			printf("Garbage Command\r\n");

	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.Oversampling.Ratio = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 2;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief SPI6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI6_Init(void)
{

  /* USER CODE BEGIN SPI6_Init 0 */

  /* USER CODE END SPI6_Init 0 */

  /* USER CODE BEGIN SPI6_Init 1 */

  /* USER CODE END SPI6_Init 1 */
  /* SPI6 parameter configuration*/
  hspi6.Instance = SPI6;
  hspi6.Init.Mode = SPI_MODE_SLAVE;
  hspi6.Init.Direction = SPI_DIRECTION_2LINES;
  hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi6.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi6.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi6.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi6.Init.CRCPolynomial = 0x0;
  hspi6.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  hspi6.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi6.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi6.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi6.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi6.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi6.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi6.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi6.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi6.Init.IOSwap = SPI_IO_SWAP_ENABLE;
  if (HAL_SPI_Init(&hspi6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI6_Init 2 */

  /* USER CODE END SPI6_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 576000;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}

/**
  * Enable MDMA controller clock
  * Configure MDMA for global transfers
  *   hmdma_mdma_channel0_dma1_stream0_tc_0
  */
static void MX_MDMA_Init(void)
{

  /* MDMA controller clock enable */
  __HAL_RCC_MDMA_CLK_ENABLE();
  /* Local variables */

  /* Configure MDMA channel MDMA_Channel0 */
  /* Configure MDMA request hmdma_mdma_channel0_dma1_stream0_tc_0 on MDMA_Channel0 */
  hmdma_mdma_channel0_dma1_stream0_tc_0.Instance = MDMA_Channel0;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.Request = MDMA_REQUEST_DMA1_Stream0_TC;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.Priority = MDMA_PRIORITY_LOW;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.SourceInc = MDMA_SRC_INC_BYTE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.DestinationInc = MDMA_DEST_INC_BYTE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.BufferTransferLength = 1;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.SourceBlockAddressOffset = 0;
  hmdma_mdma_channel0_dma1_stream0_tc_0.Init.DestBlockAddressOffset = 0;
  if (HAL_MDMA_Init(&hmdma_mdma_channel0_dma1_stream0_tc_0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure post request address and data masks */
  if (HAL_MDMA_ConfigPostRequestMask(&hmdma_mdma_channel0_dma1_stream0_tc_0, 0, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /* MDMA interrupt initialization */
  /* MDMA_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RecordEnable_Pin|Acq_Busy_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GAINA0_Pin|GAINA1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GAINB0_Pin|GAINB1_Pin|GAINB2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RED_LED_Pin|Orange_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SD_Card_Detect_Pin */
  GPIO_InitStruct.Pin = SD_Card_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(SD_Card_Detect_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RecordEnable_Pin Acq_Busy_Pin */
  GPIO_InitStruct.Pin = RecordEnable_Pin|Acq_Busy_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : GAINA0_Pin GAINA1_Pin */
  GPIO_InitStruct.Pin = GAINA0_Pin|GAINA1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : GAINB0_Pin GAINB1_Pin GAINB2_Pin */
  GPIO_InitStruct.Pin = GAINB0_Pin|GAINB1_Pin|GAINB2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : RED_LED_Pin */
  GPIO_InitStruct.Pin = RED_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RED_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Orange_LED_Pin */
  GPIO_InitStruct.Pin = Orange_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Orange_LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len)
{
	(void)file;
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		HAL_UART_Transmit(&huart4,(uint8_t*)ptr++,1,1);// Sending in normal mode
	}
	return len;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	adc_lower_status = BUFFER_FULL;
	if(adc_upper_status == BUFFER_FULL)  // Overflow detect
	{
		HAL_GPIO_WritePin(GPIOB, Orange_LED_Pin, GPIO_PIN_SET);
		ADC_overrun=1; //Set overrun flag
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	adc_upper_status = BUFFER_FULL;
	if(adc_lower_status == BUFFER_FULL)  // Overflow detect
	{
		HAL_GPIO_WritePin(GPIOB, Orange_LED_Pin, GPIO_PIN_SET);
		ADC_overrun=1; //Set overrun flag
	}
	//
}

int send_to_raspi(uint8_t* dataBlock)
{
	HAL_GPIO_WritePin(Acq_Busy_GPIO_Port, Acq_Busy_Pin, GPIO_PIN_RESET);  // Tell RASPI we are Ready
	int HAL_Status = HAL_SPI_Transmit(&hspi6, dataBlock, DATABLOCK_SIZE, 100);  // send DATABLOCK_SIZE bytes to the RASPI
	HAL_GPIO_WritePin(Acq_Busy_GPIO_Port, Acq_Busy_Pin, GPIO_PIN_SET);  // Tell the RASPI we are Busy
	return HAL_Status;
	}

int read_from_raspi(uint8_t* dataBlock)
{
	HAL_GPIO_WritePin(Acq_Busy_GPIO_Port, Acq_Busy_Pin, GPIO_PIN_RESET);  // Tell RASPI we are Ready
	int HAL_Status = HAL_SPI_Receive(&hspi6, dataBlock, DATABLOCK_SIZE, 1000);  // get DATABLOCK_SIZE bytes from the RASPI
	HAL_GPIO_WritePin(Acq_Busy_GPIO_Port, Acq_Busy_Pin, GPIO_PIN_SET);  // Tell the RASPI we are Busy
	return HAL_Status;
}

void directory_request_handler()
{
	char full_path_name[MAX_LENGTH_FILENAME];

	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);				/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);				/* Display time Format: hh:mm:ss */
	printf("RTC Date Time  %02d:%02d:%02d  %02d-%02d-%2d\r\n",gTime.Hours, gTime.Minutes, gTime.Seconds,gDate.Month,gDate.Date,2000 + gDate.Year);				/* Display date Format: dd-mm-yy */

	/* allocate memory for directory listing */
	SD_Directory2 = malloc( 100 * directory_lines++ );

	if( SD_Directory2 == NULL ) {
		printf("PANIC: unable to allocate required memory\r\n");
	}
	/* Get volume information and free clusters of drive 1 */
	result = f_getfree("0:", &fre_clust, &fs);
	if (result!= FR_OK){
		printf("PANIC: unable to determine free space on SD\r\n");
	}
	/* Get total sectors and free sectors */
	tot_sect = (fs->n_fatent - 2) * fs->csize;
	fre_sect = fre_clust * fs->csize;
	/* Print free space in unit of KB (assuming 512 bytes/sector) */
	sprintf(SD_space_description,"%lu KB total drive space.     %lu KB available.\r\n",tot_sect / 2, fre_sect / 2);
	strcpy( SD_Directory2, SD_space_description);

	/* Get directory contents */
	strcpy(full_path_name, "/");

	result= get_SD_directory(full_path_name);
	//printf("%s", SD_Directory2);
	strcat( SD_Directory2, "\f");

	/* Send directory content to raspi */
	int buffer_index=0;
	do{
		SPI_output_buffer[buffer_index]= (uint8_t)SD_Directory2[buffer_index];
	}
	while(SD_Directory2[buffer_index++] != 12);
	send_to_raspi(SPI_output_buffer);

	printf("Directory Uploaded\r\n");
	free(SD_Directory2);
	directory_lines=5;

}

FRESULT get_SD_directory (char* path)        /* Start node to be scanned (***also used as work area***) */
{
	FRESULT res;
	DIR dir;
	UINT i;
	static FILINFO fno;
	char this_file[MAX_LENGTH_FILENAME];

	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
			if (fno.fattrib & AM_DIR) {                    /* It is a directory */
				i = strlen(path);
				sprintf(&path[i], "/%s", fno.fname);
				res = get_SD_directory(path);                    /* Enter the directory */
				if (res != FR_OK) break;
				path[i] = 0;
			} else {                                       /* It is a file. */
				sprintf(&this_file[0],"%s/%s\t%ld\t%u-%02u-%02u\t%02u:%02u\r\n", path, fno.fname,fno.fsize,(fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
				SD_Directory2 = realloc(SD_Directory2, 100 * directory_lines++ );
				if( SD_Directory2 == NULL ) {
					printf("PANIC: unable to allocate required memory\r\n");
				} else {
					strcat( SD_Directory2, this_file);
				}
			}
		}
		f_closedir(&dir);
	}
	return res;
}
void datetime_request_handler(char* datablock ){
	const char delimiter[2] = ",";

	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	sTime.Hours = (uint8_t)atoi(strtok(NULL, delimiter)); // set hours
	sTime.Minutes = (uint8_t)atoi(strtok(NULL, delimiter)); // set minutes
	sTime.Seconds = (uint8_t)atoi(strtok(NULL, delimiter)); // set seconds
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		printf("FAIL: Problem setting time\r\n");
	}
	sDate.WeekDay = (uint8_t)atoi(strtok(NULL, delimiter));
	sDate.Month = (uint8_t)atoi(strtok(NULL, delimiter));
	sDate.Date = (uint8_t)atoi(strtok(NULL, delimiter)); // date
	sDate.Year = (uint8_t)atoi(strtok(NULL, delimiter)); // year
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		printf("FAIL: Problem setting date\r\n");
	}
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2); // backup register
	printf("DateTime Set Complete\r\n");
}



void transfer_request_handler(){
	const char delimiter[2] = ",";
	char *file_to_be_transferred;
	file_to_be_transferred = strtok(NULL, delimiter);

	printf("opening file %s to transfer\r\n", file_to_be_transferred);
	if(f_open(&SDFile, file_to_be_transferred, FA_READ) != FR_OK)  				//Open file for reading and uploading
	{
		printf("FAIL: Cannot open file for reading uploading\r\n");
	}
	else
	{
		while(!f_eof(&SDFile)){
			result = f_read(&SDFile, SPI_output_buffer,DATABLOCK_SIZE, (void *)&bytes_read);
			if((bytes_read == 0) || (result != FR_OK))
			{
				printf("FAIL: Cannot read file to save upload\r\n");
			}
			else
			{
				send_to_raspi(SPI_output_buffer);
			}
		}
	    printf("%s Uploaded\r\n",file_to_be_transferred);
	}
	f_close(&SDFile);
}

void delete_file_handler(){
	const char delimiter[2] = ",";
	char *file_to_be_deleted;
	file_to_be_deleted = strtok(NULL, delimiter);

	printf("deleting file %s\r\n", file_to_be_deleted);
	if(f_unlink(file_to_be_deleted) != FR_OK)  				//delete file
	{
		printf("FAIL: Cannot delete file\r\n");
	}
	else{
		printf("%s Deleted\r\n",file_to_be_deleted);
	}
}


void recording_request_handler(){
	char fullfilename[MAX_LENGTH_FILENAME];

	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);				/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);				/* Display time Format: hh:mm:ss */
	printf("RTC Date Time  %02d:%02d:%02d  %02d-%02d-%2d\r\n",gTime.Hours, gTime.Minutes, gTime.Seconds,gDate.Month,gDate.Date,2000 + gDate.Year);				/* Display date Format: dd-mm-yy */


	const char delimiter[2] = ",";
	char *file_to_be_recorded;
	int sampling_frequency_kHz = atoi(strtok(NULL, delimiter));
	int gain = atoi(strtok(NULL, delimiter));
	uint32_t millisecs_to_record = atoi(strtok(NULL, delimiter))*1000;
	file_to_be_recorded = strtok(NULL, delimiter);

	//HAL_GPIO_WritePin(GPIOC, GAINA0_Pin, gain & 0x01);      // Not setting gain for transducer preamp at this time
	//HAL_GPIO_WritePin(GPIOC, GAINA1_Pin, gain>>1 & 0x01);

	HAL_GPIO_WritePin(GPIOC, GAINB0_Pin, gain & 0x01);  // Set gain for onboard preamp
	HAL_GPIO_WritePin(GPIOC, GAINB1_Pin, gain>>1 & 0x01);
	HAL_GPIO_WritePin(GPIOC, GAINB2_Pin, gain>>2 & 0x01);

	HAL_GPIO_WritePin(GPIOB, Orange_LED_Pin, GPIO_PIN_RESET);  // Clear overrun LED

	printf("Record: Sampling %3dkHz  Gain %1d   Duration %lumS  %s \r\n", sampling_frequency_kHz, gain, millisecs_to_record,file_to_be_recorded);

	if(f_open(&SDFile, file_to_be_recorded, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  //Open file for writing (Create)
	{
		printf("FAIL: Cannot open file for recording\r\n");
	}
	else
	{
		write_wav_header(sampling_frequency_kHz*1000,sampling_frequency_kHz*millisecs_to_record); 							// Write WAV Header Sampling at 786000 Hz
		for(adcbuf_index=0;adcbuf_index < ADCBUFLEN;adcbuf_index++) // Clear ADC Buffer
		{
			adc_buf[adcbuf_index]=0;
		}
		total_blocks_written=0;
		total_bytes_written=0;
		adc_lower_status = BUFFER_EMPTY;
		adc_upper_status = BUFFER_EMPTY;
		overrun_count=0;
		max_save_time_ms=0;
		//HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
		set_ADC_clock_prescalar(sampling_frequency_kHz);
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADCBUFLEN); 	// Enable ADC DMA and add contents
		end_acq_ms=uwTick+millisecs_to_record;
		do{
			if(adc_lower_status == BUFFER_FULL)
			{
				HAL_GPIO_WritePin(GPIOE, RecordEnable_Pin, GPIO_PIN_RESET);
				start_time_ms=uwTick;
				f_write(&SDFile, &adc_buf[0], ADCBUFLEN, (void *)&bytes_written);
				ms_taken=uwTick-start_time_ms;
				if(ms_taken > max_save_time_ms){
					max_save_time_ms = ms_taken;
				}
				total_bytes_written = total_bytes_written+bytes_written;
				adc_lower_status = BUFFER_EMPTY;
				total_blocks_written++;
				if(ADC_overrun){
					HAL_ADC_Stop_DMA(&hadc1);
					f_write(&SDFile, &adc_buf[ADCBUFLEN/2], ADCBUFLEN, (void *)&bytes_written);
					total_bytes_written = total_bytes_written+bytes_written;
					adc_upper_status = BUFFER_EMPTY;
					total_blocks_written++;
					ADC_overrun=0;
					overrun_count++;
					HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADCBUFLEN); 	// Enable ADC DMA and add contents
				}
			}
			if(adc_upper_status == BUFFER_FULL)
			{
				HAL_GPIO_WritePin(GPIOE, RecordEnable_Pin, GPIO_PIN_SET);
				start_time_ms=uwTick;
				f_write(&SDFile, &adc_buf[ADCBUFLEN/2], ADCBUFLEN, (void *)&bytes_written);
				ms_taken=uwTick-start_time_ms;
				if(ms_taken > max_save_time_ms){
					max_save_time_ms = ms_taken;
				}
				total_bytes_written = total_bytes_written+bytes_written;
				adc_upper_status = BUFFER_EMPTY;
				total_blocks_written++;
				if(ADC_overrun){
					HAL_ADC_Stop_DMA(&hadc1);
					f_write(&SDFile, &adc_buf[0], ADCBUFLEN, (void *)&bytes_written);
					total_bytes_written = total_bytes_written+bytes_written;
					adc_lower_status = BUFFER_EMPTY;
					total_blocks_written++;
					ADC_overrun=0;
					overrun_count++;
					HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADCBUFLEN); 	// Enable ADC DMA and add contents
				}
			}
		}
		while(uwTick< end_acq_ms);
		HAL_ADC_Stop_DMA(&hadc1);   								// Halt ADC DMA

		// Now update WAV file with data length
		f_lseek(&SDFile,0x28);
		f_write(&SDFile,&total_bytes_written, 4, (void *)&bytes_written);

		f_close(&SDFile);  											// Writing complete so close file
		printf("Total Blocks Written %lu  Total Bytes Written %lu MaxSaveTime %d  Overruns %d\r\n",total_blocks_written,total_bytes_written,max_save_time_ms,overrun_count);
		printf("Recording Complete\r\n");

		strcpy(fullfilename,"//");
		strcat(fullfilename,file_to_be_recorded);
		HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);				/* Get the RTC current Date */
		HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);				/* Display time Format: hh:mm:ss */
		fno.fdate = (WORD)(((gDate.Year+20) * 512U) | gDate.Month * 32U | gDate.Date);
		fno.ftime = (WORD)(gTime.Hours * 2048U | gTime.Minutes * 32U | gTime.Seconds / 2U);
		result=f_utime(fullfilename, &fno);
	}
}

/*
 * Function to write header of WAV file
 */
/*Return 0 on success and -1 on failure*/
int write_wav_header(int32_t SampleRate,int32_t FrameCount)
{
	int ret=0;

	struct wavfile_header_s wav_header;
	int32_t subchunk2_size;
	int32_t chunk_size;

	//    size_t write_count;

	uint32_t bytes_written=0;

	FRESULT result; /* FatFs function common result code */

	subchunk2_size  = FrameCount * NUM_CHANNELS * BITS_PER_SAMPLE / 8;
	chunk_size      = 4 + (8 + SUBCHUNK1SIZE) + (8 + subchunk2_size);

	wav_header.ChunkID[0] = 'R';
	wav_header.ChunkID[1] = 'I';
	wav_header.ChunkID[2] = 'F';
	wav_header.ChunkID[3] = 'F';

	wav_header.ChunkSize = chunk_size;

	wav_header.Format[0] = 'W';
	wav_header.Format[1] = 'A';
	wav_header.Format[2] = 'V';
	wav_header.Format[3] = 'E';

	wav_header.Subchunk1ID[0] = 'f';
	wav_header.Subchunk1ID[1] = 'm';
	wav_header.Subchunk1ID[2] = 't';
	wav_header.Subchunk1ID[3] = ' ';

	wav_header.Subchunk1Size = SUBCHUNK1SIZE;
	wav_header.AudioFormat = AUDIO_FORMAT;
	wav_header.NumChannels = NUM_CHANNELS;
	wav_header.SampleRate = SampleRate;
	wav_header.ByteRate = SampleRate <<1;
	wav_header.BlockAlign = BLOCK_ALIGN;
	wav_header.BitsPerSample = BITS_PER_SAMPLE;

	wav_header.Subchunk2ID[0] = 'd';
	wav_header.Subchunk2ID[1] = 'a';
	wav_header.Subchunk2ID[2] = 't';
	wav_header.Subchunk2ID[3] = 'a';
	wav_header.Subchunk2Size = subchunk2_size;

	printf("chunk_size %ld %lx\r\n", chunk_size,chunk_size);
	printf("subchunk2_size %ld %lx\r\n", subchunk2_size,subchunk2_size);
	//Open file for writing (Create)
	result = f_write(&SDFile, &wav_header, sizeof(wavfile_header_t), (void *)&bytes_written);
	if((bytes_written == 0) || (result != FR_OK))
	{
		Error_Handler();
	}

	// Flush the file buffers
	result = f_sync(&SDFile);
	if(result != FR_OK)
	{
		Error_Handler();
	}
	return ret;
}

void set_ADC_clock_prescalar(int sampling_fr){
	ADC_MultiModeTypeDef multimode = {0};
	switch(sampling_fr){
	case 800:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
		break;
	case 533:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6;
		break;
	case 400:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;
		break;
	case 320:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV10;
		break;
	case 266:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV12;
		break;
	case 200:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;
		break;
	case 100:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV32;
		break;
	case 50:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV64;
		break;
	case 25:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV128;
		break;
	case 12:
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV256;
		break;
	}
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}

}



/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
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
