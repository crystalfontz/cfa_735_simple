/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : flash_if.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : specific media access Layer for internal flash
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "dfu_mal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#ifdef STM32L1XX_MD
  uint32_t tHalfPage1[128/4];
  uint32_t tHalfPage2[128/4];
#endif /* STM32L1XX_MD */  

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : FLASH_If_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t FLASH_If_Init(void)
{
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : FLASH_If_Erase
* Description    : Erase sector
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t FLASH_If_Erase(uint32_t SectorAddress)
{
#ifdef STM32L1XX_MD
  FLASH_ClearFlag(FLASH_FLAG_PGAERR | FLASH_FLAG_OPTVERR);
  FLASH_ErasePage(SectorAddress);
#else
  FLASH_ErasePage(SectorAddress);
#endif /* STM32L1XX_MD */
  
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : FLASH_If_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t FLASH_If_Write(uint32_t SectorAddress, uint32_t DataLength)
{
  uint32_t idx = 0;
#ifdef STM32L1XX_MD
  __IO uint32_t* malPointer = (uint32_t *)MAL_Buffer;
  __IO uint32_t* memPointer = (uint32_t *)SectorAddress;
  __IO uint32_t memBuffer[32]; /* Temporary buffer holding data that will be written in a half-page space */
  __IO uint32_t* mempBuffer = memBuffer;  
  __IO uint32_t* tmp;
#endif /* STM32L1XX_MD */      
  
  if  (DataLength & 0x3) /* Not an aligned data */
  {
    for (idx = DataLength; idx < ((DataLength & 0xFFFC) + 4); idx++)
    {
      MAL_Buffer[idx] = 0xFF;
    }
  } 
  
#ifdef STM32L1XX_MD  
  /* Reinitialize the intermediate buffer pointer */
  mempBuffer = memBuffer;
  
  /* If the address is not aligned to half-page fill the first location with existing data */
  if (((uint32_t)memPointer & 0x7F) != 0)
  {
    /* get the aligned address */
    tmp = (uint32_t *)((uint32_t)memPointer & 0xFFFFFF80);
    
    /* Read the first part from the memory */
    while (tmp < memPointer)
    {
      *(uint32_t *)(mempBuffer++) = *(uint32_t *)(tmp++);
    }
  }    
  
  while (malPointer < (uint32_t*)(MAL_Buffer + DataLength))
  {    
    /* Fill with the received buffer */
    while (mempBuffer < (memBuffer + 32))
    {
      /* If there are still data available in the received buffer */
      if (malPointer < ((uint32_t *)MAL_Buffer + DataLength))
      {
        *(uint32_t *)(mempBuffer++) = *(uint32_t *)(malPointer++);
      }
      else /* no more data available in the received buffer: fill remaining with dummy 0 */
      {
        *(uint32_t *)(mempBuffer++) = 0;
      }
    }
   
    /* Write the buffer to the memory*/    
    FLASH_ProgramHalfPage(((uint32_t)memPointer & 0xFFFFFF80), (uint32_t *)(memBuffer));    
    
    /* Increment the memory pointer */ 
    memPointer = (uint32_t *)(((uint32_t)memPointer & 0xFFFFFF80) + (32*4));
    
    /* Reinitialize the intermediate buffer pointer */
    mempBuffer = memBuffer;
  }
  
#else
  
  /* Data received are Word multiple */    
  for (idx = 0; idx <  DataLength; idx = idx + 4)
  {
    FLASH_ProgramWord(SectorAddress, *(uint32_t *)(MAL_Buffer + idx));  
    SectorAddress += 4;
  } 
#endif /* STM32L1XX_MD */
 
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : FLASH_If_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : buffer address pointer
*******************************************************************************/
uint8_t *FLASH_If_Read (uint32_t SectorAddress, uint32_t DataLength)
{
  return  (uint8_t*)(SectorAddress);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
