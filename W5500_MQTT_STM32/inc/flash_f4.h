#ifndef __FLASH_IF_H
#define __FLASH_IF_H

#include "stm32f4xx.h"

#define ADDR_FLASH_SECTOR_0             ((uint32_t)0x08000000)  //16Kbyte
#define ADDR_FLASH_SECTOR_1             ((uint32_t)0x08004000)  //16Kbyte
#define ADDR_FLASH_SECTOR_2             ((uint32_t)0x08008000)  //16Kbyte
#define ADDR_FLASH_SECTOR_3             ((uint32_t)0x0800C000)  //16Kbyte
#define ADDR_FLASH_SECTOR_4             ((uint32_t)0x08010000)  //64Kbyte
#define ADDR_FLASH_SECTOR_5             ((uint32_t)0x08020000)  //128Kbyte
#define ADDR_FLASH_SECTOR_6             ((uint32_t)0x08040000)  //128Kbyte
#define ADDR_FLASH_SECTOR_7             ((uint32_t)0x08060000)  //128Kbyte
#define ADDR_FLASH_SECTOR_8             ((uint32_t)0x08080000)  //128Kbyte
#define ADDR_FLASH_SECTOR_9             ((uint32_t)0x080A0000)  //128Kbyte
#define ADDR_FLASH_SECTOR_10            ((uint32_t)0x080C0000)  //128Kbyte
#define ADDR_FLASH_SECTOR_11            ((uint32_t)0x080E0000)  //128Kbyte

#define APPLICATION_ADDRESS     ((uint32_t)0x08004000)    
#define USER_FLASH_END_ADDRESS  ((uint32_t)0x080FFFFF)
#define USER_FLASH_SIZE         (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

typedef void (*pFunction)(void);

void runApp(void);

#endif