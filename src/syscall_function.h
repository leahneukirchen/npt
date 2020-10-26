#ifndef __SYSCALL_FUNCTION_HEADER__
#define __SYSCALL_FUNCTION_HEADER__

#include "define.h"

#define init_syscall_function _n(init_syscall_function)
#define build_syscall_function _n(build_syscall_function)

_g void init_syscall_function(void);
_g void build_syscall_function(void);

#endif

