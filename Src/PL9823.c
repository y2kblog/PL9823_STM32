/* Includes ------------------------------------------------------------------*/
#include "PL9823.h"
#include "stddef.h"
#include "main.h"
#include "tim.h"
#include "math.h"

/* Imported variables --------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
// User setting parameters
#define PL9823_QTY              2       // Quantity of PL9823

#define PL9823_PWM_RESET_DUMMY  30      // (size_t)(ceil((PL9823_RESET_TIME/(PL9823_T0H+PL9823_T1H))))

/* Private types -------------------------------------------------------------*/
/* Private enum tag ----------------------------------------------------------*/
/* Private struct/union tag --------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint16_t PL9823_data[24 * PL9823_QTY + 2*PL9823_PWM_RESET_DUMMY];

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PL9823_init(void)
{
    PL9823_Delay_ms = ceil((PL9823_T0H+PL9823_T1H) * (float)(sizeof(PL9823_data) / sizeof(PL9823_data[0])) * 1000.0f);

    // Validate value of PL9823_PWM_RESET_DUMMY
    size_t buf = (size_t) (ceil((PL9823_RESET_TIME / (PL9823_T0H + PL9823_T1H))));
    if (PL9823_PWM_RESET_DUMMY != buf)
    {
        printf("Error : must define PL9823_PWM_RESET_DUMMY as %d\r\n", buf);
        Error_Handler();
    }
    PL9823_resetAllColor();

    // LED Off
    PL9823_startOutput();
    HAL_Delay(PL9823_Delay_ms);
}

void PL9823_resetAllColor(void)
{
    for (size_t i = 0; i < sizeof(PL9823_data) / sizeof(PL9823_data[0]); ++i)
    {
        PL9823_data[i] = 0;
    }

    for (uint32_t i = 0; i < PL9823_QTY; ++i)
    {
        PL9823_setColor(i, 0x00, 0x00, 0x00);   // Led Off
    }
}

void PL9823_setColor(uint32_t LED_num, uint8_t R, uint8_t G, uint8_t B)
{
    if(LED_num >= PL9823_QTY)
    {
        printf("LED number must be below %d\r\n", PL9823_QTY);
        Error_Handler();
    }

    static const uint16_t PL9823_PWM_T0H = PL9823_TIM_Clock * PL9823_T0H + 0.5;
    static const uint16_t PL9823_PWM_T1H = PL9823_TIM_Clock * PL9823_T1H + 0.5;

    uint32_t RGB888 = ((uint32_t)R << 16) | ((uint32_t)G << 8) | (uint32_t)B;
    volatile uint16_t *pLedFirstByte = &PL9823_data[(size_t)PL9823_PWM_RESET_DUMMY + 24 * LED_num];
    for(uint32_t i = 0; i < 24; ++i)
    {
        if( (RGB888 >> (23 - i)) & 0x00000001)
            *(pLedFirstByte + i) = PL9823_PWM_T1H;
        else
            *(pLedFirstByte + i) = PL9823_PWM_T0H;
    }
}

void PL9823_startOutput(void)
{
    HAL_StatusTypeDef status;
    status = HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,
            (uint32_t *)PL9823_data, sizeof(PL9823_data) / sizeof(PL9823_data[0]));
    if (status != HAL_OK)
    {
        printf("HAL_TIM_PWM_Start_DMA error : %d\r\n", status);
        //Error_Handler();
    }
}

void PL9823_stopOutput(void)
{
    HAL_StatusTypeDef status;
    status = HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_4);
    if (status != HAL_OK)
    {
        printf("HAL_TIM_PWM_Stop_DMA error : %d\r\n", status);
        //Error_Handler();
    }
}


/* Private functions ---------------------------------------------------------*/

/***************************************************************END OF FILE****/
