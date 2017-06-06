
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "GUI.H"
#include "IMAGE.H"
#include "stm32f4xx.h"

extern WM_HWIN create_u_ok_image(WM_HWIN hWin, uint16_t x, uint16_t y);
extern WM_HWIN create_u_ng_image(WM_HWIN hWin, uint16_t x, uint16_t y);
extern WM_HWIN create_unlock_image(WM_HWIN hWin, uint16_t x, uint16_t y);
extern WM_HWIN create_lock_image(WM_HWIN hWin, uint16_t x, uint16_t y);

extern WM_HWIN create_u_flash_1_image(WM_HWIN hWin, uint16_t x, uint16_t y);
extern WM_HWIN create_u_flash_2_image(WM_HWIN hWin, uint16_t x, uint16_t y);
extern WM_HWIN create_key_lock_image(WM_HWIN hWin, uint16_t x, uint16_t y);

extern void set_u_flash_1_ok_image(WM_HWIN hWin);
extern void set_u_flash_1_ng_image(WM_HWIN hWin);
extern void set_u_flash_2_ok_image(WM_HWIN hWin);
extern void set_u_flash_2_ng_image(WM_HWIN hWin);
extern void set_key_lock_image(WM_HWIN hWin);
extern void set_key_unlock_image(WM_HWIN hWin);

extern WM_HWIN create_capital_small_letter_image(WM_HWIN hWin, uint16_t x, uint16_t y);
extern void set_capital_letter_image(WM_HWIN hWin);
extern void set_small_letter_image(WM_HWIN hWin);

#endif //__IMAGE_H__
