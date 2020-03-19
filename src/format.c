#include <stdarg.h>
#include "condition.h"
#include "cons.h"
#include "cons_list.h"
#include "execute.h"
#include "format.h"
#include "format_function.h"
#include "format_parse.h"
#include "gc.h"
#include "local.h"
#include "stream.h"
#include "stream_string.h"
#include "strtype.h"
#include "strvect.h"

/*****************************************************************************
 *  format
 *****************************************************************************/
/*
 * ~args opt operator
 *
 * arg -> [+-]?[0-9]+ | '. | [vV] | #
 * args -> (arg (, arg)*)?
 * opt -> : | @ | :@ | @:
 * operator -> aAsSdDbBoOxXrRpPcCfFeEgG$%&|~\ntT*?_wWiI()[];{}<>^
 *
 * vV		argument
 * #		count
 *
 * aA		Ascii
 * sS		S-expression
 * dD		Decimal
 * bB		Binary
 * oO		Octal
 * xX		Hexadecimal
 * rR		Radix
 * pP		Plural
 * cC		Character
 * fF		Fixed-format floating-point
 * eE		Exponential floating-point
 * gG		General floating-point
 * $		Dollars floating-point
 * %		terpri
 * &		fresh-line
 * |		Page out
 * ~		Tilde
 * \n		Line espace
 * tT		Tabulate
 * *		Ignore
 * ?		Indirection
 * _		Conditional newline
 * wW		Write
 * iI		Indent
 * ()		Case conversion
 * []		Conditional expression
 * ;		sparate
 * {}		Iteration
 * <>		Justification, Logical Block
 * ^		Up and out
 */
_g int format_stream_lisp(Execute ptr, addr stream, addr format, addr args)
{
	return format_execute(ptr, stream, format, args, &args);
}

_g int format_string_lisp(Execute ptr, addr format, addr args, addr *ret)
{
	addr stream;

	open_output_string_stream(&stream, 0);
	if (format_stream_lisp(ptr, stream, format, args)) {
		close_stream(stream);
		return 1;
	}
	string_stream_heap(stream, ret);
	close_stream(stream);

	return 0;
}

_g int format_array_lisp(Execute ptr, addr array, addr format, addr args, addr *ret)
{
	int result;
	addr stream;

	open_extend_output_stream(&stream, array);
	result = format_stream_lisp(ptr, stream, format, args);
	close_stream(stream);
	*ret = array;

	return result;
}

_g int format_lisp(Execute ptr, addr stream, addr format, addr args, addr *ret)
{
	if (stream == Nil)
		return format_string_lisp(ptr, format, args, ret);

	if (stream == T) {
		standard_output_stream(ptr, &stream);
		return format_stream_lisp(ptr, stream, format, args);
	}

	if (stringp(stream))
		return format_array_lisp(ptr, stream, format, args, ret);

	return format_stream_lisp(ptr, stream, format, args);
}


/*
 *  format clang
 */
static int format_stdarg(Execute ptr,
		addr stream, const char *str, va_list args, addr *ret)
{
	addr format, list;
	LocalRoot local;
	LocalStack stack;

	local = ptr->local;
	push_local(local, &stack);
	strvect_char_local(local, &format, str);
	list_alloc_stdarg(local, &list, args);
	if (format_lisp(ptr, stream, format, list, ret))
		return 1;
	rollback_local(local, stack);

	return 0;
}

_g int format_stream(Execute ptr, addr stream, const char *str, ...)
{
	int check;
	va_list args;

	va_start(args, str);
	check = format_stdarg(ptr, stream, str, args, NULL);
	va_end(args);

	return check;
}

_g int format_string(Execute ptr, addr *ret, const char *str, ...)
{
	int check;
	va_list args;

	va_start(args, str);
	check = format_stdarg(ptr, Nil, str, args, ret);
	va_end(args);

	return check;
}

_g int format_stdout(Execute ptr, const char *str, ...)
{
	int check;
	va_list args;

	va_start(args, str);
	check = format_stdarg(ptr, T, str, args, NULL);
	va_end(args);

	return check;
}

_g void format(const char *str, ...)
{
	int check;
	va_list args;

	va_start(args, str);
	check = format_stdarg(Execute_Thread, T, str, args, NULL);
	va_end(args);
	if (check) {
		Abort("format error.");
	}
}


/*
 *  initialize
 */
_g void init_format(void)
{
	init_format_parse();
	init_format_function();
}

