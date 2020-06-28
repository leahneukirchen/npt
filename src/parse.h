#ifndef __PARSE_HEADER__
#define __PARSE_HEADER__

#include "execute.h"
#include "local.h"
#include "parse_typedef.h"

_g void check_variable(addr symbol);
_g void check_function_variable(addr symbol);
_g int tagbody_tag_p(addr pos);
_g void parse_compile_toplevel(Execute ptr, addr expr, addr list, addr *ret);
_g int eval_parse(Execute ptr, addr *ret, addr pos, addr toplevel);

#endif

