/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PL9823_H
#define __PL9823_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include "stdint.h"

/* Private macro -------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function macro ---------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
void PL9823_init(void);
void PL9823_resetAllColor(void);
void PL9823_setColor(uint32_t, uint8_t, uint8_t, uint8_t);
void PL9823_startOutput(void);
void PL9823_stopOutput(void);

#ifdef __cplusplus
}
#endif

#endif /* __PL9823_H */

/***************************************************************END OF FILE****/
