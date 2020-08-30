#ifndef __EXECUTE_SETJMP__
#define __EXECUTE_SETJMP__

#include "define_setjmp.h"
#include "typedef.h"

#define abort_execute _n(abort_execute)
#define set_abort_handler _n(set_abort_handler)
#define set_abort_setjmp_handler _n(set_abort_setjmp_handler)
#define set_degrade_setjmp_handler _n(set_degrade_setjmp_handler)

/* abort */
__extern lisp_abort_calltype Lisp_abort_handler;
_g void abort_execute(void);
_g lisp_abort_calltype set_abort_handler(lisp_abort_calltype call);
_g lisp_abort_calltype set_abort_setjmp_handler(void);

/* degrade */
_g lisp_abort_calltype set_degrade_setjmp_handler(void);

#endif

