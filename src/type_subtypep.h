#ifndef __TYPE_SUBTYPEP_HEADER__
#define __TYPE_SUBTYPEP_HEADER__

#include "type.h"
#include "typedef.h"

#define init_type_subtypep _n(init_type_subtypep)
#define subtypep_result_ _n(subtypep_result_)
#define subtypep_asterisk_clang_ _n(subtypep_asterisk_clang_)
#define subtypep_clang_ _n(subtypep_clang_)
#define subtypep_common _n(subtypep_common)
#define subtypep_result_syscall_ _n(subtypep_result_syscall_)

enum SUBTYPEP_RESULT {
	SUBTYPEP_INCLUDE,
	SUBTYPEP_EXCLUDE,
	SUBTYPEP_FALSE,
	SUBTYPEP_INVALID,
	SUBTYPEP_SIZE
};
typedef enum SUBTYPEP_RESULT SubtypepResult;

void init_type_subtypep(void);
int subtypep_result_(addr left, addr right, int aster, SubtypepResult *ret);
int subtypep_asterisk_clang_(addr left, addr right, int *ret, int *validp);
int subtypep_clang_(addr left, addr right, int *ret, int *validp);
int subtypep_common(Execute ptr, addr x, addr y, addr env, addr *v1, addr *v2);
int subtypep_result_syscall_(Execute ptr, addr x, addr y, addr env, addr *ret);

#endif

