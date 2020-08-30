#ifndef __CALL_ARRAYS_HEADER__
#define __CALL_ARRAYS_HEADER__

#include "local.h"
#include "execute.h"
#include "typedef.h"

#define make_array_common _n(make_array_common)
#define adjust_array_common _n(adjust_array_common)
#define adjustable_array_p_common _n(adjustable_array_p_common)
#define aref_common _n(aref_common)
#define setf_aref_common _n(setf_aref_common)
#define array_dimension_common _n(array_dimension_common)
#define array_dimensions_common _n(array_dimensions_common)
#define array_element_type_common _n(array_element_type_common)
#define array_has_fill_pointer_p_common _n(array_has_fill_pointer_p_common)
#define array_displacement_common _n(array_displacement_common)
#define array_in_bounds_p_common _n(array_in_bounds_p_common)
#define array_rank_common _n(array_rank_common)
#define array_row_major_index_common _n(array_row_major_index_common)
#define array_total_size_common _n(array_total_size_common)
#define arrayp_common _n(arrayp_common)
#define fill_pointer_common _n(fill_pointer_common)
#define setf_fill_pointer_common _n(setf_fill_pointer_common)
#define row_major_aref_common _n(row_major_aref_common)
#define setf_row_major_aref_common _n(setf_row_major_aref_common)
#define simple_vector_p_common _n(simple_vector_p_common)
#define svref_common _n(svref_common)
#define setf_svref_common _n(setf_svref_common)
#define vectorp_common _n(vectorp_common)
#define bit_common _n(bit_common)
#define setf_bit_common _n(setf_bit_common)
#define bit_vector_p_common _n(bit_vector_p_common)
#define simple_bit_vector_p_common _n(simple_bit_vector_p_common)

_g int make_array_common(Execute ptr, addr var, addr rest, addr *ret);
_g int adjust_array_common(Execute ptr, addr pos, addr dim, addr rest, addr *ret);
_g int adjustable_array_p_common(addr var, int *ret);
_g int aref_common(addr var, addr rest, addr *ret);
_g int setf_aref_common(addr value, addr var, addr rest);
_g int array_dimension_common(addr var, addr axis, addr *ret);
_g int array_dimensions_common(addr var, addr *ret);
_g int array_element_type_common(addr var, addr *ret);
_g int array_has_fill_pointer_p_common(addr var, int *ret);
_g int array_displacement_common(addr pos, addr *ret, addr *offset);
_g int array_in_bounds_p_common(addr array, addr rest, int *ret);
_g int array_rank_common(addr pos, addr *ret);
_g int array_row_major_index_common(addr array, addr rest, addr *ret);
_g int array_total_size_common(addr array, addr *ret);
_g int arrayp_common(addr var);
_g int fill_pointer_common(Execute ptr, addr array, addr *ret);
_g int setf_fill_pointer_common(Execute ptr, addr value, addr array);
_g int row_major_aref_common(addr array, addr index, addr *ret);
_g int setf_row_major_aref_common(addr value, addr array, addr index);
_g int simple_vector_p_common(addr var);
_g int svref_common(addr pos, addr index, addr *ret);
_g int setf_svref_common(addr value, addr pos, addr index);
_g int vectorp_common(addr var);
_g int bit_common(addr pos, addr rest, addr *ret);
_g int setf_bit_common(addr value, addr pos, addr rest);
_g int bit_vector_p_common(addr var);
_g int simple_bit_vector_p_common(addr var);

#endif

