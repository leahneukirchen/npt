/*
 *  ANSI COMMON LISP: 17. Sequences
 */
#include "array_access.h"
#include "array_copy.h"
#include "array_make.h"
#include "array_sequence.h"
#include "bit.h"
#include "call_sequences.h"
#include "common_header.h"
#include "cons.h"
#include "cons_plist.h"
#include "integer.h"
#include "number.h"
#include "sequence.h"
#include "sequence_count.h"
#include "sequence_duplicates.h"
#include "sequence_find.h"
#include "sequence_make.h"
#include "sequence_map.h"
#include "sequence_merge.h"
#include "sequence_remove.h"
#include "sequence_substitute.h"
#include "sort.h"
#include "strtype.h"
#include "type_parse.h"

/* (defun copy-seq (sequence) ...) -> sequence */
static int function_copy_seq(Execute ptr, addr var)
{
	Return(copy_seq_common_(var, &var));
	setresult_control(ptr, var);
	return 0;
}

static void type_copy_seq(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, Sequence);
	typeargs_var1(&args, args);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_copy_seq(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_COPY_SEQ, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var1(pos, p_defun_copy_seq);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_copy_seq(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun elt (sequence index) ...) -> t */
static int function_elt(Execute ptr, addr var, addr index)
{
	Return(elt_common_(var, index, &var));
	setresult_control(ptr, var);
	return 0;
}

static void type_elt(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, Sequence);
	GetTypeTable(&values, Index);
	typeargs_var2(&args, args, values);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void defun_elt(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_ELT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2(pos, p_defun_elt);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_elt(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun (setf elt) (value sequence index) ...) -> t */
static int function_setf_elt(Execute ptr, addr value, addr pos, addr index)
{
	Return(setf_elt_common_(value, pos, index));
	setresult_control(ptr, value);
	return 0;
}

static void type_setf_elt(addr *ret)
{
	addr args, values, type;

	GetTypeTable(&args, Sequence);
	GetTypeTable(&values, Index);
	GetTypeTable(&type, T);
	typeargs_var3(&args, type, args, values);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void defun_setf_elt(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_ELT, &symbol);
	compiled_setf_system(&pos, symbol);
	setcompiled_var3(pos, p_defun_setf_elt);
	setsetf_symbol(symbol, pos);
	/* type */
	type_setf_elt(&type);
	settype_function(pos, type);
	settype_setf_symbol(symbol, type);
}


/* (defun fill (sequence item &key start end) ...) -> sequence
 *    item   t
 *    start  keyword-start
 *    end    keyword-end
 */
static int function_fill(Execute ptr, addr var, addr item, addr rest)
{
	Return(fill_common_(var, item, rest));
	setresult_control(ptr, var);
	return 0;
}

static void type_fill(addr *ret)
{
	addr args, values, key1, key2;

	/* key */
	GetConst(KEYWORD_START, &key1);
	GetTypeTable(&values, KeywordStart);
	cons_heap(&key1, key1, values);
	GetConst(KEYWORD_END, &key2);
	GetTypeTable(&values, KeywordEnd);
	cons_heap(&key2, key2, values);
	list_heap(&key1, key1, key2, NULL);
	/* type */
	GetTypeTable(&args, Sequence);
	GetTypeTable(&values, T);
	typeargs_var2key(&args, args, values, key1);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_fill(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_FILL, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_fill);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_fill(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun make-sequence (type size &key initial-element) ...) -> sequence */
static int function_make_sequence(Execute ptr, addr type, addr size, addr rest)
{
	Return(make_sequence_common_(ptr, &type, type, size, rest));
	setresult_control(ptr, type);
	return 0;
}

static void type_make_sequence(addr *ret)
{
	addr args, values, key;

	/* key */
	GetConst(KEYWORD_INITIAL_ELEMENT, &key);
	GetTypeTable(&values, T);
	cons_heap(&key, key, values);
	list_heap(&key, key, NULL);
	/* type */
	GetTypeTable(&args, TypeSpec);
	GetTypeTable(&values, Index);
	typeargs_var2key(&args, args, values, key);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_make_sequence(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_MAKE_SEQUENCE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_make_sequence);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_make_sequence(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun subseq (sequence start &optional end) ...) -> sequence
 *   start  keyword-start
 *   end    keyword-end
 */
static int function_subseq(Execute ptr, addr var, addr start, addr end)
{
	Return(subseq_common_(var, start, end, &var));
	setresult_control(ptr, var);
	return 0;
}

static void type_subseq(addr *ret)
{
	addr args, values, type;

	GetTypeTable(&args, Sequence);
	GetTypeTable(&values, KeywordStart);
	GetTypeTable(&type, KeywordEnd);
	typeargs_var2opt1(&args, args, values, type);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_subseq(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_SUBSEQ, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2opt1(pos, p_defun_subseq);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_subseq(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun (setf subseq) (sequence sequence start &optional end) ...) -> sequence */
static int function_setf_subseq(Execute ptr,
		addr pos, addr root, addr start, addr end)
{
	Return(setf_subseq_common_(root, pos, start, end));
	setresult_control(ptr, pos);
	return 0;
}

static void type_setf_subseq(addr *ret)
{
	addr args, values, type;

	GetTypeTable(&args, Sequence);
	GetTypeTable(&values, KeywordStart);
	GetTypeTable(&type, KeywordEnd);
	typeargs_var3opt1(&args, args, args, values, type);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_setf_subseq(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_SUBSEQ, &symbol);
	compiled_setf_system(&pos, symbol);
	setcompiled_var3opt1(pos, p_defun_setf_subseq);
	setsetf_symbol(symbol, pos);
	/* type */
	type_setf_subseq(&type);
	settype_function(pos, type);
	settype_setf_symbol(symbol, type);
}


/* (defun map (type function sequence &rest sequence) ...) -> sequence
 *   type      type-spec
 *   function  function-designer
 *   result    sequence
 */
static int function_map(Execute ptr, addr type, addr call, addr rest)
{
	Return(map_common_(ptr, &type, type, call, rest));
	setresult_control(ptr, type);
	return 0;
}

static void type_map(addr *ret)
{
	addr args, values, type;

	GetTypeTable(&args, TypeSpec);
	GetTypeTable(&values, FunctionDesigner);
	GetTypeTable(&type, Sequence);
	typeargs_var3rest(&args, args, values, type, type);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_map(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_MAP, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_map);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_map(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun map-into (sequence funcion &rest sequence) ...) -> sequence */
static int function_map_into(Execute ptr, addr var, addr call, addr rest)
{
	Return(map_into_common_(ptr, var, call, rest));
	setresult_control(ptr, var);
	return 0;
}

static void type_map_into(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, Sequence);
	GetTypeTable(&values, FunctionDesigner);
	typeargs_var2rest(&args, args, values, args);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_map_into(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_MAP_INTO, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_map_into);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_map_into(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun reduce (function sequence &key key from-end end initial-value) ...) -> t
 *   key            function-designer  ;; (or function-designer null)
 *   from-end       t  ;; boolean
 *   start          keyword-start
 *   end            keyword-end
 *   initial-value  t
 */
static int function_reduce(Execute ptr, addr call, addr pos, addr rest)
{
	Return(reduce_common_(ptr, &call, call, pos, rest));
	setresult_control(ptr, call);
	return 0;
}

static void type_reduce(addr *ret)
{
	addr args, values, type, key1, key2, key3, key4, key5;

	/* key */
	GetConst(KEYWORD_KEY, &key1);
	GetTypeTable(&type, FunctionDesigner);
	cons_heap(&key1, key1, type);
	GetConst(KEYWORD_FROM_END, &key2);
	GetTypeTable(&type, T);
	cons_heap(&key2, key2, type);
	GetConst(KEYWORD_START, &key3);
	GetTypeTable(&type, KeywordStart);
	cons_heap(&key3, key3, type);
	GetConst(KEYWORD_END, &key4);
	GetTypeTable(&type, KeywordEnd);
	cons_heap(&key4, key4, type);
	GetConst(KEYWORD_INITIAL_VALUE, &key5);
	GetTypeTable(&type, T);
	cons_heap(&key5, key5, type);
	list_heap(&type, key1, key2, key3, key4, key5, NULL);
	/* type */
	GetTypeTable(&args, FunctionDesigner);
	GetTypeTable(&values, Sequence);
	typeargs_var2key(&args, args, values, type);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void defun_reduce(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_REDUCE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_reduce);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_reduce(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun count (item sequence
 *     &key from-end start end key test test-not) ...) -> n
 *   from-end  t  ;; boolean
 *   start     keyword-start
 *   end       keyword-end
 *   key       function-designer
 *   test      function-designer
 *   test-not  function-designer
 */
static int function_count(Execute ptr, addr item, addr pos, addr rest)
{
	Return(count_common_(ptr, &item, item, pos, rest));
	setresult_control(ptr, item);
	return 0;
}

static void type_count(addr *ret)
{
	addr args, values, key;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Sequence);
	GetTypeTable(&key, CountKey);
	typeargs_var2key(&args, args, values, key);
	GetTypeValues(&values, Index);
	type_compiled_heap(args, values, ret);
}

static void defun_count(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_COUNT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_count);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_count(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun count-if (call sequence &key from-end start end key) ...) -> n */
static int function_count_if(Execute ptr, addr call, addr var, addr rest)
{
	Return(count_if_common_(ptr, &call, call, var, rest));
	setresult_control(ptr, call);
	return 0;
}

static void defun_count_if(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_COUNT_IF, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_count_if);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, CountIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun count-if (call sequence &key from-end start end key) ...) -> n */
static int function_count_if_not(Execute ptr, addr call, addr var, addr rest)
{
	Return(count_if_not_common_(ptr, &call, call, var, rest));
	setresult_control(ptr, call);
	return 0;
}

static void defun_count_if_not(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_COUNT_IF_NOT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_count_if_not);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, CountIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun length (sequence) ...) -> index */
static int function_length(Execute ptr, addr var)
{
	size_t size;

	Return(length_sequence_(var, 1, &size));
	setresult_control(ptr, intsizeh(size));

	return 0;
}

static void type_length(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, Sequence);
	typeargs_var1(&args, args);
	GetTypeValues(&values, Index);
	type_compiled_heap(args, values, ret);
}

static void defun_length(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_LENGTH, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var1(pos, p_defun_length);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_length(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun reverse (sequence) ...) -> sequence */
static int function_reverse(Execute ptr, addr var)
{
	Return(reverse_sequence_heap_(&var, var));
	setresult_control(ptr, var);
	return 0;
}

static void defun_reverse(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_REVERSE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var1(pos, p_defun_reverse);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Reverse);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun nreverse (sequence) ...) -> sequence */
static int function_nreverse(Execute ptr, addr var)
{
	Return(nreverse_sequence_(&var, var));
	setresult_control(ptr, var);
	return 0;
}

static void defun_nreverse(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_NREVERSE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var1(pos, p_defun_nreverse);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Reverse);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun merge (type sequence1 sequence2 call &key key) ...) -> sequence */
static int function_merge(Execute ptr,
		addr type, addr pos1, addr pos2, addr call, addr rest)
{
	Return(merge_common_(ptr, type, pos1, pos2, call, rest, &pos1));
	setresult_control(ptr, pos1);
	return 0;
}

static void type_merge(addr *ret)
{
	addr args, values, type, key;

	/* key */
	GetConst(KEYWORD_KEY, &key);
	GetTypeTable(&type, FunctionDesigner);
	cons_heap(&key, key, type);
	conscar_heap(&key, key);
	/* type */
	GetTypeTable(&args, TypeSpec);
	GetTypeTable(&values, Sequence);
	typeargs_var4key(&args, args, values, values, type, key);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_merge(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_MERGE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var4dynamic(pos, p_defun_merge);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_merge(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun sort (sequence call &key key) ...) -> sequence */
static int function_sort(Execute ptr, addr pos, addr call, addr rest)
{
	Return(sort_common_(ptr, pos, call, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_sort(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_SORT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_sort);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Sort);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun stable-sort (sequence call &key key) ...) -> sequence */
static int function_stable_sort(Execute ptr, addr pos, addr call, addr rest)
{
	Return(stable_sort_common_(ptr, pos, call, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_stable_sort(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_STABLE_SORT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_stable_sort);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Sort);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun find (item sequence
 *     &key from-end test test-not start end key) ...) -> t
 */
static int function_find(Execute ptr, addr item, addr pos, addr rest)
{
	Return(find_common_(ptr, &pos, item, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void type_find(addr *ret)
{
	addr args, values, key;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Sequence);
	GetTypeTable(&key, CountKey);
	typeargs_var2key(&args, args, values, key);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void defun_find(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_FIND, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_find);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_find(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun find-if (call sequence &key from-end start end key) ...) -> t */
static int function_find_if(Execute ptr, addr call, addr pos, addr rest)
{
	Return(find_if_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_find_if(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_FIND_IF, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_find_if);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, FindIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun find-if-not (call sequence &key from-end start end key) ...) -> t */
static int function_find_if_not(Execute ptr, addr call, addr pos, addr rest)
{
	Return(find_if_not_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_find_if_not(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_FIND_IF_NOT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_find_if_not);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, FindIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun position (item sequence
 *     &key from-end test test-not start end key) ...) -> t
 */
static int function_position(Execute ptr, addr item, addr pos, addr rest)
{
	Return(position_common_(ptr, &pos, item, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void type_position(addr *ret)
{
	addr args, values, key;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Sequence);
	GetTypeTable(&key, CountKey);
	typeargs_var2key(&args, args, values, key);
	GetTypeValues(&values, IndexNull);
	type_compiled_heap(args, values, ret);
}

static void defun_position(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_POSITION, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_position);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_position(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun position-if (call sequence &key from-end start end key) ...) -> t */
static int function_position_if(Execute ptr, addr call, addr pos, addr rest)
{
	Return(position_if_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_position_if(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_POSITION_IF, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_position_if);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, PositionIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun position-if-not (call sequence &key from-end start end key) ...) -> t */
static int function_position_if_not(Execute ptr, addr call, addr pos, addr rest)
{
	Return(position_if_not_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_position_if_not(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_POSITION_IF_NOT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_position_if_not);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, PositionIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun search (sequence1 sequence2
 *     &key from-end test test-not key start1 start2 end1 end2) ...) -> position
 */
static int function_search(Execute ptr, addr pos1, addr pos2, addr rest)
{
	Return(search_common_(ptr, &pos1, pos1, pos2, rest));
	setresult_control(ptr, pos1);
	return 0;
}

static void defun_search(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_SEARCH, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_search);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Search);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun mismatch (sequence1 sequence2
 *     &key from-end test test-not key start1 start2 end1 end2) ...) -> position
 */
static int function_mismatch(Execute ptr, addr pos1, addr pos2, addr rest)
{
	Return(mismatch_common_(ptr, &pos1, pos1, pos2, rest));
	setresult_control(ptr, pos1);
	return 0;
}

static void defun_mismatch(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_MISMATCH, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_mismatch);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Search);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun replace (sequence1 sequence2 &ekey start1 end1 start2 end2) ...)
 *     -> sequence1
 */
static int function_replace(Execute ptr, addr pos1, addr pos2, addr rest)
{
	Return(replace_common_(ptr, pos1, pos2, rest));
	setresult_control(ptr, pos1);
	return 0;
}

static void type_replace(addr *ret)
{
	addr args, values, key, key1, key2, key3, key4;

	/* key */
	KeyTypeTable(&key1, START1, KeywordStart);
	KeyTypeTable(&key2, START2, KeywordStart);
	KeyTypeTable(&key3, END1, KeywordEnd);
	KeyTypeTable(&key4, END2, KeywordEnd);
	list_heap(&key, key1, key2, key3, key4, NULL);

	/* type */
	GetTypeTable(&args, Sequence);
	typeargs_var2key(&args, args, args, key);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_replace(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_REPLACE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_replace);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_replace(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun substitute (newitem olditem sequence
 *     &key from-end test test-not start end count key) ...)
 *   -> sequence
 *   count  (or (integer 0) null)
 */
static int function_substitute(Execute ptr,
		addr item1, addr item2, addr pos, addr rest)
{
	Return(substitute_common_(ptr, &pos, item1, item2, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_substitute(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_SUBSTITUTE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var3dynamic(pos, p_defun_substitute);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Substitute);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun substitute-if (item call sequence
 *     &key from-end start end count key) ...)
 *   -> sequence
 *   count  (or (integer 0) null)
 */
static int function_substitute_if(Execute ptr,
		addr item, addr call, addr pos, addr rest)
{
	Return(substitute_if_common_(ptr, &pos, item, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_substitute_if(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_SUBSTITUTE_IF, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var3dynamic(pos, p_defun_substitute_if);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, SubstituteIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun substitute-if-not (item call sequence
 *     &key from-end start end count key) ...)
 *   -> sequence
 *   count  (or (integer 0) null)
 */
static int function_substitute_if_not(Execute ptr,
		addr item, addr call, addr pos, addr rest)
{
	Return(substitute_if_not_common_(ptr, &pos, item, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_substitute_if_not(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_SUBSTITUTE_IF_NOT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var3dynamic(pos, p_defun_substitute_if_not);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, SubstituteIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun nsubstitute (newitem olditem sequence
 *     &key from-end test test-not start end count key) ...)
 *   -> sequence
 *   count  (or (integer 0) null)
 */
static int function_nsubstitute(Execute ptr,
		addr item1, addr item2, addr pos, addr rest)
{
	Return(nsubstitute_common_(ptr, item1, item2, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_nsubstitute(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_NSUBSTITUTE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var3dynamic(pos, p_defun_nsubstitute);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Substitute);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun nsubstitute-if (item call sequence
 *     &key from-end start end count key) ...)
 *   -> sequence
 *   count  (or (integer 0) null)
 */
static int function_nsubstitute_if(Execute ptr,
		addr item, addr call, addr pos, addr rest)
{
	Return(nsubstitute_if_common_(ptr, item, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_nsubstitute_if(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_NSUBSTITUTE_IF, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var3dynamic(pos, p_defun_nsubstitute_if);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, SubstituteIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun nsubstitute-if-not (item call sequence
 *     &key from-end start end count key) ...)
 *   -> sequence
 *   count  (or (integer 0) null)
 */
static int function_nsubstitute_if_not(Execute ptr,
		addr item, addr call, addr pos, addr rest)
{
	Return(nsubstitute_if_not_common_(ptr, item, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_nsubstitute_if_not(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_NSUBSTITUTE_IF_NOT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var3dynamic(pos, p_defun_nsubstitute_if_not);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, SubstituteIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun concatenate (typespec &rest sequences) ...) -> result
 *   typespec   typespec
 *   sequences  &rest sequence
 *   result     sequence
 */
static int function_concatenate(Execute ptr, addr type, addr rest)
{
	Return(concatenate_common_(ptr, &type, type, rest));
	setresult_control(ptr, type);
	return 0;
}

static void type_concatenate(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, TypeSpec);
	GetTypeTable(&values, Sequence);
	typeargs_var1rest(&args, args, values);
	GetTypeValues(&values, Sequence);
	type_compiled_heap(args, values, ret);
}

static void defun_concatenate(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_CONCATENATE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var1dynamic(pos, p_defun_concatenate);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_concatenate(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun remove (item sequence
 *     &key from-end test test-not start end count key) ...) -> sequence
 */
static int function_remove(Execute ptr, addr item, addr pos, addr rest)
{
	Return(remove_common_(ptr, &pos, item, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_remove(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_REMOVE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_remove);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Remove);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun remove-if (test sequence
 *     &key from-end start end count key) ...) -> sequence
 */
static int function_remove_if(Execute ptr, addr call, addr pos, addr rest)
{
	Return(remove_if_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_remove_if(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_REMOVE_IF, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_remove_if);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, RemoveIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun remove-if-not (test sequence
 *     &key from-end start end count key) ...) -> sequence
 */
static int function_remove_if_not(Execute ptr, addr call, addr pos, addr rest)
{
	Return(remove_if_not_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_remove_if_not(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_REMOVE_IF_NOT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_remove_if_not);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, RemoveIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun delete (item sequence
 *     &key from-end test test-not start end count key) ...) -> sequence
 */
static int function_delete(Execute ptr, addr item, addr pos, addr rest)
{
	Return(delete_common_(ptr, &pos, item, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_delete(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_DELETE, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_delete);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, Remove);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun delete-if (test sequence
 *     &key from-end start end count key) ...) -> sequence
 */
static int function_delete_if(Execute ptr, addr call, addr pos, addr rest)
{
	Return(delete_if_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_delete_if(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_DELETE_IF, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_delete_if);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, RemoveIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun delete-if-not (test sequence
 *     &key from-end start end count key) ...) -> sequence
 */
static int function_delete_if_not(Execute ptr, addr call, addr pos, addr rest)
{
	Return(delete_if_not_common_(ptr, &pos, call, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_delete_if_not(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_DELETE_IF_NOT, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var2dynamic(pos, p_defun_delete_if_not);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, RemoveIf);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun remove-duplicates (sequence
 *     &key from-end test test-not start end key) ...) -> sequence
 */
static int function_remove_duplicates(Execute ptr, addr pos, addr rest)
{
	Return(remove_duplicates_common_(ptr, &pos, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_remove_duplicates(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_REMOVE_DUPLICATES, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var1dynamic(pos, p_defun_remove_duplicates);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, RemoveDuplicates);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun delete-duplicates (sequence
 *     &key from-end test test-not start end key) ...) -> sequence
 */
static int function_delete_duplicates(Execute ptr, addr pos, addr rest)
{
	Return(delete_duplicates_common_(ptr, &pos, pos, rest));
	setresult_control(ptr, pos);
	return 0;
}

static void defun_delete_duplicates(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_DELETE_DUPLICATES, &symbol);
	compiled_system(&pos, symbol);
	setcompiled_var1dynamic(pos, p_defun_delete_duplicates);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetTypeCompiled(&type, RemoveDuplicates);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/*
 *  function
 */
void init_common_sequences(void)
{
	SetPointerCall(defun, var1, copy_seq);
	SetPointerCall(defun, var2, elt);
	SetPointerCall(defun, var3, setf_elt);
	SetPointerCall(defun, var2dynamic, fill);
	SetPointerCall(defun, var2dynamic, make_sequence);
	SetPointerCall(defun, var2opt1, subseq);
	SetPointerCall(defun, var3opt1, setf_subseq);
	SetPointerCall(defun, var2dynamic, map);
	SetPointerCall(defun, var2dynamic, map_into);
	SetPointerCall(defun, var2dynamic, reduce);
	SetPointerCall(defun, var2dynamic, count);
	SetPointerCall(defun, var2dynamic, count_if);
	SetPointerCall(defun, var2dynamic, count_if_not);
	SetPointerCall(defun, var1, length);
	SetPointerCall(defun, var1, reverse);
	SetPointerCall(defun, var1, nreverse);
	SetPointerCall(defun, var4dynamic, merge);
	SetPointerCall(defun, var2dynamic, sort);
	SetPointerCall(defun, var2dynamic, stable_sort);
	SetPointerCall(defun, var2dynamic, find);
	SetPointerCall(defun, var2dynamic, find_if);
	SetPointerCall(defun, var2dynamic, find_if_not);
	SetPointerCall(defun, var2dynamic, position);
	SetPointerCall(defun, var2dynamic, position_if);
	SetPointerCall(defun, var2dynamic, position_if_not);
	SetPointerCall(defun, var2dynamic, search);
	SetPointerCall(defun, var2dynamic, mismatch);
	SetPointerCall(defun, var2dynamic, replace);
	SetPointerCall(defun, var3dynamic, substitute);
	SetPointerCall(defun, var3dynamic, substitute_if);
	SetPointerCall(defun, var3dynamic, substitute_if_not);
	SetPointerCall(defun, var3dynamic, nsubstitute);
	SetPointerCall(defun, var3dynamic, nsubstitute_if);
	SetPointerCall(defun, var3dynamic, nsubstitute_if_not);
	SetPointerCall(defun, var1dynamic, concatenate);
	SetPointerCall(defun, var2dynamic, remove);
	SetPointerCall(defun, var2dynamic, remove_if);
	SetPointerCall(defun, var2dynamic, remove_if_not);
	SetPointerCall(defun, var2dynamic, delete);
	SetPointerCall(defun, var2dynamic, delete_if);
	SetPointerCall(defun, var2dynamic, delete_if_not);
	SetPointerCall(defun, var1dynamic, remove_duplicates);
	SetPointerCall(defun, var1dynamic, delete_duplicates);
}

void build_common_sequences(void)
{
	defun_copy_seq();
	defun_elt();
	defun_setf_elt();
	defun_fill();
	defun_make_sequence();
	defun_subseq();
	defun_setf_subseq();
	defun_map();
	defun_map_into();
	defun_reduce();
	defun_count();
	defun_count_if();
	defun_count_if_not();
	defun_length();
	defun_reverse();
	defun_nreverse();
	defun_merge();
	defun_sort();
	defun_stable_sort();
	defun_find();
	defun_find_if();
	defun_find_if_not();
	defun_position();
	defun_position_if();
	defun_position_if_not();
	defun_search();
	defun_mismatch();
	defun_replace();
	defun_substitute();
	defun_substitute_if();
	defun_substitute_if_not();
	defun_nsubstitute();
	defun_nsubstitute_if();
	defun_nsubstitute_if_not();
	defun_concatenate();
	defun_remove();
	defun_remove_if();
	defun_remove_if_not();
	defun_delete();
	defun_delete_if();
	defun_delete_if_not();
	defun_remove_duplicates();
	defun_delete_duplicates();
}

