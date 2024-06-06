#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 */
void win_list_resize(win_list_t* win, int x, int y, int w, int h)
{
  win_head_resize(win->head, x, y, w, h);

  // 1. Clamp the marked item to the floor
  if((win->item_index - win->scroll) >= (h - 2))
  {
    win->scroll = (win->item_index - (h - 2) + 1);
  }

  // 2. Clamp the marked item to the ceiling
  win->scroll = MIN(win->scroll, win->item_index);

  // 3. Prevent empty space when items can occupy the space
  if(win->scroll + (h - 2) > win->item_count)
  {
    win->scroll = MAX(0, win->item_count - (h - 2));
  }
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 *
 * RETURN (win_list_t* win)
 */
win_list_t* win_list_create(int x, int y, int w, int h, char** items, int count, bool active)
{
  win_list_t* win = malloc(sizeof(win_list_t));

  win->head = win_head_create(x, y, w, h, active);

  win->items      = items;
  win->item_count = count;

  return win;
}

void win_list_free(win_list_t* win)
{
  if(win == NULL) return;

  win_head_free(win->head);

  free(win);
}

void win_list_refresh(win_list_t* win)
{
  if(!win->head->active) return;

  win_head_clean(win->head);

  WINDOW* window = win->head->window;

  int ymax = win->head->ymax;

  int amount = MIN(win->item_count, ymax - 2);

  for(int index = 0; index < amount; index++)
  {
    int print_index = index + win->scroll;

    if(print_index == win->item_index) wattron(window, A_REVERSE);

    mvwprintw(window, index + 1, 1, "%s", win->items[print_index]);

    wattroff(window, A_REVERSE);
  }

  box(window, 0, 0);

  wrefresh(window);
}

static void win_list_scroll_down(win_list_t* win)
{
  win->item_index = MIN(win->item_count - 1, win->item_index + 1);

  int ymax = win->head->ymax;

  if((win->item_index - win->scroll) >= (ymax - 2))
  {
    win->scroll++; // Scroll down one more
  }
}

static void win_list_scroll_up(win_list_t* win)
{
  win->item_index = MAX(0, win->item_index - 1);

  win->scroll = MIN(win->scroll, win->item_index);
}

static void win_list_key_handler(win_list_t* win, int key)
{
  switch(key)
  {
    case 'j':
      win_list_scroll_down(win);
      break;

    case 'k':
      win_list_scroll_up(win);
      break;
  }
}