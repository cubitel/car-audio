#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

#include "usb_conf.h"

#define USBD_CFG_MAX_NUM                1
#define USBD_ITF_MAX_NUM                3
#define USB_MAX_STR_DESC_SIZ            50

/* Interfaces */
#define ETH_INTERFACE                   0
#define AUDIO_INTERFACE                 2

/* Endpoints */
#define ETH_IN_EP                       0x81  /* EP1 for data IN */
#define ETH_OUT_EP                      0x01  /* EP1 for data OUT */
#define ETH_NOTIFICATION_EP             0x82  /* EP2 for notifications */
#define AUDIO_OUT_EP                    0x03  /* EP3 for data OUT */

/* Endpoint sizes */
#define ETH_NOTIFY_EP_SIZE             16
#define ETH_DATA_EP_SIZE               64   /* ECM data endpoint packet size */
#define AUDIO_EP_SIZE                  192  /* Audio data endpoint packet size */

/* Buffer sizes */
#define ETH_CMD_BUFFER_SIZE            64   /* ETH control endpoint buffer size */
#define USB_MAX_PACKET_SIZE            192  /* For IN & OUT buffers */

#endif //__USBD_CONF__H__
