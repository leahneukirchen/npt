#ifndef __TERME_INPUT_HEADER__
#define __TERME_INPUT_HEADER__

#include "typedef.h"

#define terme_input_init _n(terme_input_init)
#define terme_input_clear _n(terme_input_clear)
#define terme_input_event _n(terme_input_event)

enum terme_escape {
	terme_escape_error,
	terme_escape_signal,
	terme_escape_hang,
	terme_escape_code,
	terme_escape_up,         /* ^P */
	terme_escape_down,       /* ^N */
	terme_escape_left,       /* ^F */
	terme_escape_right,      /* ^B */
	terme_escape_function,   /* Fx, PFx */
	terme_escape_return,     /* ^J, ^M, Enter */
	terme_escape_backspace,  /* ^H, BS */
	terme_escape_first,      /* ^A */
	terme_escape_last,       /* ^E */
	terme_escape_update,     /* ^L */
	terme_escape_delete,     /* ^D */
	terme_escape_rmleft,     /* ^U */
	terme_escape_rmright,    /* ^K */
	terme_escape_tabular,    /* ^I */
	terme_escape_search,     /* ^R */
	terme_escape_size
};
typedef enum terme_escape TermeEscape;

struct terme_keyboard {
	TermeEscape type;
	unicode c;
};
typedef struct terme_keyboard TermeKeyboard;

void terme_input_init(void);
int terme_input_clear(void);
void terme_input_event(int blocking, TermeKeyboard *ret);

#endif

