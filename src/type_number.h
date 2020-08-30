#ifndef __TYPE_NUMBER_HEADER__
#define __TYPE_NUMBER_HEADER__

#include "define.h"
#include "type.h"

#define real_extract_local_ _n(real_extract_local_)
#define real_extract_heap_ _n(real_extract_heap_)
#define type_subtypep_p _n(type_subtypep_p)
#define type_optimized_or_subtypep _n(type_optimized_or_subtypep)
#define get_type_subtypep _n(get_type_subtypep)

_g int real_extract_local_(LocalRoot local, addr *ret, addr type);
_g int real_extract_heap_(LocalRoot local, addr *ret, addr type);
_g int type_subtypep_p(addr type);
_g int type_optimized_or_subtypep(addr type);
_g void get_type_subtypep(addr *ret, addr type);

#endif

