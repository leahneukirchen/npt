#ifndef __WINDOWS_SCREEN_HEADER__
#define __WINDOWS_SCREEN_HEADER__

#include "typedef.h"

#define Window_SizeX _n(Window_SizeX)
#define Window_SizeY _n(Window_SizeY)
#define Window_CursorX _n(Window_CursorX)
#define Window_CursorY _n(Window_CursorY)

#define windows_screen_init _n(windows_screen_init)
#define windows_screen_begin _n(windows_screen_begin)
#define windows_screen_end _n(windows_screen_end)
#define windows_screen_textmode _n(windows_screen_textmode)
#define windows_screen_rawmode _n(windows_screen_rawmode)
#define windows_screen_getmode _n(windows_screen_getmode)
#define windows_screen_setmode _n(windows_screen_setmode)

#define windows_screen_lock_init _n(windows_screen_lock_init)
#define windows_screen_lock_free _n(windows_screen_lock_free)
#define windows_screen_enter _n(windows_screen_enter)
#define windows_screen_leave _n(windows_screen_leave)

extern unsigned Window_SizeX;
extern unsigned Window_SizeY;
extern unsigned Window_CursorX;
extern unsigned Window_CursorY;

void windows_screen_init(void);
void windows_screen_begin(void);
void windows_screen_end(void);
void windows_screen_textmode(void);
void windows_screen_rawmode(void);
int windows_screen_getmode(void);
void windows_screen_setmode(int mode);

void windows_screen_lock_init(void);
void windows_screen_lock_free(void);
void windows_screen_enter(void);
void windows_screen_leave(void);

#endif

