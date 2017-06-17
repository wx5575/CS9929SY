
#ifndef __COM_EDIT_API_H__
#define __COM_EDIT_API_H__

/* Includes ------------------------------------------------------------------*/
#include "com_ui_info.h"


/**
  * @brief  数据是否上载到内存的标记枚举定义
  */
typedef enum{
    UNLOAD_TO_RAM,///<不上载数据到RAM
    LOAD_TO_RAM,///<上载数据到RAM
}LOAD_DATA_FLAG;

extern void init_save_file_ui(MYUSER_WINDOW_T* win, EDIT_ELE_T *ele_pool, TEST_FILE *file);
extern EDIT_ELE_T *get_cur_win_edit_ele_list_head(void);
extern TEXT_ELE_T *get_cur_win_text_ele_list_head(void);
extern void dis_select_edit_ele(EDIT_ELE_T *ele, LOAD_DATA_FLAG flag);
extern void select_edit_ele(EDIT_ELE_T *ele);
extern void select_text_ele(TEXT_ELE_T *ele);
extern void dis_select_text_ele(TEXT_ELE_T *ele);
extern void set_scan_key_custom_fun(void (*fun)());
extern void update_cur_edit_handle(void);
extern void upload_par_to_ram(EDIT_ELE_T* ele);
extern void load_par_to_buf(EDIT_ELE_T* ele, void *value);
extern WM_HMEM get_cur_edit_handle(void);
extern void set_edit_ele_data(EDIT_ELE_T *ele, void *data);
extern void init_window_edit_ele(MYUSER_WINDOW_T* win);
extern void clear_edit_ele(int hWin);
extern void menu_key_backspace(int hWin);
extern void del_a_char_from_edit_str(uint8_t cur);
extern void add_a_char_into_edit_str(uint8_t ch, uint8_t cur, uint8_t max_len);
extern void set_edit_num_value(EDIT_ELE_T* ele, uint32_t value);
extern CS_INDEX get_data_in_resource_table_index(EDIT_ELE_T *ele,
                                                uint8_t bytes,
                                                int32_t *data);
extern void init_menu_key_custom_inf(CUSTOM_MENU_KEY_INF *cus_inf,
                            uint16_t cus_size,
                            EDIT_ELE_T *edit,
                            MENU_KEY_INFO_T * inf,
                            uint16_t size);
extern uint32_t get_edit_ele_value(EDIT_ELE_T* ele, uint8_t *buf);
extern void set_sw_status_on(WM_HMEM hWin);
extern void set_sw_status_off(WM_HMEM hWin);
extern void update_ele_range_text(EDIT_ELE_T *ele);
extern void com_edit_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
extern void com_edit_win_direct_key_down_cb(KEY_MESSAGE *key_msg);
extern CS_INDEX get_edit_ele_index(EDIT_ELE_T *edit_pool, uint32_t pool_size, CS_INDEX index, CS_ERR*err);
extern EDIT_ELE_T* get_edit_ele_inf(EDIT_ELE_T *edit_pool, uint32_t pool_size, CS_INDEX index, CS_ERR*err);
extern void init_sw_type_edit_ele_resource_inf(EDIT_ELE_T* ele);
extern void set_g_cur_win_edit_index_inf(CS_INDEX *index_pool, uint32_t size);
extern void reg_step_ele_data(CS_INDEX index, void *data, uint8_t bytes);

#endif //__COM_EDIT_API_H__

