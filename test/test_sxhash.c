#include "character.h"
#include "constant.h"
#include "degrade.h"
#include "object.h"
#include "strvect.h"
#include "sxhash.c"

static int test_sxfixed_eq(void)
{
	addr cons1, cons2;

	consnil_heap(&cons1);
	consnil_heap(&cons2);
	test(sxfixed_eq(cons1, -1) == sxfixed_eq(cons1, -1), "sxfixed_eq.1");
	test(sxfixed_eq(cons1, -1) != sxfixed_eq(cons2, -1), "sxfixed_eq.2");

	RETURN;
}

static int test_sxfixed_cons(void)
{
	addr cons1, cons2, cons3, cons4, v1, v2;

	consnil_heap(&cons1);
	consnil_heap(&cons2);
	test(sxfixed_cons(cons1, -1) == sxfixed_cons(cons2, -1), "sxfixed_cons.1");
	SetCons(cons1, T, Nil);
	test(sxfixed_cons(cons1, -1) != sxfixed_cons(cons2, -1), "sxfixed_cons.2");
	test(sxfixed_cons(cons1, 0) == sxfixed_cons(cons2, 0), "sxfixed_cons.3");
	test(sxfixed_cons(cons1, 1) != sxfixed_cons(cons2, 1), "sxfixed_cons.4");

	fixnum_heap(&v1, 100);
	fixnum_heap(&v2, 200);
	SetCons(cons1, Nil, v1);
	SetCons(cons2, Nil, v2);
	test(sxfixed_cons(cons1, -1) != sxfixed_cons(cons2, -1), "sxfixed_cons.5");
	SetStatusValue(v2, LISPSTATUS_READONLY, 0);
	SetFixnum(v2, 100);
	test(sxfixed_cons(cons1, -1) == sxfixed_cons(cons2, -1), "sxfixed_cons.6");

	consnil_heap(&cons3);
	consnil_heap(&cons4);
	SetFixnum(v2, 200);
	SetCons(cons1, cons3, Nil);
	SetCons(cons2, cons4, Nil);
	SetCons(cons3, T, v1);
	SetCons(cons4, T, v2);
	test(sxfixed_cons(cons1, -1) != sxfixed_cons(cons2, -1), "sxfixed_cons.7");
	test(sxfixed_cons(cons1, 0) == sxfixed_cons(cons2, 0), "sxfixed_cons.8");
	test(sxfixed_cons(cons1, 1) == sxfixed_cons(cons2, 1), "sxfixed_cons.9");
	test(sxfixed_cons(cons1, 2) != sxfixed_cons(cons2, 2), "sxfixed_cons.10");

	SetFixnum(v2, 100);
	test(sxfixed_cons(cons1, -1) == sxfixed_cons(cons2, -1), "sxfixed_cons.11");
	test(sxfixed_cons(cons1, 0) == sxfixed_cons(cons2, 0), "sxfixed_cons.12");
	test(sxfixed_cons(cons1, 1) == sxfixed_cons(cons2, 1), "sxfixed_cons.13");
	test(sxfixed_cons(cons1, 2) == sxfixed_cons(cons2, 2), "sxfixed_cons.14");

	RETURN;
}

static int test_sxfixed_vector(void)
{
	addr pos;

	vector2_heap(&pos, 100);
	test(sxfixed_vector(pos, -1) == 100, "sxfixed_vector.1");

	RETURN;
}

static int test_sxfixed_character(void)
{
	addr pos;

	character_heap(&pos, 'a');
	test(sxfixed_character(pos, -1) == 'a', "sxfixed_character.1");
	character_heap(&pos, 10000);
	test(sxfixed_character(pos, -1) == 10000, "sxfixed_character.2");

	RETURN;
}

static int test_sxfixed_string(void)
{
	addr pos;
	fixed v1, v2;

	strvect_char_heap(&pos, "");
	v1 = sxfixed_string(pos, -1);
	strvect_char_heap(&pos, "");
	v2 = sxfixed_string(pos, -1);
	test(v1 == v2, "sxfixed_string.1");

	strvect_char_heap(&pos, "a");
	v2 = sxfixed_string(pos, -1);
	test(v1 != v2, "sxfixed_string.2");

	strvect_char_heap(&pos, "a");
	v1 = sxfixed_string(pos, -1);
	test(v1 == v2, "sxfixed_string.3");

	strvect_char_heap(&pos, "A");
	v2 = sxfixed_string(pos, -1);
	test(v1 != v2, "sxfixed_string.4");

	strvect_char_heap(&pos, "HelloAAAABBB");
	v1 = sxfixed_string(pos, -1);
	test(v1 != v2, "sxfixed_string.5");

	strvect_char_heap(&pos, "zzzyyy");
	v2 = sxfixed_string(pos, -1);
	test(v1 != v2, "sxfixed_string.6");

	strvect_char_heap(&pos, "HelloAAAABBBB");
	v2 = sxfixed_string(pos, -1);
	test(v1 != v2, "sxfixed_string.7");

	strvect_char_heap(&pos, "HelloAAAABBB");
	v2 = sxfixed_string(pos, -1);
	test(v1 == v2, "sxfixed_string.8");

	RETURN;
}

static int test_sxfixed_fixnum(void)
{
	addr pos;
	fixed v1, v2;

	fixnum_heap(&pos, 100);
	test(sxfixed_fixnum(pos, -1) == 100, "sxfixed_fixnum.1");
	fixnum_heap(&pos, -10);
	v1 = sxfixed_fixnum(pos, -1);
	fixnum_heap(&pos, -11);
	v2 = sxfixed_fixnum(pos, -1);
	test(v1 != v2, "sxfixed_fixnum.2");

	RETURN;
}


/*
 *  sxhash
 */
static int testcase_sxhash(void)
{
	TestBreak(test_sxfixed_eq);
	TestBreak(test_sxfixed_cons);
	TestBreak(test_sxfixed_vector);
	TestBreak(test_sxfixed_character);
	TestBreak(test_sxfixed_string);
	TestBreak(test_sxfixed_fixnum);

	return 0;
}

static void testinit_sxhash(Execute ptr)
{
	build_lisproot(ptr);
	build_constant();
	build_object();
	build_character();
}

int test_sxhash(void)
{
	TITLE;
	return degrade_code(
			testinit_sxhash,
			testcase_sxhash);
}

