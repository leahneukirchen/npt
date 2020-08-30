#ifndef __STREAM_STRING_HEADER__
#define __STREAM_STRING_HEADER__

#include "local.h"
#include "typedef.h"

#define open_input_string_stream_ _n(open_input_string_stream_)
#define open_input_string_stream1_ _n(open_input_string_stream1_)
#define open_input_string_stream2_ _n(open_input_string_stream2_)
#define open_input_char_stream _n(open_input_char_stream)
#define null_input_string_stream _n(null_input_string_stream)
#define getindex_input_stream _n(getindex_input_stream)
#define setvalue_input_string_stream _n(setvalue_input_string_stream)
#define clear_input_string_stream _n(clear_input_string_stream)
#define close_input_string_stream _n(close_input_string_stream)
#define open_output_string_stream _n(open_output_string_stream)
#define copy_termsize_string_stream_ _n(copy_termsize_string_stream_)
#define string_stream_alloc_ _n(string_stream_alloc_)
#define string_stream_local_ _n(string_stream_local_)
#define string_stream_heap_ _n(string_stream_heap_)
#define clear_output_string_stream _n(clear_output_string_stream)
#define set_pretty_output_string_stream _n(set_pretty_output_string_stream)
#define get_pretty_output_string_stream _n(get_pretty_output_string_stream)
#define open_extend_output_stream _n(open_extend_output_stream)
#define close_output_string_stream _n(close_output_string_stream)
#define init_stream_string_input _n(init_stream_string_input)
#define init_stream_string_output _n(init_stream_string_output)

_g int open_input_string_stream_(addr *stream, addr string);
_g int open_input_string_stream1_(addr *stream, addr string, size_t start);
_g int open_input_string_stream2_(addr *stream, addr string, size_t start, size_t end);
_g void open_input_char_stream(addr *stream, const char *str);
_g void null_input_string_stream(addr *stream);
_g void getindex_input_stream(addr stream, size_t *ret);
_g void setvalue_input_string_stream(addr stream, addr value);
_g void clear_input_string_stream(addr stream);
_g void close_input_string_stream(addr stream);

_g void open_output_string_stream(addr *stream, size_t size);
_g int copy_termsize_string_stream_(addr stream, addr src);
_g int string_stream_alloc_(LocalRoot local, addr stream, addr *string);
_g int string_stream_local_(LocalRoot local, addr stream, addr *string);
_g int string_stream_heap_(addr stream, addr *string);
_g void clear_output_string_stream(addr stream);
_g void set_pretty_output_string_stream(addr stream);
_g int get_pretty_output_string_stream(addr stream);
_g void open_extend_output_stream(addr *stream, addr array);
_g void close_output_string_stream(addr stream);

_g void init_stream_string_input(void);
_g void init_stream_string_output(void);

#endif

