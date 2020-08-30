#ifndef __SORT_HEADER__
#define __SORT_HEADER__

#include "execute.h"
#include "typedef.h"

#define simplesort_cons_unsafe_ _n(simplesort_cons_unsafe_)
#define simplesort_info_cons_unsafe_ _n(simplesort_info_cons_unsafe_)
#define simple_sort_sequence_ _n(simple_sort_sequence_)
#define bubble_sort_sequence_ _n(bubble_sort_sequence_)
#define quick_sort_sequence_ _n(quick_sort_sequence_)
#define merge_sort_sequence_ _n(merge_sort_sequence_)

/* unsafe */
_g int simplesort_cons_unsafe_(addr *ret,
		addr cons, int (*call_)(addr left, addr right, int *ret));
_g int simplesort_info_cons_unsafe_(addr *ret, addr cons, addr info,
		int (*call_)(addr info, addr left, addr right, int *ret));

/* sort */
_g int simple_sort_sequence_(Execute ptr, addr pos, addr call, addr key);
_g int bubble_sort_sequence_(Execute ptr, addr pos, addr call, addr key);
_g int quick_sort_sequence_(Execute ptr, addr pos, addr call, addr key);
_g int merge_sort_sequence_(Execute ptr, addr pos, addr call, addr key);

#endif

