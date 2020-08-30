#ifndef __PARSE_HEADER__
#define __PARSE_HEADER__

#include "execute.h"
#include "local.h"
#include "parse_typedef.h"

#define check_variable_ _n(check_variable_)
#define check_function_variable_ _n(check_function_variable_)
#define tagbody_tag_p _n(tagbody_tag_p)
#define parse_compile_toplevel_ _n(parse_compile_toplevel_)
#define eval_parse _n(eval_parse)

_g int check_variable_(addr symbol);
_g int check_function_variable_(addr symbol);
_g int tagbody_tag_p(addr pos);
_g int parse_compile_toplevel_(Execute ptr, addr expr, addr list, addr *ret);
_g int eval_parse(Execute ptr, addr *ret, addr pos, addr toplevel);

#endif

