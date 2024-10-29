/**
  ******************************************************************************
  * @file    usbd_composite.c
  * @author  MCD Application Team
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                composite Class  Description
  *          ===================================================================
  *
  *
  *
  *
  *
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  *
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_composite.h"
#include "usbd_ctlreq.h"
#include "stdio.h"
#include "usbd_cdc_if.h"
#include "usbd_custom_hid_if.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_composite
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_composite_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_composite_Private_Defines
  * @{
  */
void *pClassDataCDC = NULL;
void *pClassDataHID = NULL;
/**
  * @}
  */


/** @defgroup USBD_composite_Private_Macros
  * @{
  */

/**
  * @}
  */




/** @defgroup USBD_composite_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_composite_Init(USBD_HandleTypeDef *pdev,
                                    uint8_t cfgidx);

static uint8_t  USBD_composite_DeInit(USBD_HandleTypeDef *pdev,
                                      uint8_t cfgidx);

static uint8_t  USBD_composite_Setup(USBD_HandleTypeDef *pdev,
                                     USBD_SetupReqTypedef *req);

static uint8_t  *USBD_composite_GetCfgDesc(uint16_t *length);

static uint8_t  *USBD_composite_GetDeviceQualifierDesc(uint16_t *length);

static uint8_t  USBD_composite_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_composite_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_composite_EP0_RxReady(USBD_HandleTypeDef *pdev);

static uint8_t  USBD_composite_EP0_TxReady(USBD_HandleTypeDef *pdev);

static uint8_t  USBD_composite_SOF(USBD_HandleTypeDef *pdev);

static uint8_t  USBD_composite_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_composite_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);

/**
  * @}
  */

/** @defgroup USBD_composite_Private_Variables
  * @{
  */

USBD_ClassTypeDef  USBD_composite_ClassDriver =
{
    USBD_composite_Init,
    USBD_composite_DeInit,
    USBD_composite_Setup,
    USBD_composite_EP0_TxReady,
    USBD_composite_EP0_RxReady,
    USBD_composite_DataIn,
    USBD_composite_DataOut,
    USBD_composite_SOF,
    USBD_composite_IsoINIncomplete,
    USBD_composite_IsoOutIncomplete,
    USBD_composite_GetCfgDesc,
    USBD_composite_GetCfgDesc,
    USBD_composite_GetCfgDesc,
    USBD_composite_GetDeviceQualifierDesc,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB composite device Configuration Descriptor */
static uint8_t USBD_composite_CfgDesc[USB_composite_CONFIG_DESC_SIZ] =
{
    /*配置描述*/
    0x09, /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    USB_composite_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00,
    USBD_INTERFACE_NUM,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
    0xC0,         /*bmAttributes: bus powered */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
    /*CDC设备描述(66)*/
#if 1
    /*IAD描述*/
#if 1
    /*Interface Association Descriptor*/
    USBD_IAD_DESC_SIZE,     //blength IAP描述符大小
    USBD_IAD_DESCRIPT,      //bDescriptorType IAP描述符种类
    0x00,                     //bFirstInterface 接口描述符是在总的配置描述符中的第几个从0开始数
    0x02,                     //bInterface 接口描述符数量
    0x02,                     //bFunctionClass 设备符中的bDeviceClass
    0x02,                     //bFunctionSubClass 设备符中的bDeviceSubClass
    0x01,                     //bInterfaceProtocol 设备符中的bDeviceProtocol
    0x00,
#endif
    /*---------------------------------------------------------------------------*/
    /*Interface Descriptor */
    0x09,   /* bLength: Interface Descriptor size */
    USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    USBD_INTERFACE_CDC_CMD,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x01,   /* bNumEndpoints: One endpoints used */
    0x02,   /* bInterfaceClass: Communication Interface Class */
    0x02,   /* bInterfaceSubClass: Abstract Control Model */
    0x01,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */

    /*Header Functional Descriptor*/
    0x05,   /* bLength: Endpoint Descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: Header Func Desc */
    0x10,   /* bcdCDC: spec release number */
    0x01,

    /*Call Management Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0+D1 */
    0x01,   /* bDataInterface: 1 */

    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */

    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x00,   /* bMasterInterface: Communication class interface */
    0x01,   /* bSlaveInterface0: Data Class Interface */

    /*Endpoint 2 Descriptor*/
    0x07,                           /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
    CDC_CMD_EP,                     /* bEndpointAddress */
    0x03,                           /* bmAttributes: Interrupt */
    LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
    HIBYTE(CDC_CMD_PACKET_SIZE),
    CDC_FS_BINTERVAL,                           /* bInterval: */
    /*---------------------------------------------------------------------------*/

    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
    USBD_INTERFACE_CDC,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */

    /*Endpoint OUT Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
    CDC_OUT_EP,                        /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
    CDC_IN_EP,                         /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
    0x00,                               /* bInterval: ignore for Bulk transfer */
#endif

    /*HID设备描述(32)*/
#if 1
    /************** Descriptor of CUSTOM HID interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
    USBD_INTERFACE_HID,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: CUSTOM_HID*/
    0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of CUSTOM_HID *************************/
    /* 18 */
    0x09,         /*bLength: CUSTOM_HID Descriptor size*/
    CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: CUSTOM_HID*/
    0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Custom HID endpoints ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

    CUSTOM_HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    CUSTOM_HID_EPIN_SIZE, /*wMaxPacketSize: 2 Byte max */
    0x00,
    CUSTOM_HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
    /* 34 */

    0x07,          /* bLength: Endpoint Descriptor size */
    USB_DESC_TYPE_ENDPOINT, /* bDescriptorType: */
    CUSTOM_HID_EPOUT_ADDR,  /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03, /* bmAttributes: Interrupt endpoint */
    CUSTOM_HID_EPOUT_SIZE,  /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    CUSTOM_HID_FS_BINTERVAL,  /* bInterval: Polling Interval */
    /* 41 */
#endif
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB Standard Device Descriptor */
static uint8_t USBD_composite_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

/**
  * @}
  */

/** @defgroup USBD_composite_Private_Functions
  * @{
  */

/**
  * @brief  USBD_composite_Init
  *         Initialize the composite interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_composite_Init(USBD_HandleTypeDef *pdev,
                                    uint8_t cfgidx)
{
    uint8_t res = 0;
    /*CDC*/
    pdev->pUserData = (void *)&USBD_CDC_fops_FS;
    res += USBD_CDC.Init(pdev,cfgidx);
    pClassDataCDC = pdev->pClassData;
    /*HID*/
    pdev->pUserData = (void *)&USBD_CustomHID_fops_FS;
    res += USBD_CUSTOM_HID.Init(pdev,cfgidx);
    pClassDataHID = pdev->pClassData;
    return res;
}

/**
  * @brief  USBD_composite_Init
  *         DeInitialize the composite layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_composite_DeInit(USBD_HandleTypeDef *pdev,
                                      uint8_t cfgidx)
{
    uint8_t res = 0;
    /*CDC*/
    pdev->pClassData = pClassDataCDC;
    pdev->pUserData = (void *)&USBD_CDC_fops_FS;
    res +=  USBD_CDC.DeInit(pdev,cfgidx);
    /*HID*/
    pdev->pClassData = pClassDataHID;
    pdev->pUserData = (void *)&USBD_CustomHID_fops_FS;
    res +=  USBD_CUSTOM_HID.DeInit(pdev,cfgidx);

    return res;
}

/**
  * @brief  USBD_composite_Setup
  *         Handle the composite specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_composite_Setup(USBD_HandleTypeDef *pdev,
                                     USBD_SetupReqTypedef *req)
{
    USBD_StatusTypeDef ret = USBD_OK;

#if 0
    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    case USB_REQ_TYPE_CLASS :
        switch (req->bRequest)
        {
        default:
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;
        }
        break;

    case USB_REQ_TYPE_STANDARD:
        switch (req->bRequest)
        {
        default:
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;
        }
        break;

    default:
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
        break;
    }
#endif






    switch(req->bmRequest & USB_REQ_RECIPIENT_MASK)
    {
    case USB_REQ_RECIPIENT_INTERFACE:
        switch(req->wIndex)
        {
        case USBD_INTERFACE_CDC:
        case USBD_INTERFACE_CDC_CMD:
            pdev->pClassData = pClassDataCDC;
            pdev->pUserData = &USBD_CDC_fops_FS;
            return (USBD_CDC.Setup(pdev,req));
        case USBD_INTERFACE_HID:
            pdev->pClassData = pClassDataHID;
            pdev->pUserData = &USBD_CustomHID_fops_FS;
            return (USBD_CUSTOM_HID.Setup(pdev,req));
        default:
            printf("[%s] %d:",__func__,__LINE__);
            break;
        }
        break;
    case USB_REQ_RECIPIENT_ENDPOINT:
        switch(req->wIndex)
        {
        case CDC_IN_EP:
        case CDC_OUT_EP:
        case CDC_CMD_EP:
            pdev->pClassData = pClassDataCDC;
            pdev->pUserData = &USBD_CDC_fops_FS;
            return (USBD_CDC.Setup(pdev,req));
        case CUSTOM_HID_EPIN_ADDR:
        case CUSTOM_HID_EPOUT_ADDR:
            pdev->pClassData = pClassDataHID;
            pdev->pUserData = &USBD_CustomHID_fops_FS;
            return (USBD_CUSTOM_HID.Setup(pdev,req));
        default:
            printf("[%s] %d:",__func__,__LINE__);

            break;
        }
        break;
    default:
        printf("[%s] %d:",__func__,__LINE__);
        break;
    }
    return ret;
}


/**
  * @brief  USBD_composite_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_composite_GetCfgDesc(uint16_t *length)
{
    *length = sizeof(USBD_composite_CfgDesc);
    return USBD_composite_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_composite_DeviceQualifierDescriptor(uint16_t *length)
{
    *length = sizeof(USBD_composite_DeviceQualifierDesc);
    return USBD_composite_DeviceQualifierDesc;
}


/**
  * @brief  USBD_composite_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_composite_DataIn(USBD_HandleTypeDef *pdev,
                                      uint8_t epnum)
{
    switch(epnum)
    {
    case (CDC_IN_EP & 0x0f):
        pdev->pClassData = pClassDataCDC;
        pdev->pUserData = &USBD_CDC_fops_FS;
        return(USBD_CDC.DataIn(pdev,epnum));
    case (CUSTOM_HID_EPIN_ADDR & 0x0f):
        pdev->pClassData = pClassDataHID;
        pdev->pUserData = &USBD_CustomHID_fops_FS;
        return(USBD_CUSTOM_HID.DataIn(pdev,epnum));
    default:
        break;
    }
    return USBD_OK;
}

/**
  * @brief  USBD_composite_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_composite_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
    pdev->pClassData = pClassDataCDC;
    pdev->pUserData = &USBD_CDC_fops_FS;

    return USBD_CDC.EP0_RxReady(pdev);
}
/**
  * @brief  USBD_composite_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_composite_EP0_TxReady(USBD_HandleTypeDef *pdev)
{

    return USBD_OK;
}
/**
  * @brief  USBD_composite_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_composite_SOF(USBD_HandleTypeDef *pdev)
{

    return USBD_OK;
}
/**
  * @brief  USBD_composite_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_composite_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

    return USBD_OK;
}
/**
  * @brief  USBD_composite_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_composite_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

    return USBD_OK;
}
/**
  * @brief  USBD_composite_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_composite_DataOut(USBD_HandleTypeDef *pdev,
                                       uint8_t epnum)
{
    switch(epnum)
    {
    case (CDC_OUT_EP):
    case (CDC_CMD_EP):
        pdev->pClassData = pClassDataCDC;
        pdev->pUserData = &USBD_CDC_fops_FS;
        return(USBD_CDC.DataOut(pdev,epnum));
    case (CUSTOM_HID_EPOUT_ADDR):
        pdev->pClassData = pClassDataHID;
        pdev->pUserData = &USBD_CustomHID_fops_FS;
        return(USBD_CUSTOM_HID.DataOut(pdev,epnum));
    default:
        printf("ERROR[%s] %d:",__func__,__LINE__);
        break;
    }
    return USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_composite_GetDeviceQualifierDesc(uint16_t *length)
{
    *length = sizeof(USBD_composite_DeviceQualifierDesc);
    return USBD_composite_DeviceQualifierDesc;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
