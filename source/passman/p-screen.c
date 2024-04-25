#include "../passman.h"

bool running = true;

cnfwin_t* extpop;
infwin_t* szepop;

menu_t  menu;

static void menu_refresh(void)
{
  switch(menu)
  {
    case MENU_DATABASES:
      mendbs_refresh();
      break;

    case MENU_PASSWORD:
      menpsw_refresh();
      break;

    default:
      break;
  }
}

void screen_refresh(void)
{
  clear();
  refresh();

  menu_refresh();

  infwin_refresh(szepop);

  cnfwin_refresh(extpop);
}

static void popups_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  cnfwin_resize(extpop, x, y, 24);

  infwin_resize(szepop, x, y, 104, 26);
}

static void menus_resize(int xmax, int ymax)
{
  mendbs_resize(xmax, ymax);

  menpsw_resize(xmax, ymax);
}

void screen_resize(void)
{
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  menus_resize(xmax, ymax);

  popups_resize(xmax, ymax);
}

static void menus_init(int xmax, int ymax)
{
  mendbs_init(xmax, ymax);

  menpsw_init(xmax, ymax);
}

static void menus_free(void)
{
  mendbs_free();

  menpsw_free();
}

static void popups_init(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  extpop = cnfwin_create(x, y, 24, "Do you want to exit?", "Yes", "No", false);

  szepop = infwin_create(x, y, 104, 26, "Info", "Resize the terminal to match this window", true);
}

static void popups_free(void)
{
  cnfwin_free(extpop);

  infwin_free(szepop);
}

void screen_init(void)
{
  initscr();
  noecho();
  curs_set(0);
  raw();

  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  menus_init(xmax, ymax);

  popups_init(xmax, ymax);
}

void screen_free(void)
{
  menus_free();

  popups_free();

  endwin();
}

void screen_key_handler(int key)
{
  switch(key)
  {
    case KEY_CTRLC:
      cnfpop_input(extpop, NULL);

      if(extpop->answer) running = false;
      break;

    case KEY_RESIZE:
      screen_resize();
      break;

    default:
      break;
  }
}
