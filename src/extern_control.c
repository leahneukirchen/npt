#include "condition.h"
#include "cons.h"
#include "cons_list.h"
#include "control.h"
#include "extern_control.h"
#include "extern_object.h"
#include "eval.h"
#include "execute.h"
#include "format.h"
#include "function.h"
#include "gc.h"
#include "local.h"
#include "pointer.h"
#include "strtype.h"
#include "unicode.h"

/*
 *  control
 */
addr lisp_push_control(void)
{
	addr x;
	push_close_control(Execute_Thread, &x);
	return x;
}

int lisp_free_control(addr control)
{
	if (GetType(control) != LISPTYPE_CONTROL)
		fmte("Invalid argument ~S.", control, NULL);
	return free_control(Execute_Thread, control);
}

int lisp_eval_control(addr eval)
{
	return eval_execute(Execute_Thread, eval);
}

int lisp_eval_string_control(addr eval)
{
	LocalRoot local;
	LocalStack stack;
	addr x;

	local = Local_Thread;
	push_local(local, &stack);
	Return1(lisp_reader(&x, eval));
	if (x == NULL)
		fmte("Invalid eval string ~S.", eval, NULL);
	Return1(lisp_eval_control(x));
	rollback_local(local, stack);

	return 0;
}

addr lisp_nth_control(size_t index)
{
	addr x;
	getvalues_control(Execute_Thread, index, &x);
	return x;
}

addr lisp_values_control(void)
{
	addr list;
	getvalues_list_control_heap(Execute_Thread, &list);
	return list;
}

addr lisp_result_control(void)
{
	addr x;
	getresult_control(Execute_Thread, &x);
	return x;
}

void lisp_result2_control(addr *ret1, addr *ret2)
{
	Execute ptr = Execute_Thread;
	getvalues_control(ptr, 0, ret1);
	getvalues_control(ptr, 1, ret2);
}

int lisp_eval8(addr *ret, const void *str)
{
	LocalRoot local;
	LocalStack stack;
	addr x, y;

	local = Local_Thread;
	push_local(local, &stack);
	string8_null_heap(&x, (const char *)str);
	Return1(lisp_reader(&y, x));
	if (x == NULL)
		fmte("Invalid eval string ~S.", x, NULL);
	Return1(lisp_eval_control(y));
	getresult_control(Execute_Thread, ret);
	rollback_local(local, stack);

	return 0;
}

int lisp_eval16(addr *ret, const void *str)
{
	LocalRoot local;
	LocalStack stack;
	addr x, y;

	local = Local_Thread;
	push_local(local, &stack);
	string16_null_heap(&x, (const byte16 *)str);
	Return1(lisp_reader(&y, x));
	if (x == NULL)
		fmte("Invalid eval string ~S.", x, NULL);
	Return1(lisp_eval_control(y));
	getresult_control(Execute_Thread, ret);
	rollback_local(local, stack);

	return 0;
}


/*
 *  format
 */
static int lisp_format_call(addr stream, addr format, addr args)
{
	addr control;
	Execute ptr;
	LocalHold hold;

	ptr = Execute_Thread;
	control = lisp_push_control();
	hold = LocalHold_local(ptr);
	localhold_pushva_force(hold, stream, format, args, NULL);
	Return1(format_lisp(ptr, stream, format, args, &args));
	localhold_end(hold);

	return lisp_free_control(control);
}

int lisp_format8(addr stream, const void *str, ...)
{
	addr format, args;
	va_list va;

	format = lisp_string8(str);
	va_start(va, str);
	list_alloc_stdarg(NULL, &args, va);
	va_end(va);

	return lisp_format_call(stream, format, args);
}

int lisp_format16(addr stream, const void *str, ...)
{
	addr format, args;
	va_list va;

	format = lisp_string16(str);
	va_start(va, str);
	list_alloc_stdarg(NULL, &args, va);
	va_end(va);

	return lisp_format_call(stream, format, args);
}


/*
 *  syscall
 */
void lisp_syscall_rest(int index, lisp_calltype_syscall call)
{
	if (index < 0 && LISP_POINTER_EXTEND <= index)
		fmte("Invalid index value ~S.", fixnumh((fixnum)index), NULL);
	index += (int)p_size;
	SetPointer_extend_dynamic(index, call);
}

void lisp_syscall_dynamic(int index, lisp_calltype_syscall call)
{
	if (index < 0 && LISP_POINTER_EXTEND <= index)
		fmte("Invalid index value ~S.", fixnumh((fixnum)index), NULL);
	index += (int)p_size;
	SetPointer_extend_rest(index, call);
}

addr lisp_syscall_function(int index, addr name)
{
	addr pos;

	if (index < 0 && LISP_POINTER_EXTEND <= index)
		fmte("Invalid index value ~S.", fixnumh((fixnum)index), NULL);
	index += (int)p_size;
	if (name == NULL)
		name = Nil;

	/* function */
	compiled_heap(&pos, name);
	switch (pointer_table[index].type) {
		case CallBind_extend_dynamic:
			setcompiled_extend_dynamic(pos, index);
			break;

		case CallBind_extend_rest:
			setcompiled_extend_rest(pos, index);
			break;

		default:
			fmte("Invalid callbind type, index = ~A", fixnumh((fixnum)index), NULL);
			return Nil;
	}

	return pos;
}

void lisp_syscall_setvalue(addr pos, addr value)
{
	if (! functionp(pos))
		fmte("Invalid function object ~S.", pos, NULL);
	if (value == NULL)
		value = Nil;
	SetDataFunction(pos, value);
}

addr lisp_syscall_getvalue(void)
{
	addr value;

	getdata_control(Execute_Thread, &value);
	if (value == Unbound || value == NULL)
		value = Nil;

	return value;
}

/*
 *  unwind-protect
 */
static void extern_unwind_protect(Execute ptr)
{
	addr clean;
	getdata_control(ptr, &clean);
	funcall_control(ptr, clean, NULL);
}

int lisp_unwind_protect(addr code, addr clean)
{
	int check;
	addr control, pos;
	Execute ptr;
	LocalHold hold;

	ptr = Execute_Thread;
	hold = LocalHold_array(ptr, 2);

	/* finalize */
	push_finalize_control(ptr, &control);
	localhold_set(hold, 0, code);
	localhold_set(hold, 1, clean);
	/* cleanup form */
	syscall_code(ptr->local, &pos, p_extern_unwind_protect, clean);
	setfinalize_control(ptr, control, pos);
	/* code */
	check = lisp_funcall(&code, code, NULL);
	Return1(free_check_control(ptr, control, check));
	localhold_end(hold);

	return 0;
}


/*
 *  initialize
 */
_g void init_extern_control(void)
{
	SetPointerType(empty, extern_unwind_protect);
}

