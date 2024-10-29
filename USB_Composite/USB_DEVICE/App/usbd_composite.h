/**
  ******************************************************************************
  * @file    usbd_composite_core.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_composite_core.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_composite_CORE_H
#define __USB_composite_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_composite
  * @brief This file is the header file for usbd_composite_core.c
  * @{
  */


/** @defgroup USBD_composite_Exported_Defines
  * @{
  */
 #define USBD_IAD_DESC_SIZE                  0x08
#define USBD_IAD_DESCRIPT                   0x0B
#define composite_EPIN_ADDR                 0x81
#define composite_EPIN_SIZE                 0x10

#define USB_composite_CONFIG_DESC_SIZ       (9U+66U+32U)

#define USBD_CDC_fops_FS USBD_Interface_fops_FS


enum{
    USBD_INTERFACE_CDC_CMD = 0x00,
    USBD_INTERFACE_CDC,
    USBD_INTERFACE_HID,
    USBD_INTERFACE_NUM,
};

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_composite_ClassDriver;
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_composite_CORE_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
