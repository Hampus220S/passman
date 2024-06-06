#ifndef P_WIN_LIST_H
#define P_WIN_LIST_H

typedef struct
{
  win_head_t* head;
  char**      items;      // The items in the list
  int         item_count; // The number of items
  int         item_index; // Which item the user hovers over
  int         scroll;     // How many lines has been scrolled down
} win_list_t;

extern win_list_t* win_list_create(int x, int y, int w, int h, char** items, int count, bool active);

extern void        win_list_free(win_list_t* win);

extern void        win_list_resize(win_list_t* win, int x, int y, int w, int h);

extern void        win_list_refresh(win_list_t* win);

#endif // P_WIN_LIST_H