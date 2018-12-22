/* Copyright (c) 2018 Vasily Voropaev <vvg@cubitel.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "usb.h"
#include "usb_class.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usb_ecm.h"
#include "usb_audio.h"


/*********************************************
   CDC Device library callbacks
 *********************************************/
static uint8_t  usbd_cdc_Init        (void  *pdev, uint8_t cfgidx);
static uint8_t  usbd_cdc_DeInit      (void  *pdev, uint8_t cfgidx);
static uint8_t  usbd_cdc_Setup       (void  *pdev, USB_SETUP_REQ *req);
static uint8_t  usbd_cdc_EP0_RxReady  (void *pdev);
static uint8_t  usbd_cdc_DataIn      (void *pdev, uint8_t epnum);
static uint8_t  usbd_cdc_DataOut     (void *pdev, uint8_t epnum);
static uint8_t  usbd_cdc_SOF         (void *pdev);
static uint8_t  usbd_cdc_IsoOutIncomplete (void *pdev);

/*********************************************
   CDC specific management functions
 *********************************************/
static uint8_t  *USBD_cdc_GetCfgDesc (uint8_t speed, uint16_t *length);
static uint8_t  *USBD_cdc_GetUsrStr ( uint8_t speed ,uint8_t index,  uint16_t *length);


__ALIGN_BEGIN uint8_t usbd_cdc_CfgDesc  [USB_CDC_CONFIG_DESC_SIZ] __ALIGN_END ;

__ALIGN_BEGIN uint8_t usbd_cdc_OtherCfgDesc  [USB_CDC_CONFIG_DESC_SIZ] __ALIGN_END ;

__ALIGN_BEGIN static __IO uint32_t  usbd_cdc_AltSet  __ALIGN_END = 0;

__ALIGN_BEGIN uint8_t USB_Rx_Buffer   [USB_MAX_PACKET_SIZE] __ALIGN_END ;

__ALIGN_BEGIN uint8_t USB_Tx_Buffer   [USB_MAX_PACKET_SIZE] __ALIGN_END ;

__ALIGN_BEGIN uint8_t CmdBuff[ETH_CMD_BUFFER_SIZE] __ALIGN_END ;


uint8_t  USB_Tx_State = 0;

static uint32_t cdcCmd = 0xFF;
static uint32_t cdcType;
static uint32_t cdcIf;
static uint32_t cdcLen;

/* CDC interface class callbacks structure */
USBD_Class_cb_TypeDef USBD_CDC_cb = {
	usbd_cdc_Init,
	usbd_cdc_DeInit,
	usbd_cdc_Setup,
	NULL,                 /* EP0_TxSent, */
	usbd_cdc_EP0_RxReady,
	usbd_cdc_DataIn,
	usbd_cdc_DataOut,
	usbd_cdc_SOF,
	NULL,
	usbd_cdc_IsoOutIncomplete,
	USBD_cdc_GetCfgDesc,
	USBD_cdc_GetCfgDesc,
	USBD_cdc_GetUsrStr
};


/* USB CDC device Configuration Descriptor */
__ALIGN_BEGIN uint8_t usbd_cdc_CfgDesc[USB_CDC_CONFIG_DESC_SIZ]  __ALIGN_END =
{
	/* Configuration Descriptor */
	0x09,   /* bLength: Configuration Descriptor size */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
	USB_CDC_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
	0x00,
	0x04,   /* bNumInterfaces: 4 interface */
	0x01,   /* bConfigurationValue: Configuration value */
	0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
	0xC0,   /* bmAttributes: self powered */
	0x32,   /* MaxPower 100 mA */

	/*--------------------------------------------------------------------------*/
	/* Ethernet */
	/*--------------------------------------------------------------------------*/

	/* Interface 0 -- Ethernet control interface */
	0x09,   /* bLength: Interface Descriptor size */
	USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x00,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x01,   /* bNumEndpoints */
	0x02,   /* bInterfaceClass */
	0x06,   /* bInterfaceSubClass */
	0x00,   /* bInterfaceProtocol */
	0x00,   /* iInterface */

	0x05,   /* bLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x00,   /* Header functional descriptor */
	0x10, 0x01,

	0x05,   /* bLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x06,   /* Union functional descriptor */
	0x00,   /* bMasterInterface */
	0x01,   /* bSlaveInterface */

	0x0D,   /* bLength */
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x0F,   /* Ethernet networking functional descriptor */
	USBD_IDX_SERIAL_STR,     /* Index of MAC address string */
	0x00, 0x00, 0x00, 0x00,  /* bmEthernetStatictics */
	0xEA, 0x05,              /* wMaxSegmentSize = 1514 */
	0x00, 0x00,              /* wNumberMCFilters = 0 */
	0x00,   /* bNumberPowerFilters = 0 */

	0x07,                         /* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType */
	ETH_NOTIFICATION_EP,          /* bEndpointAddr */
	0x03,                         /* bmAttributes */
	ETH_NOTIFY_EP_SIZE, 0,        /* wMaxPacketSize */
	0x01,                         /* bInterval */

	/* Interface 1 -- Ethernet data interface */
	0x09,   /* bLength: Interface Descriptor size */
	USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x01,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints */
	0x0A,   /* bInterfaceClass */
	0x00,   /* bInterfaceSubClass */
	0x00,   /* bInterfaceProtocol */
	0x00,   /* iInterface */

	/* Endpoint OUT Descriptor */
	0x07,   /* bLength: Endpoint Descriptor size */
	USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
	ETH_OUT_EP,                        /* bEndpointAddress */
	0x02,                              /* bmAttributes: Bulk */
	LOBYTE(ETH_DATA_EP_SIZE),          /* wMaxPacketSize: */
	HIBYTE(ETH_DATA_EP_SIZE),
	0x00,                              /* bInterval: ignore for Bulk transfer */

	/* Endpoint IN Descriptor */
	0x07,   /* bLength: Endpoint Descriptor size */
	USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
	ETH_IN_EP,                         /* bEndpointAddress */
	0x02,                              /* bmAttributes: Bulk */
	LOBYTE(ETH_DATA_EP_SIZE),          /* wMaxPacketSize: */
	HIBYTE(ETH_DATA_EP_SIZE),
	0x00,                              /* bInterval: ignore for Bulk transfer */

	/*--------------------------------------------------------------------------*/
	/* Audio */
	/*--------------------------------------------------------------------------*/

	/* Interface 2 -- Audio control interface */
	0x09,   /* bLength: Interface Descriptor size */
	USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x02,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x00,   /* bNumEndpoints */
	0x01,   /* bInterfaceClass */
	0x01,   /* bInterfaceSubClass */
	0x00,   /* bInterfaceProtocol */
	0x00,   /* iInterface */

	9,      /* bLength: Interface Descriptor size */
	36,     /* bDescriptorType: AudioStreaming interface */
	1,      /* bDescriptorSubtype: HEADER */
	1, 0,   /* bcdADC */
	30, 0,  /* wTotalLength */
	1,      /* bInCollection */
	3,      /* baInterfaceNr */

	12,     /* bLength: Interface Descriptor size */
	36,     /* bDescriptorType: AudioStreaming interface */
	2,      /* bDescriptorSubtype: INPUT_TERMINAL */
	1,      /* bTerminalId */
	1, 1,   /* wTerminalType */
	0,      /* bAssocTerminal */
	1,      /* bNrChannels */
	0, 0,   /* wChannelConfig */
	0,      /* iChannelNames */
	0,      /* iTerminal */

	9,      /* bLength: Interface Descriptor size */
	36,     /* bDescriptorType: AudioStreaming interface */
	3,      /* bDescriptorSubtype: OUTPUT_TERMINAL */
	2,      /* bTerminalId */
	5, 3,   /* wTerminalType: Room speaker */
	0,      /* bAssocTerminal */
	1,      /* bSourceId */
	0,      /* iTerminal */

	/* Interface 3 -- Audio data interface (idle) */
	0x09,   /* bLength: Interface Descriptor size */
	USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x03,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x00,   /* bNumEndpoints */
	0x01,   /* bInterfaceClass */
	0x02,   /* bInterfaceSubClass */
	0x00,   /* bInterfaceProtocol */
	0x00,   /* iInterface */

	/* Interface 3 -- Audio data interface (active) */
	0x09,   /* bLength: Interface Descriptor size */
	USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
	/* Interface descriptor type */
	0x03,   /* bInterfaceNumber: Number of Interface */
	0x01,   /* bAlternateSetting: Alternate setting */
	0x01,   /* bNumEndpoints */
	0x01,   /* bInterfaceClass */
	0x02,   /* bInterfaceSubClass */
	0x00,   /* bInterfaceProtocol */
	0x00,   /* iInterface */

	7,      /* bLength: Interface Descriptor size */
	36,     /* bDescriptorType: AudioStreaming interface */
	1,      /* bDescriptorSubtype: AS_GENERAL */
	1, 0, 1, 0,

	11,     /* bLength: Interface Descriptor size */
	36,     /* bDescriptorType: AudioStreaming interface */
	2,      /* bDescriptorSubtype: AS_FORMAT */
	1,      /* bFormatType */
	2,      /* bNrChannels */
	2,      /* bSubframeSize */
	16,     /* bBitResolution */
	1,      /* bSamFreqType */
	LOBYTE(48000), HIBYTE(48000), 0,   /* tSamFreq */

	/* Endpoint OUT Descriptor */
	0x09,   /* bLength: Endpoint Descriptor size */
	USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
	AUDIO_OUT_EP,                      /* bEndpointAddress */
	0x05,                              /* bmAttributes: Iso */
	LOBYTE(AUDIO_EP_SIZE),             /* wMaxPacketSize: */
	HIBYTE(AUDIO_EP_SIZE),
	0x01,                              /* bInterval */
	0x00,                              /* bRefresh */
	0x00,                              /* bSynchAddress */

	7,      /* bLength: Interface Descriptor size */
	37,     /* bDescriptorType: AudioStreaming endpoint */
	1,      /* bDescriptorSubtype: EP_GENERAL */
	0, 0, 0, 0
};

/**
  * @brief  usbd_cdc_Init
  *         Initilaize the CDC interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t usbd_cdc_Init(void *pdev, uint8_t cfgidx)
{
	DCD_EP_Open(pdev, ETH_IN_EP, ETH_DATA_EP_SIZE, USB_OTG_EP_BULK);
	DCD_EP_Open(pdev, ETH_OUT_EP, ETH_DATA_EP_SIZE, USB_OTG_EP_BULK);
	DCD_EP_Open(pdev, ETH_NOTIFICATION_EP, ETH_NOTIFY_EP_SIZE, USB_OTG_EP_INT);

	DCD_EP_Open(pdev, AUDIO_OUT_EP, AUDIO_EP_SIZE, USB_OTG_EP_ISOC);

	/* Prepare OUT endpoints to receive next packet */
	ecmDataRx(pdev, ETH_OUT_EP, 0);
	audioDataRx(pdev, AUDIO_OUT_EP, 0);

	return USBD_OK;
}

/**
  * @brief  usbd_cdc_Init
  *         DeInitialize the CDC layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  usbd_cdc_DeInit (void  *pdev, 
                                 uint8_t cfgidx)
{
	DCD_EP_Close(pdev, ETH_IN_EP);
	DCD_EP_Close(pdev, ETH_OUT_EP);
	DCD_EP_Close(pdev, ETH_NOTIFICATION_EP);

	DCD_EP_Close(pdev, AUDIO_OUT_EP);

	return USBD_OK;
}

/**
  * @brief  usbd_cdc_Setup
  *         Handle the CDC specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  usbd_cdc_Setup (void  *pdev, 
                                USB_SETUP_REQ *req)
{
  uint16_t len=USB_CDC_DESC_SIZ;
  uint8_t  *pbuf=usbd_cdc_CfgDesc + 9;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    /* CDC Class Requests -------------------------------*/
  case USB_REQ_TYPE_CLASS :
      /* Check if the request is a data setup packet */
      if (req->wLength)
      {
        /* Check if the request is Device-to-Host */
        if (req->bmRequest & 0x80)
        {
          /* Get the data to be sent to Host from interface layer */
          len = usb_Ctrl(pdev, req->bmRequest, req->bRequest, req->wIndex, CmdBuff, req->wLength);

          /* Send the data to the host */
          USBD_CtlSendData (pdev,
                            CmdBuff,
                            len);
        }
        else /* Host-to-Device request */
        {
          /* Set the value of the current command to be processed */
          cdcType = req->bmRequest;
          cdcCmd = req->bRequest;
          cdcIf = req->wIndex;
          cdcLen = req->wLength;

          /* Prepare the reception of the buffer over EP0
          Next step: the received data will be managed in usbd_cdc_EP0_TxSent() 
          function. */
          USBD_CtlPrepareRx (pdev,
                             CmdBuff,
                             req->wLength);
        }
      }
      else /* No Data request */
      {
        /* Transfer the command to the interface layer */
        usb_Ctrl(pdev, req->bmRequest, req->bRequest, req->wIndex, NULL, 0);
      }

      return USBD_OK;

    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL;



    /* Standard Requests -------------------------------*/
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
      if( (req->wValue >> 8) == CDC_DESCRIPTOR_TYPE)
      {
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
        pbuf = usbd_cdc_Desc;
#else
        pbuf = usbd_cdc_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM);
#endif
        len = MIN(USB_CDC_DESC_SIZ , req->wLength);
      }

      USBD_CtlSendData (pdev,
                        pbuf,
                        len);
      break;

    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&usbd_cdc_AltSet,
                        1);
      break;

    case USB_REQ_SET_INTERFACE :
      if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM)
      {
        usbd_cdc_AltSet = (uint8_t)(req->wValue);
      }
      else
      {
        /* Call the error management function (command will be nacked */
        USBD_CtlError (pdev, req);
      }
      break;
    }
  }
  return USBD_OK;
}

/**
  * @brief  usbd_cdc_EP0_RxReady
  *         Data received on control endpoint
  * @param  pdev: device device instance
  * @retval status
  */
static uint8_t  usbd_cdc_EP0_RxReady (void  *pdev)
{
	if (cdcCmd != 0xFF) {
		/* Process the data */
		usb_Ctrl(pdev, cdcType, cdcCmd, cdcIf, CmdBuff, cdcLen);

		/* Reset the command variable to default value */
		cdcCmd = 0xFF;
	}

	return USBD_OK;
}

/**
  * @brief  usbd_cdc_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t usbd_cdc_DataIn (void *pdev, uint8_t epnum)
{
	switch (epnum) {
	case ETH_IN_EP:
		ecmDataTxCompleted(pdev, epnum);
		break;
	}

	return USBD_OK;
}

/**
  * @brief  usbd_cdc_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t usbd_cdc_DataOut (void *pdev, uint8_t epnum)
{
	uint16_t len;

	/* Get the received data buffer and update the counter */
	len = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;

	switch (epnum) {
	case ETH_OUT_EP:
		ecmDataRx(pdev, epnum, len);
		break;
	case AUDIO_OUT_EP:
		audioDataRx(pdev, epnum, len);
		break;
	}

	return USBD_OK;
}

/**
  * @brief  usbd_cdc_SOF
  *         Start Of Frame event management
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_cdc_SOF (void *pdev)
{
	return USBD_OK;
}

/**
  * @brief  usbd_cdc_IsoOutIncomplete
  * @param  pdev: instance
  * @retval status
  */
static uint8_t  usbd_cdc_IsoOutIncomplete (void *pdev)
{
	return USBD_OK;
}

/**
  * @brief  USBD_cdc_GetCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_cdc_GetCfgDesc (uint8_t speed, uint16_t *length)
{
  *length = sizeof (usbd_cdc_CfgDesc);
  return usbd_cdc_CfgDesc;
}

static uint8_t  *USBD_cdc_GetUsrStr ( uint8_t speed ,uint8_t index,  uint16_t *length)
{
	return NULL;
}
