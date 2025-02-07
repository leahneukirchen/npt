#include <stdarg.h>
#include "clos.h"
#include "clos_class.h"
#include "clos_type.h"
#include "condition.h"
#include "cons.h"
#include "cons_list.h"
#include "cons_plist.h"
#include "control_execute.h"
#include "control_object.h"
#include "control_operator.h"
#include "eval_table.h"
#include "equal.h"
#include "execute.h"
#include "execute_object.h"
#include "format.h"
#include "function.h"
#include "hold.h"
#include "restart.h"
#include "stream.h"
#include "stream_common.h"
#include "symbol.h"
#include "type_parse.h"

/*
 *  arguments
 */
void setargs_nil_control(Execute ptr)
{
	addr root;

	root = ptr->control;
	Check(root == Nil, "control error");
	SetControl(root, Control_Cons, Nil);
	SetControl(root, Control_ConsTail, Nil);
}

void setargs_va_control(Execute ptr, ...)
{
	addr pos;
	va_list args;

	setargs_nil_control(ptr);
	va_start(args, ptr);
	for (;;) {
		pos = va_arg(args, addr);
		if (pos == NULL)
			break;
		pushargs_control(ptr, pos);
	}
	va_end(args);
}

static void pushargs_list_control(Execute ptr, addr list)
{
	addr pos;

	while (list != Nil) {
		GetCons(list, &pos, &list);
		pushargs_control(ptr, pos);
	}
}

void setargs_list_control(Execute ptr, addr list)
{
	setargs_nil_control(ptr);
	pushargs_list_control(ptr, list);
}

void pushargs_control(Execute ptr, addr value)
{
	addr root, cons, next;

	root = ptr->control;
	Check(root == Nil, "control error");
	GetControl(root, Control_ConsTail, &cons);
	if (cons == Nil) {
		conscar_local(ptr->local, &cons, value);
		SetControl(root, Control_Cons, cons);
		SetControl(root, Control_ConsTail, cons);
	}
	else {
		conscar_local(ptr->local, &next, value);
		SetCdr(cons, next);
		SetControl(root, Control_ConsTail, next);
	}
}

int popargs_control_(Execute ptr, addr *ret)
{
	addr control, list;

	control = ptr->control;
	GetControl(control, Control_Cons, &list);
	if (list == Nil) {
		*ret = Unbound;
	}
	else {
		Return_getcons(list, ret, &list);
		SetControl(control, Control_Cons, list);
	}

	return 0;
}

void getargs_control(Execute ptr, size_t index, addr *ret)
{
	addr root;

	root = ptr->control;
	GetControl(root, Control_Cons, &root);
	getnth_unbound_unsafe(root, index, ret);
}

void getargs_tail_control(Execute ptr, addr *ret)
{
	addr cons;

	GetControl(ptr->control, Control_ConsTail, &cons);
	GetCar(cons, ret);
}

void getargs_list_control_unsafe(Execute ptr, size_t index, addr *ret)
{
	addr root;

	root = ptr->control;
	GetControl(root, Control_Cons, &root);
	getnthcdr_unsafe(root, index, ret);
}

void getargs_list_control_heap(Execute ptr, size_t index, addr *ret)
{
	addr cons;

	getargs_list_control_unsafe(ptr, index, &cons);
	copy_list_heap_unsafe(ret, cons);
}

void pushargs_allvalues(Execute ptr)
{
	int check;
	addr *values, pos, list;
	size_t size, sizer, i;

	sizer = ptr->sizer;
	check = EXECUTE_VALUES < sizer;
	size = check? EXECUTE_VALUES: sizer;

	values = ptr->values_reader;
	for (i = 0; i < size; i++) {
		pos = values[i];
		pushargs_control(ptr, pos);
	}
	if (! check)
		return;
	GetExecuteValuesList(ptr->values_vector, &list);
	while (list != Nil) {
		GetCons(list, &pos, &list);
		pushargs_control(ptr, pos);
	}
}


/*
 *  flow control
 */
int goto_control_(Execute ptr, size_t point)
{
	addr control;
	struct control_struct *str;

	control = ptr->control;
	for (;;) {
		str = StructControl(control);
		if (str->run_code)
			break;
		GetControl(control, Control_Next, &control);
		Check(control == Unbound, "revert-goto error.");
	}
	str->run_point = point;

	return 0;
}

int go_control_(Execute ptr, addr pos)
{
	struct taginfo_struct *str;

	/* find tag */
	CheckType(pos, LISPSYSTEM_TAGINFO);
	str = StructTagInfo(pos);
	if (str->open == 0) {
		GetNameTagInfo(pos, &pos);
		return fmte_("Tag ~S already closed.", pos, NULL);
	}

	/* rollback */
	ptr->throw_value = throw_tagbody;
	ptr->throw_handler = pos;
	ptr->throw_control = str->control;
	ptr->throw_point = str->point;
	ptr->throw_point_p = 1;
	return 1;
}

int return_from_control_(Execute ptr, addr pos)
{
	struct taginfo_struct *str;

	/* find name */
	CheckType(pos, LISPSYSTEM_TAGINFO);
	str = StructTagInfo(pos);
	if (str->open == 0) {
		GetNameTagInfo(pos, &pos);
		return fmte_("Block ~S already closed.", pos, NULL);
	}

	/* rollback */
	ptr->throw_value = throw_block;
	ptr->throw_handler = pos;
	ptr->throw_control = str->control;
	ptr->throw_point_p = 0;
	return 1;
}

void catch_control(Execute ptr, addr name)
{
	setcatch_control(ptr->local, ptr->control, name);
}

static int throw_find_control(Execute ptr, addr *ret, addr name)
{
	addr control, check;

	control = ptr->control;
	while (control != Nil) {
		if (getcatch_control(control, &check)) {
			if (check == name) {
				*ret = control;
				return 1;
			}
		}
		GetControl(control, Control_Next, &control);
	}
	*ret = 0;
	return 0;
}

int throw_control_(Execute ptr, addr name)
{
	addr next;

	/* find name */
	if (! throw_find_control(ptr, &next, name)) {
		return call_simple_control_error_va_(ptr,
				"Cannot find catch name ~S.", name, NULL);
	}
	/* rollback */
	ptr->throw_value = throw_catch;
	ptr->throw_handler = name;
	ptr->throw_control = next;
	ptr->throw_point_p = 0;
	return 1;
}


/*
 *  handler
 */
int pushhandler_common_(Execute ptr, addr name, addr call, int escape)
{
	addr pos;

	Return(parse_type_(ptr, &name, name, Nil));
	CheckType(name, LISPTYPE_TYPE);
	handler_local(ptr->local, &pos, name, call, escape);
	pushhandler_control(ptr, pos);

	return 0;
}

void reverse_handler_control(Execute ptr)
{
	addr control, list;

	control = ptr->control;
	gethandler_control(control, &list);
	nreverse(&list, list);
	sethandler_control(ptr->local, control, list);
}

void pushbind_restart_control(Execute ptr, addr list, int escape)
{
	addr name, form, inter, report, test, pos;

	/* A2 restart */
	List_bind(list, &name, &form, &inter, &report, &test, NULL);
	restart_heap(&pos, name);
	setfunction_restart(pos, form);
	setinteractive_restart(pos, inter);
	setreport_restart(pos, report);
	settest_restart(pos, test);
	setescape_restart(pos, escape);

	/* push handler */
	pushrestart_control(ptr, pos);
}

void reverse_restart_control(Execute ptr)
{
	addr control, list;

	control = ptr->control;
	getrestart_control(control, &list);
	nreverse(&list, list);
	setrestart_control(ptr->local, control, list);
}


/*
 *  find-condition
 */
int find_condition_control_(Execute ptr, addr instance, int *ret)
{
	int check;
	addr control, list, pos;

	for (control = ptr->control; control != Nil; ) {
		gethandler_control(control, &list);
		while (list != Nil) {
			GetCons(list, &pos, &list);
			Return(checkhandler_control_(ptr, pos, instance, &check));
			if (check) {
				return Result(ret, 1);
			}
		}
		GetControl(control, Control_Next, &control);
	}

	return Result(ret, 0);
}


/*
 *  invoke-handler
 */
static int wake_handler_call_(Execute ptr, addr next, addr instance, addr pos)
{
	addr call;

	/* call */
	GetCallHandler(pos, &call);
	Return(funcall_control_(ptr, call, instance, NULL));

	/* escape */
	if (getescape_handler(pos)) {
		ptr->throw_value = throw_handler_case;
		ptr->throw_handler = pos;
		ptr->throw_control = next;
		ptr->throw_point_p = 0;
		return 1;
	}

	return 0;
}

static int wake_handler_(Execute ptr, addr next, addr instance, addr pos)
{
	int check;

	setdisable_handler(pos, 1);
	check = wake_handler_call_(ptr, next, instance, pos);
	setdisable_handler(pos, 0);

	return check;
}

int invoke_handler_control_(Execute ptr, addr instance)
{
	int check;
	addr next, list, pos;

	next = ptr->control;
	while (next != Nil) {
		gethandler_control(next, &list);
		while (list != Nil) {
			GetCons(list, &pos, &list);
			Return(checkhandler_control_(ptr, pos, instance, &check));
			if (check) {
				Return(wake_handler_(ptr, next, instance, pos));
			}
		}
		GetControl(next, Control_Next, &next);
	}

	return 0;
}


/*
 *  invoke-restart
 */
static int check_enable_restart_control_(addr restart)
{
	if (! getenable_restart(restart)) {
		return call_simple_control_error_va_(NULL,
				"The restart ~S is already closed.", restart, NULL);
	}

	return 0;
}

static int rollback_restart_control_(addr control, addr restart, addr *ret)
{
	addr list, check;

	while (control != Nil) {
		getrestart_control(control, &list);
		while (list != Nil) {
			GetCons(list, &check, &list);
			if (check == restart)
				return Result(ret, control);
		}
		GetControl(control, Control_Next, &control);
	}
	*ret = 0;
	return fmte_("The restart ~S is invalid.", restart, NULL);
}

int invoke_restart_control_(Execute ptr, addr restart, addr args)
{
	int escape;
	addr call, next;

	if (symbolp(restart)) {
		Return(find_restart_control_error_(ptr, restart, Nil, &restart));
	}

	/* call */
	Return(check_enable_restart_control_(restart));
	getfunction_restart(restart, &call);
	Return(apply_control_(ptr, call, args));

	/* escape */
	escape = getescape_restart(restart);
	if (escape) {
		Return(rollback_restart_control_(ptr->control, restart, &next));
		ptr->throw_value = throw_restart_case;
		ptr->throw_handler = restart;
		ptr->throw_control = next;
		ptr->throw_point_p = 0;
		return 1;
	}

	return 0;
}

int invoke_restart_interactively_control_(Execute ptr, addr restart)
{
	addr args;

	if (symbolp(restart)) {
		Return(find_restart_control_error_(ptr, restart, Nil, &restart));
	}

	Return(check_enable_restart_control_(restart));
	getinteractive_restart(restart, &args);
	if (args != Nil) {
		Return(apply1_control_(ptr, &args, args, Nil));
	}

	return invoke_restart_control_(ptr, restart, args);
}


/*
 *  find-restart
 */
static int test_restart_control_(Execute ptr, addr restart, addr condition, int *ret)
{
	int check;
	addr test, list;

	/* associated */
	if (condition != Nil) {
		getassociated_restart(restart, &list);
		if (list != Nil) {
			check = find_list_eq_unsafe(condition, list);
			if (! check)
				return Result(ret, 0);
		}
	}

	/* :test */
	gettest_restart(restart, &test);
	if (test != Nil) {
		Return(funcall1_control_(ptr, &test, test, condition, NULL));
		if (test == Nil)
			return Result(ret, 0);
	}

	/* :test (constantly t) */
	return Result(ret, 1);
}

static int equal_restart_control_(Execute ptr,
		addr restart, addr symbol, addr condition, addr *value, int *ret)
{
	int check;
	addr name;

	getname_restart(restart, &name);
	if (name == symbol) {
		Return(test_restart_control_(ptr, restart, condition, &check));
		if (check) {
			*value = restart;
			return Result(ret, 1);
		}
	}

	return Result(ret, 0);
}

static int find_restart_stack_(Execute ptr,
		addr symbol, addr condition, addr *value, int *ret)
{
	int check;
	addr control, list, restart;

	control = ptr->control;
	while (control != Nil) {
		getrestart_control(control, &list);
		while (list != Nil) {
			GetCons(list, &restart, &list);
			Return(check_enable_restart_control_(restart));
			Return(equal_restart_control_(ptr,
						restart, symbol, condition, value, &check));
			if (check)
				return Result(ret, 1);
		}
		GetControl(control, Control_Next, &control);
	}

	/* not found */
	return Result(ret, 0);
}

int find_restart_control_(Execute ptr,
		addr name, addr condition, addr *value, int *ret)
{
	int check;

	/* name = restart */
	if (GetType(name) == LISPTYPE_RESTART) {
		if (getenable_restart(name)) {
			*value = name;
			return Result(ret, 1);
		}
		else {
			/* disable */
			*value = Nil;
			return Result(ret, 0);
		}
	}

	/* name = symbol */
	if (name == Nil) {
		*value = Nil;
		*ret = 0;
		return fmte_("The restart name ~S must not be a NIL.", name, NULL);
	}
	if (! symbolp(name)) {
		*value = Nil;
		*ret = 0;
		return fmte_("The argument ~S must be a symbol.", name, NULL);
	}
	if (condition != Nil) {
		Return(condition_instance_p_(condition, &check));
		if (! check) {
			*value = Nil;
			*ret = 0;
			return fmte_("The argument ~S "
					"must be a NIL or condition.", condition, NULL);
		}
	}

	return find_restart_stack_(ptr, name, condition, value, ret);
}

int find_restart_control_error_(Execute ptr, addr name, addr condition, addr *ret)
{
	int check;

	Return(find_restart_control_(ptr, name, condition, ret, &check));
	if (check == 0)
		return fmte_("The restart name ~S is not found.", name, NULL);

	return 0;
}

int compute_restarts_control_(Execute ptr, addr condition, addr *ret)
{
	int check;
	addr control, root, list, restart;
	LocalHold hold;

	if (condition != Nil) {
		Return(condition_instance_p_(condition, &check));
		if (! check) {
			*ret = Nil;
			return fmte_("The argument ~S "
					"must be a NIL or condition.", condition, NULL);
		}
	}
	hold = LocalHold_array(ptr, 1);
	control = ptr->control;
	for (root = Nil; control != Nil; ) {
		getrestart_control(control, &list);
		while (list != Nil) {
			GetCons(list, &restart, &list);
			Return(check_enable_restart_control_(restart));
			Return(test_restart_control_(ptr, restart, condition, &check));
			if (check) {
				pushnew_heap(root, restart, &root);
				localhold_set(hold, 0, root);
			}
		}
		GetControl(control, Control_Next, &control);
	}
	localhold_end(hold);
	nreverse(ret, root);

	return 0;
}


/*
 *  restart interface
 */
struct restart_call {
	union {
		int (*call_0)(Execute, void *);
		int (*call_1)(Execute, addr);
		int (*call_1r)(Execute, addr, addr *);
		int (*call_2)(Execute, addr, addr);
	} u;
	Execute ptr;
	addr restart;
	addr args[8];
	addr *ret;
	void *voidp;
};

static int restart_call_control_(struct restart_call *str,
		int (*call)(struct restart_call *))
{
	Execute ptr;
	addr control;

	/* execute */
	ptr = str->ptr;
	control = ptr->control;
	if (str->restart) {
		pushrestart_control(ptr, str->restart);
	}
	if ((*call)(str)) {
		if (! equal_control_restart(ptr, control)) {
			return 1;
		}
		/* restart abort */
		normal_throw_control(ptr);
		return 0;
	}

	/* free control */
	setresult_control(ptr, Nil);
	return 0;
}

/* restart */
int restart_control_(Execute ptr, int (*call)(Execute, void *), void *voidp)
{
	return restart0_control_(ptr, NULL, call, voidp);
}

/* restart0 */
static int restart0_control_adaptor(struct restart_call *str)
{
	return (str->u.call_0)(str->ptr, str->voidp);
}

int restart0_control_(Execute ptr, addr restart,
		int (*call)(Execute, void *), void *voidp)
{
	struct restart_call str;

	str.ptr = ptr;
	str.restart = restart;
	str.u.call_0 = call;
	str.voidp = voidp;
	return restart_call_control_(&str, restart0_control_adaptor);
}

/* restart1 */
static int restart1_control_adaptor(struct restart_call *str)
{
	return (str->u.call_1)(str->ptr, str->args[0]);
}

int restart1_control_(Execute ptr, addr restart,
		int (*call)(Execute, addr), addr v1)
{
	struct restart_call str;

	str.ptr = ptr;
	str.restart = restart;
	str.u.call_1 = call;
	str.args[0] = v1;
	return restart_call_control_(&str, restart1_control_adaptor);
}

/* restart1r */
static int restart1r_control_adaptor(struct restart_call *str)
{
	return (str->u.call_1r)(str->ptr, str->args[0], str->ret);
}

int restart1r_control_(Execute ptr, addr restart,
		int (*call)(Execute, addr, addr *), addr v1, addr *ret)
{
	struct restart_call str;

	str.ptr = ptr;
	str.restart = restart;
	str.u.call_1r = call;
	str.args[0] = v1;
	str.ret = ret;
	return restart_call_control_(&str, restart1r_control_adaptor);
}

/* restart2 */
static int restart2_control_adaptor(struct restart_call *str)
{
	return (str->u.call_2)(str->ptr, str->args[0], str->args[1]);
}

int restart2_control_(Execute ptr, addr restart,
		int (*call)(Execute, addr, addr), addr v1, addr v2)
{
	struct restart_call str;

	str.ptr = ptr;
	str.restart = restart;
	str.u.call_2 = call;
	str.args[0] = v1;
	str.args[1] = v2;
	return restart_call_control_(&str, restart2_control_adaptor);
}


/*
 *  code
 */
void set_taginfo_control(Execute ptr, addr list)
{
	addr control, pos, name, jump, lexical;
	size_t index;

	control = ptr->control;
	while (list != Nil) {
		GetCons(list, &pos, &list);
		List_bind(pos, &name, &jump, &lexical, NULL);
		GetIndex(jump, &index);
		taginfo_heap(&pos, control, name, index);
		GetIndex(lexical, &index);
		setlow_lexical_control(ptr, index, pos);
		pushtaginfo_control(ptr, pos);
	}
}

void set_blockinfo_control(Execute ptr, addr pos)
{
	addr name, lexical;
	size_t index;

	List_bind(pos, &name, &lexical, NULL);
	taginfo_heap(&pos, ptr->control, name, 0);
	GetIndex(lexical, &index);
	setlow_lexical_control(ptr, index, pos);
	pushtaginfo_control(ptr, pos);
}


/*
 *  catch / throw
 */
int catch_clang_(Execute ptr, pointer call, addr tag, addr value)
{
	addr pos, control;

	/* function */
	compiled_heap(&pos, Nil);
	setcompiled_empty(pos, call);
	SetDataFunction(pos, value);
	/* execute */
	push_control(ptr, &control);
	catch_control(ptr, tag);
	if (apply_control_(ptr, pos, Nil)) {
		if (equal_control_catch(ptr, tag))
			normal_throw_control(ptr);
	}

	return pop_control_(ptr, control);
}


/*
 *  stack-frame
 */
static void stack_frame_list(addr control, addr *ret)
{
	addr list, pos;

	list = Nil;
	while (control != Nil) {
		GetControl(control, Control_Call, &pos);
		if (pos != Nil)
			cons_heap(&list, pos, list);
		GetControl(control, Control_Next, &control);
	}

	nreverse(ret, list);
}

int stack_frame_stream_(Execute ptr, addr stream)
{
	int ignore;
	addr list, pos, index;
	LocalHold hold;
	fixnum i;

	hold = LocalHold_array(ptr, 1);
	stack_frame_list(ptr->control, &list);
	Return(fresh_line_stream_(stream, &ignore));
	localhold_set(hold, 0, list);

	for (i = 0; list != Nil; i++) {
		GetCons(list, &pos, &list);
		fixnum_heap(&index, i);
		Return(format_stream_(ptr, stream, "~A: ~A~%", index, pos, NULL));
	}

	return 0;
}

int stack_frame_output_(Execute ptr)
{
	addr stream;
	Return(debug_io_stream_(ptr, &stream));
	return stack_frame_stream_(ptr, stream);
}

