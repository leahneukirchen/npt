#ifndef __FORMAT_FUNCTION_HEADER__
#define __FORMAT_FUNCTION_HEADER__

#include "execute.h"
#include "typedef.h"

#define format_execute _n(format_execute)
#define init_format_function _n(init_format_function)

_g int format_execute(Execute ptr, addr stream, addr format, addr args, addr *ret);
_g void init_format_function(void);

#endif

