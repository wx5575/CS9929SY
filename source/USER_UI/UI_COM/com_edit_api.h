
#ifndef __COM_EDIT_API_H__
#define __COM_EDIT_API_H__

#include "com_ui_info.h"


/**
  * @brief  数据是否上载到内存的标记枚举定义
  */
typedef enum{
    UNLOAD_TO_RAM,///<不上载数据到RAM
    LOAD_TO_RAM,///<上载数据到RAM
}LOAD_DATA_FLAG;

extern void init_save_file_ui(MYUSER_WINDOW_T* win, WIDGET_ELEMENT *ele_pool, TEST_FILE *file);
extern WIDGET_ELEMENT *get_cur_win_edit_ele_list_head(void);
extern TEXT_ELE_T *get_cur_win_text_ele_list_head(void);
extern void dis_select_edit_ele(WIDGET_ELEMENT *ele, LOAD_DATA_FLAG flag);
extern void select_edit_ele(WIDGET_ELEMENT *ele);
extern void select_text_ele(TEXT_ELE_T *ele);
extern void dis_select_text_ele(TEXT_ELE_T *ele);
extern void set_scan_key_custom_fun(void (*fun)());
extern void update_cur_edit_handle(void);
extern void upload_par_to_ram(WIDGET_ELEMENT* ele);
extern void load_par_to_buf(WIDGET_ELEMENT* ele, void *value);
extern WM_HMEM get_cur_edit_handle(void);
extern void set_edit_ele_data(WIDGET_ELEMENT *ele, void *data);
extern void init_window_edit_ele(MYUSER_WINDOW_T* win);
extern void clear_edit_ele(int hWin);
extern void menu_key_backspace(int hWin);
extern void del_a_char_from_edit_str(uint8_t cur);
extern void add_a_char_into_edit_str(uint8_t ch, uint8_t cur, uint8_t max_len);
extern void set_edit_num_value(WIDGET_ELEMENT* ele, uint32_t value);
extern uint32_t get_edit_num_value(WIDGET_ELEMENT* ele);
extern CS_INDEX get_data_in_resource_table_index(WIDGET_ELEMENT *ele, uint8_t bytes, int32_t *data);

#endif //__COM_EDIT_API_H__

