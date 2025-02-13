#include <math.h>
#include "array.h"
#include "array_access.h"
#include "array_inplace.h"
#include "array_make.h"
#include "bignum_equal.h"
#include "bit.h"
#include "build.h"
#include "cmpl.h"
#include "character.h"
#include "condition.h"
#include "cons.h"
#include "equal.h"
#include "hashtable.h"
#include "memory.h"
#include "number.h"
#include "number_equal.h"
#include "object.h"
#include "pathname_object.h"
#include "ratio.h"
#include "ratio_equal.h"
#include "strtype.h"
#include "strvect.h"
#include "structure.h"

static int array_get_unicode_check_(addr pos, size_t index, unicode *value, int *ret)
{
	addr x;
	struct array_value str;

	/* access */
	Return(arrayinplace_get_(pos, index, &str));

	/* character */
	if (str.type == ARRAY_TYPE_CHARACTER) {
		*value = str.value.character;
		return Result(ret, 1);
	}

	/* others */
	if (str.type != ARRAY_TYPE_T) {
		*value = 0;
		return Result(ret, 0);
	}

	/* object */
	x = str.value.object;
	if (! characterp(x)) {
		*value = 0;
		return Result(ret, 0);
	}

	/* character */
	GetCharacter(x, value);
	return Result(ret, 1);
}


/*
 *  eq
 */
int atom_function(addr pos)
{
	Check(pos == Unbound, "Unbound-variable");
	return GetType(pos) != LISPTYPE_CONS;
}

int eq_function(addr a, addr b)
{
	Check(a == Unbound, "Unbound-variable");
	Check(b == Unbound, "Unbound-variable");
	return a == b;
}

int eq_function_(addr a, addr b, int *ret)
{
	Check(a == Unbound, "Unbound-variable");
	Check(b == Unbound, "Unbound-variable");
	return Result(ret, a == b);
}


/*
 *  eql
 */
static int eql_function_fixnum(addr a, enum LISPTYPE type2, addr b)
{
	switch (type2) {
		case LISPTYPE_FIXNUM:
			return RefFixnum(a) == RefFixnum(b);

		case LISPTYPE_BIGNUM:
			return equal_fb_real(a, b);

		case LISPTYPE_RATIO:
			return equal_fr_real(a, b);

		default:
			return 0;
	}
}

static int eql_function_bignum(addr a, enum LISPTYPE type2, addr b)
{
	switch (type2) {
		case LISPTYPE_FIXNUM:
			return equal_bf_real(a, b);

		case LISPTYPE_BIGNUM:
			return equal_bb_real(a, b);

		case LISPTYPE_RATIO:
			return equal_br_real(a, b);

		default:
			return 0;
	}
}

static int eql_function_ratio(addr a, enum LISPTYPE type2, addr b)
{
	switch (type2) {
		case LISPTYPE_FIXNUM:
			return equal_rf_real(a, b);

		case LISPTYPE_BIGNUM:
			return equal_rb_real(a, b);

		case LISPTYPE_RATIO:
			return equal_rr_real(a, b);

		default:
			return 0;
	}
}

static int eql_function_single_float(addr a, addr b)
{
	single_float x, y;

	CheckType(a, LISPTYPE_SINGLE_FLOAT);
	CheckType(b, LISPTYPE_SINGLE_FLOAT);
	GetSingleFloat(a, &x);
	GetSingleFloat(b, &y);

	return (x == y) && (signbit(x) == signbit(y));
}

static int eql_function_double_float(addr a, addr b)
{
	double_float x, y;

	CheckType(a, LISPTYPE_DOUBLE_FLOAT);
	CheckType(b, LISPTYPE_DOUBLE_FLOAT);
	GetDoubleFloat(a, &x);
	GetDoubleFloat(b, &y);

	return (x == y) && (signbit(x) == signbit(y));
}

static int eql_function_long_float(addr a, addr b)
{
	long_float x, y;

	CheckType(a, LISPTYPE_LONG_FLOAT);
	CheckType(b, LISPTYPE_LONG_FLOAT);
	GetLongFloat(a, &x);
	GetLongFloat(b, &y);

	return (x == y) && (signbit(x) == signbit(y));
}

int eql_function(addr a, addr b)
{
	enum LISPTYPE type1, type2;

	Check(a == Unbound, "Unbound-variable");
	Check(b == Unbound, "Unbound-variable");

	/* eq */
	if (a == b)
		return 1;

	/* eql */
	type1 = GetType(a);
	type2 = GetType(b);
	switch (type1) {
		case LISPTYPE_FIXNUM:
			return eql_function_fixnum(a, type2, b);

		case LISPTYPE_BIGNUM:
			return eql_function_bignum(a, type2, b);

		case LISPTYPE_RATIO:
			return eql_function_ratio(a, type2, b);

		default:
			break;
	}

	if (type1 != type2)
		return 0;
	switch (type1) {
		case LISPTYPE_CHARACTER:
			return character_equal(a, b);

		case LISPTYPE_SINGLE_FLOAT:
			return eql_function_single_float(a, b);

		case LISPTYPE_DOUBLE_FLOAT:
			return eql_function_double_float(a, b);

		case LISPTYPE_LONG_FLOAT:
			return eql_function_long_float(a, b);

		case LISPTYPE_COMPLEX:
			return eql_complex(a, b);

		default:
			return 0;
	}
}

int eql_function_(addr left, addr right, int *ret)
{
	return Result(ret, eql_function(left, right));
}


/*
 *  equal
 */
static int equal_function_cons_(addr a, addr b, int *ret)
{
	int check;
	addr car1, car2, cdr1, cdr2;

	if (! consp(b))
		return Result(ret, 0);
	GetCons(a, &car1, &cdr1);
	GetCons(b, &car2, &cdr2);
	Return(equal_function_(car1, car2, &check));
	if (! check)
		return Result(ret, 0);
	else
		return equal_function_(cdr1, cdr2, ret);
}

static int equal_function_string_(addr a, addr b, int *ret)
{
	if (stringp(b))
		return string_equal_(a, b, ret);
	else
		return Result(ret, 0);
}

static int equal_function_bitvector_(addr a, addr b, int *ret)
{
	if (bitvectorp(b))
		return bitvector_equal_(a, b, ret);
	else
		return Result(ret, 0);
}

static int equal_function_array_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_ARRAY:
		case LISPTYPE_STRING:
			return equal_function_string_(b, a, ret);

		case LISPTYPE_BITVECTOR:
			return equal_function_bitvector_(b, a, ret);

		default:
			return Result(ret, 0);
	}
}

static int equal_function_pathname_(addr a, addr b, int *ret)
{
	if (! pathnamep(a))
		return Result(ret, 0);
	if (! pathnamep(b))
		return Result(ret, 0);

	return pathname_equal_(a, b, ret);
}

int equal_function_(addr a, addr b, int *ret)
{
	Check(a == Unbound, "Unbound-variable");
	Check(b == Unbound, "Unbound-variable");

	/* eql */
	if (eql_function(a, b))
		return Result(ret, 1);

	/* equal */
	switch (GetType(a)) {
		case LISPTYPE_CONS:
			return equal_function_cons_(a, b, ret);

		case LISPTYPE_STRING:
			return equal_function_string_(a, b, ret);

		case LISPTYPE_BITVECTOR:
			return equal_function_bitvector_(a, b, ret);

		case LISPTYPE_ARRAY:
			return equal_function_array_(a, b, ret);

		case LISPTYPE_PATHNAME:
			return equal_function_pathname_(a, b, ret);

		default:
			return Result(ret, 0);
	}
}


/*
 *  equalp
 */
static int equalp_function_number_(addr a, addr b, int *ret)
{
	if (numberp(b))
		return equal_number_(Local_Thread, a, b, ret);
	else
		return Result(ret, 0);
}

static int equalp_function_character_(addr a, addr b, int *ret)
{
	if (characterp(b))
		return Result(ret, character_equalp(a, b));
	else
		return Result(ret, 0);
}

static int equalp_function_cons_(addr a, addr b, int *ret)
{
	int check;
	addr car1, car2, cdr1, cdr2;

	if (! consp(b))
		return Result(ret, 0);
	GetCons(a, &car1, &cdr1);
	GetCons(b, &car2, &cdr2);
	Return(equalp_function_(car1, car2, &check));
	if (! check)
		return Result(ret, 0);
	else
		return equalp_function_(cdr1, cdr2, ret);
}

static int equalp_function_aa_(addr a, addr b, int *ret)
{
	int check;
	addr c, d;
	LocalRoot local;
	LocalStack stack;
	size_t size, i;

	/* rank, dimension */
	if (! array_equal_dimension(a, b))
		return Result(ret, 0);
	/* fill-pointer */
	array_get_rowlength(a, &size);
	array_get_rowlength(b, &i);
	if (size != i)
		return Result(ret, 0);
	/* body */
	local = Local_Thread;
	for (i = 0; i < size; i++) {
		push_local(local, &stack);
		Return(array_get_(local, a, i, &c));
		Return(array_get_(local, b, i, &d));
		Return(equalp_function_(c, d, &check));
		rollback_local(local, stack);
		if (! check)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalp_function_av_(addr a, addr b, int *ret)
{
	int check;
	addr c, d;
	LocalRoot local;
	LocalStack stack;
	size_t size, i;

	/* size check */
	if (! array_vector_p(a))
		return Result(ret, 0);
	array_get_rowlength(a, &size);
	lenarray(b, &i);
	if (size != i)
		return Result(ret, 0);
	/* body */
	local = Local_Thread;
	for (i = 0; i < size; i++) {
		push_local(local, &stack);
		Return(array_get_(local, a, i, &c));
		getarray(b, i, &d);
		Return(equalp_function_(c, d, &check));
		rollback_local(local, stack);
		if (! check)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalp_function_as_(addr a, addr b, int *ret)
{
	int check;
	unicode c, d;
	size_t size, i;

	/* string */
	if (array_stringp(a))
		return string_equalp_(a, b, ret);
	/* size check */
	if (! array_vector_p(a))
		return Result(ret, 0);
	array_get_rowlength(a, &size);
	strvect_length(b, &i);
	if (size != i)
		return Result(ret, 0);
	/* body */
	for (i = 0; i < size; i++) {
		Return(array_get_unicode_check_(a, i, &c, &check));
		if (! check)
			return Result(ret, 0);
		strvect_getc(b, i, &d);
		if (toUpperUnicode(c) != toUpperUnicode(d))
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalp_function_ab_(addr a, addr b, int *ret)
{
	int check;
	addr c, d;
	size_t size, i;

	/* string */
	if (array_bvarrayp(a))
		return bitvector_equal_(a, b, ret);
	/* size check */
	if (! array_vector_p(a))
		return Result(ret, 0);
	array_get_rowlength(a, &size);
	bitmemory_length(b, &i);
	if (size != i)
		return Result(ret, 0);
	/* body */
	for (i = 0; i < size; i++) {
		Return(array_get_(NULL, a, i, &c));
		Return(bitmemory_get_(NULL, b, i, &d));
		Return(equalp_function_(c, d, &check));
		if (! check)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalp_function_array_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_ARRAY:
			return equalp_function_aa_(b, a, ret);

		case LISPTYPE_VECTOR:
			return equalp_function_av_(a, b, ret);

		case LISPTYPE_STRING:
			return equalp_function_as_(a, b, ret);

		case LISPTYPE_BITVECTOR:
			return equalp_function_ab_(a, b, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalp_function_vv_(addr a, addr b, int *ret)
{
	int check;
	addr c, d;
	size_t size, i;

	lenarray(a, &size);
	lenarray(b, &i);
	if (size != i)
		return Result(ret, 0);
	for (i = 0; i < size; i++) {
		getarray(a, i, &c);
		getarray(b, i, &d);
		Return(equalp_function_(c, d, &check));
		if (! check)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalp_function_vs_(addr a, addr b, int *ret)
{
	addr c;
	unicode d, e;
	size_t size, i;

	lenarray(a, &size);
	strvect_length(b, &i);
	if (size != i)
		return Result(ret, 0);
	for (i = 0; i < size; i++) {
		getarray(a, i, &c);
		if (! characterp(c))
			return Result(ret, 0);
		GetCharacter(c, &d);
		strvect_getc(b, i, &e);
		if (toUpperUnicode(d) != toUpperUnicode(e))
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalp_function_vb_(addr a, addr b, int *ret)
{
	int d, e;
	addr c;
	size_t size, i;

	lenarray(a, &size);
	bitmemory_length(b, &i);
	if (size != i)
		return Result(ret, 0);
	for (i = 0; i < size; i++) {
		getarray(a, i, &c);
		if (bit_getint(c, &d))
			return Result(ret, 0);
		Return(bitmemory_getint_(b, i, &e));
		if (d != e)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalp_function_vector_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_VECTOR:
			return equalp_function_vv_(a, b, ret);

		case LISPTYPE_ARRAY:
			return equalp_function_av_(b, a, ret);

		case LISPTYPE_STRING:
			return equalp_function_vs_(a, b, ret);

		case LISPTYPE_BITVECTOR:
			return equalp_function_vb_(a, b, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalp_function_string_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_STRING:
			return Result(ret, strvect_equalp(a, b));

		case LISPTYPE_VECTOR:
			return equalp_function_vs_(b, a, ret);

		case LISPTYPE_ARRAY:
			return equalp_function_as_(b, a, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalp_function_bitvector_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_BITVECTOR:
			return Result(ret, bitmemory_equal(a, b));

		case LISPTYPE_VECTOR:
			return equalp_function_vb_(b, a, ret);

		case LISPTYPE_ARRAY:
			return equalp_function_ab_(b, a, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalp_function_hashtable_(addr a, addr b, int *ret)
{
	if (hashtablep(b))
		return equalp_hashtable_(a, b, ret);
	else
		return Result(ret, 0);
}

static int equalp_function_structure_(addr a, addr b, int *ret)
{
	int check;

	Return(structure_instance_p_(b, &check));
	if (check)
		return equalp_structure_(a, b, ret);
	else
		return Result(ret, 0);
}

int equalp_function_(addr a, addr b, int *ret)
{
	Check(a == Unbound, "Unbound-variable");
	Check(b == Unbound, "Unbound-variable");

	/* eq */
	if (a == b)
		return Result(ret, 1);

	/* number */
	if (numberp(a))
		return equalp_function_number_(a, b, ret);

	/* equalp */
	switch (GetType(a)) {
		case LISPTYPE_CHARACTER:
			return equalp_function_character_(a, b, ret);

		case LISPTYPE_CONS:
			return equalp_function_cons_(a, b, ret);

		case LISPTYPE_ARRAY:
			return equalp_function_array_(a, b, ret);

		case LISPTYPE_VECTOR:
			return equalp_function_vector_(a, b, ret);

		case LISPTYPE_STRING:
			return equalp_function_string_(a, b, ret);

		case LISPTYPE_BITVECTOR:
			return equalp_function_bitvector_(a, b, ret);

		case LISPTYPE_PATHNAME:
			return equal_function_pathname_(a, b, ret);

		case LISPTYPE_HASHTABLE:
			return equalp_function_hashtable_(a, b, ret);

		case LISPTYPE_CLOS:
			return equalp_function_structure_(a, b, ret);

		default:
			return Result(ret, 0);
	}
}


/*
 *  equal for rt
 */
static int equalrt_function_character_(addr a, addr b, int *ret)
{
	if (characterp(b))
		return Result(ret, character_equal(a, b));
	else
		return Result(ret, 0);
}

static int equalrt_function_cons_(addr a, addr b, int *ret)
{
	int check;
	addr car1, car2, cdr1, cdr2;

	if (! consp(b))
		return Result(ret, 0);
	GetCons(a, &car1, &cdr1);
	GetCons(b, &car2, &cdr2);
	Return(equalrt_function_(car1, car2, &check));
	if (! check)
		return Result(ret, 0);
	else
		return equalrt_function_(cdr1, cdr2, ret);
}

static int equalrt_function_aa_(addr a, addr b, int *ret)
{
	int check;
	addr c, d;
	LocalRoot local;
	LocalStack stack;
	size_t size, i;

	/* rank, dimension */
	if (! array_equal_dimension(a, b))
		return Result(ret, 0);
	/* fill-pointer */
	array_get_rowlength(a, &size);
	array_get_rowlength(b, &i);
	if (size != i)
		return Result(ret, 0);
	/* body */
	local = Local_Thread;
	for (i = 0; i < size; i++) {
		push_local(local, &stack);
		Return(array_get_(local, a, i, &c));
		Return(array_get_(local, b, i, &d));
		Return(equalrt_function_(c, d, &check));
		rollback_local(local, stack);
		if (! check)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalrt_function_av_(addr a, addr b, int *ret)
{
	int check;
	addr c, d;
	LocalRoot local;
	LocalStack stack;
	size_t size, i;

	/* size check */
	if (! array_vector_p(a))
		return Result(ret, 0);
	array_get_rowlength(a, &size);
	lenarray(b, &i);
	if (size != i)
		return Result(ret, 0);
	/* body */
	local = Local_Thread;
	for (i = 0; i < size; i++) {
		push_local(local, &stack);
		Return(array_get_(local, a, i, &c));
		getarray(b, i, &d);
		Return(equalrt_function_(c, d, &check));
		rollback_local(local, stack);
		if (! check)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalrt_function_as_(addr a, addr b, int *ret)
{
	int check;
	unicode c, d;
	size_t size, i;

	/* string */
	if (array_stringp(a))
		return string_equal_(a, b, ret);
	/* size check */
	if (! array_vector_p(a))
		return Result(ret, 0);
	array_get_rowlength(a, &size);
	strvect_length(b, &i);
	if (size != i)
		return Result(ret, 0);
	/* body */
	for (i = 0; i < size; i++) {
		Return(array_get_unicode_check_(a, i, &c, &check));
		if (! check)
			return Result(ret, 0);
		Return(array_get_unicode_(a, i, &c));
		strvect_getc(b, i, &d);
		if (c != d)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalrt_function_array_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_ARRAY:
			return equalrt_function_aa_(b, a, ret);

		case LISPTYPE_VECTOR:
			return equalrt_function_av_(a, b, ret);

		case LISPTYPE_STRING:
			return equalrt_function_as_(a, b, ret);

		case LISPTYPE_BITVECTOR:
			return equalp_function_ab_(a, b, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalrt_function_vv_(addr a, addr b, int *ret)
{
	int check;
	addr c, d;
	size_t size, i;

	lenarray(a, &size);
	lenarray(b, &i);
	if (size != i)
		return Result(ret, 0);
	for (i = 0; i < size; i++) {
		getarray(a, i, &c);
		getarray(b, i, &d);
		Return(equalrt_function_(c, d, &check));
		if (! check)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalrt_function_vs_(addr a, addr b, int *ret)
{
	addr c;
	unicode d, e;
	size_t size, i;

	lenarray(a, &size);
	strvect_length(b, &i);
	if (size != i)
		return Result(ret, 0);
	for (i = 0; i < size; i++) {
		getarray(a, i, &c);
		if (! characterp(c))
			return Result(ret, 0);
		GetCharacter(c, &d);
		strvect_getc(b, i, &e);
		if (d != e)
			return Result(ret, 0);
	}

	return Result(ret, 1);
}

static int equalrt_function_vector_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_VECTOR:
			return equalrt_function_vv_(a, b, ret);

		case LISPTYPE_STRING:
			return equalrt_function_vs_(a, b, ret);

		case LISPTYPE_ARRAY:
			return equalrt_function_av_(b, a, ret);

		case LISPTYPE_BITVECTOR:
			return equalp_function_vb_(a, b, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalrt_function_string_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_STRING:
			return Result(ret, strvect_equal(a, b));

		case LISPTYPE_VECTOR:
			return equalrt_function_vs_(b, a, ret);

		case LISPTYPE_ARRAY:
			return equalrt_function_as_(b, a, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalrt_function_bitvector_(addr a, addr b, int *ret)
{
	switch (GetType(b)) {
		case LISPTYPE_BITVECTOR:
			return Result(ret, bitmemory_equal(a, b));

		case LISPTYPE_VECTOR:
			return equalp_function_vb_(b, a, ret);

		case LISPTYPE_ARRAY:
			return equalp_function_ab_(b, a, ret);

		default:
			return Result(ret, 0);
	}
}

static int equalrt_function_hashtable_(addr a, addr b, int *ret)
{
	if (hashtablep(b))
		return equalrt_hashtable_(a, b, ret);
	else
		return Result(ret, 0);
}

static int equalrt_function_structure_(addr a, addr b, int *ret)
{
	int check;

	Return(structure_instance_p_(b, &check));
	if (check)
		return equalrt_structure_(a, b, ret);
	else
		return Result(ret, 0);
}

int equalrt_function_(addr a, addr b, int *ret)
{
	Check(a == Unbound, "Unbound-variable");
	Check(b == Unbound, "Unbound-variable");

	/* eql */
	if (eql_function(a, b))
		return Result(ret, 1);

	/* equalrt */
	switch (GetType(a)) {
		case LISPTYPE_CHARACTER:
			return equalrt_function_character_(a, b, ret);

		case LISPTYPE_CONS:
			return equalrt_function_cons_(a, b, ret);

		case LISPTYPE_ARRAY:
			return equalrt_function_array_(a, b, ret);

		case LISPTYPE_VECTOR:
			return equalrt_function_vector_(a, b, ret);

		case LISPTYPE_STRING:
			return equalrt_function_string_(a, b, ret);

		case LISPTYPE_BITVECTOR:
			return equalrt_function_bitvector_(a, b, ret);

		case LISPTYPE_PATHNAME:
			return equal_function_pathname_(a, b, ret);

		case LISPTYPE_HASHTABLE:
			return equalrt_function_hashtable_(a, b, ret);

		case LISPTYPE_CLOS:
			return equalrt_function_structure_(a, b, ret);

		default:
			return Result(ret, 0);
	}
}


/*
 *  for debug
 */
int equal_debug(addr left, addr right)
{
	int check;
	check = 0;
	Error(equal_function_(left, right, &check));
	return check;
}

int equalp_debug(addr left, addr right)
{
	int check;
	check = 0;
	Error(equalp_function_(left, right, &check));
	return check;
}

int equalrt_debug(addr left, addr right)
{
	int check;
	check = 0;
	Error(equalrt_function_(left, right, &check));
	return check;
}

