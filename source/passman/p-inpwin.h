#ifndef P_INPWIN_H
#define P_INPWIN_H

#include "../passman.h"

typedef struct
{
  window_t* window;
  char* buffer;     // The inputted string
  int msize;        // Max size of buffer
  int length;       // The length of the string
  int cursor;       // The index which the cursor is on
  int scroll;       // How many characters that is scrolled
  bool hidden;      // Render like a hidden password
  bool secret;      // Only secret inputs can hide content
  char* title;      // Title
  int ttllen;       // Title length
} inpwin_t;         // Input window

extern inpwin_t* inpwin_create(int x, int y, int w, char* buffer, size_t size, char* prompt, bool secret, bool active);

extern void inpwin_resize(inpwin_t* inpwin, int x, int y, int w);

extern void inpwin_free(inpwin_t* inpwin);

extern void inpwin_refresh(inpwin_t* inpwin);

extern void inpwin_input(inpwin_t* inpwin, void (*key_handler)(int));

extern void inppop_input(inpwin_t* inppop, void (*key_handler)(int));

extern void inpwin_buffer_set(inpwin_t* inpwin, char* buffer, size_t size);

extern void inpwin_key_handler(inpwin_t* inpwin, int key);

#endif // P_INPWIN_H
