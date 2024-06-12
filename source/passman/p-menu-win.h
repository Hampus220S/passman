#ifndef P_MENU_WIN_H
#define P_MENU_WIN_H

extern void   menu_win_tab(menu_t* menu, bool reverse);

extern win_t* menu_active_win_get(menu_t* menu);

extern void   menu_name_win_focus_set(menu_t* menu, char* win_name);


extern win_t*         menu_name_win_get(menu_t* menu, char* win_name);

extern win_list_t*    menu_name_win_list_get(menu_t* menu, char* win_name);

extern win_confirm_t* menu_name_win_confirm_get(menu_t* menu, char* win_name);

extern win_text_t*    menu_name_win_text_get(menu_t* menu, char* win_name);

extern win_input_t*   menu_name_win_input_get(menu_t* menu, char* win_name);


extern void menu_win_input_resize(menu_t* menu, char* win_name, int x, int y, int w);

extern void menu_win_list_resize(menu_t* menu, char* win_name, int x, int y, int w, int h);

extern void menu_win_confirm_resize(menu_t* menu, char* win_name, int x, int y, int w);


extern void menu_win_input_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, key_handler_t* key_handler);

extern void menu_win_text_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, char* title, char* text, key_handler_t* key_handler);

extern void menu_win_list_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, char** items, int count, key_handler_t* key_handler);

extern void menu_win_confirm_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, key_handler_t* key_handler);

#endif // P_MENU_WIN_H
