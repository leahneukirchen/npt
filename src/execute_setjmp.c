#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "execute_setjmp.h"
#include "terme.h"
#include "typedef.h"

#ifdef LISP_ABORT_SETJMP
#include <setjmp.h>
jmp_buf Lisp_abort_setjmp;
jmp_buf Lisp_degrade_setjmp;
#endif

/*
 *  abort
 */
static void abort_shutdown(void)
{
	exit(1); /* or abort(); */
}

void abort_execute(void)
{
	/* handler */
	if (Lisp_abort_handler) {
		(*Lisp_abort_handler)();
	}

	/* default */
	(void)text_color_terme(print_color_bright_red);
	(void)fprintf(stderr, "\n\n");
	(void)fprintf(stderr, "**************\n");
	(void)fprintf(stderr, "  LISP ABORT  \n");
	(void)fprintf(stderr, "**************\n");
	(void)text_color_terme(print_color_reset);
	abort_shutdown();
}

lisp_abort_calltype set_abort_handler(lisp_abort_calltype call)
{
	lisp_abort_calltype ret;

	ret = Lisp_abort_handler;
	Lisp_abort_handler = call;
	return ret;
}

static void abort_setjmp_handler(void)
{
	Lisp_abort_throw();
}
lisp_abort_calltype set_abort_setjmp_handler(void)
{
	return set_abort_handler(abort_setjmp_handler);
}


/*
 *  degrade
 */
static void degrade_setjmp_handler(void)
{
	Lisp_degrade_throw();
}
lisp_abort_calltype set_degrade_setjmp_handler(void)
{
	return set_abort_handler(degrade_setjmp_handler);
}

