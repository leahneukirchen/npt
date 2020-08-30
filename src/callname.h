#ifndef __CALLNAME_HEADER__
#define __CALLNAME_HEADER__

#include "execute.h"
#include "local.h"
#include "memory.h"
#include "typedef.h"

#define refcallname _n(refcallname)
#define getcallname _n(getcallname)
#define setcallname _n(setcallname)
#define refcallnametype _n(refcallnametype)
#define getcallnametype _n(getcallnametype)
#define setcallnametype _n(setcallnametype)
#define make_callname_alloc _n(make_callname_alloc)
#define callname_alloc _n(callname_alloc)
#define callname_local _n(callname_local)
#define callname_heap _n(callname_heap)
#define setf_callname_alloc _n(setf_callname_alloc)
#define setf_callname_local _n(setf_callname_local)
#define setf_callname_heap _n(setf_callname_heap)
#define copy_callname_alloc _n(copy_callname_alloc)
#define copy_callname_local _n(copy_callname_local)
#define copy_callname_heap _n(copy_callname_heap)
#define parse_callname _n(parse_callname)
#define parse_callname_alloc _n(parse_callname_alloc)
#define parse_callname_local _n(parse_callname_local)
#define parse_callname_heap _n(parse_callname_heap)
#define parse_callname_abort _n(parse_callname_abort)
#define parse_callname_error_ _n(parse_callname_error_)
#define callnamep _n(callnamep)
#define symbolp_callname _n(symbolp_callname)
#define setfp_callname _n(setfp_callname)
#define constantp_callname _n(constantp_callname)
#define function_name_p _n(function_name_p)
#define equal_callname _n(equal_callname)
#define getglobal_callname _n(getglobal_callname)
#define getglobalcheck_callname_ _n(getglobalcheck_callname_)
#define setglobal_callname_ _n(setglobal_callname_)
#define remtype_global_callname_ _n(remtype_global_callname_)
#define getglobal_parse_callname _n(getglobal_parse_callname)
#define getglobalcheck_parse_callname_ _n(getglobalcheck_parse_callname_)
#define setglobal_parse_callname_ _n(setglobal_parse_callname_)
#define name_callname_alloc _n(name_callname_alloc)
#define name_callname_local _n(name_callname_local)
#define name_callname_heap _n(name_callname_heap)

enum CALLNAME_TYPE {
	CALLNAME_ERROR = 0,
	CALLNAME_SYMBOL,
	CALLNAME_SETF,
	CALLNAME_SIZE
};

typedef enum CALLNAME_TYPE CallNameType;

#define RefCallName_Low(s)          RefArrayA2((s),0)
#define GetCallName_Low(s,v)        GetArrayA2((s),0,(v))
#define SetCallName_Low(s,v)        SetArrayA2((s),0,(v))
#define RefCallNameType_Low(s)      ((CallNameType)GetUser(s))
#define GetCallNameType_Low(s,v)    (*(v) = RefCallNameType_Low(s))
#define SetCallNameType_Low(s,v)    SetUser((s), (byte)(v))

#ifdef LISP_DEBUG
#define RefCallName(s)              refcallname(s)
#define GetCallName(s,v)            getcallname(s,v)
#define SetCallName(s,v)            setcallname(s,v)
#define RefCallNameType(s)          refcallnametype(s)
#define GetCallNameType(s,v)        getcallnametype(s,v)
#define SetCallNameType(s,v)        setcallnametype(s,v)
#define ParseCallName               parse_callname_abort
#else
#define RefCallName(s)              RefCallName_Low(s)
#define GetCallName(s,v)            GetCallName_Low(s,v)
#define SetCallName(s,v)            SetCallName_Low(s,v)
#define RefCallNameType(s)          RefCallNameType_Low(s)
#define GetCallNameType(s,v)        GetCallNameType_Low(s,v)
#define SetCallNameType(s,v)        SetCallNameType_Low(s,v)
#define ParseCallName               parse_callname_alloc
#endif

/* access */
_g addr refcallname(addr pos);
_g void getcallname(addr pos, addr *value);
_g void setcallname(addr pos, addr value);
_g CallNameType refcallnametype(addr pos);
_g void getcallnametype(addr pos, CallNameType *value);
_g void setcallnametype(addr pos, CallNameType value);

/* allocate */
_g void make_callname_alloc(LocalRoot local, addr *ret);
_g void callname_alloc(LocalRoot local, addr *ret, addr name, CallNameType type);
_g void callname_local(LocalRoot local, addr *ret, addr name, CallNameType type);
_g void callname_heap(addr *ret, addr name, CallNameType type);
_g void setf_callname_alloc(LocalRoot local, addr *ret, addr symbol);
_g void setf_callname_local(LocalRoot local, addr *ret, addr symbol);
_g void setf_callname_heap(addr *ret, addr symbol);

/* copy */
_g void copy_callname_alloc(LocalRoot local, addr *ret, addr pos);
_g void copy_callname_local(LocalRoot local, addr *ret, addr pos);
_g void copy_callname_heap(addr *ret, addr pos);

/* parse */
_g CallNameType parse_callname(addr name, addr *ret);
_g int parse_callname_alloc(LocalRoot local, addr *ret, addr name);
_g int parse_callname_local(LocalRoot local, addr *ret, addr name);
_g int parse_callname_heap(addr *ret, addr name);
_g void parse_callname_abort(LocalRoot local, addr *ret, addr name);
_g int parse_callname_error_(addr *ret, addr name);

/* boolean */
_g int callnamep(addr pos);
_g int symbolp_callname(addr call);
_g int setfp_callname(addr call);
_g int constantp_callname(addr call);
_g int function_name_p(addr name);
_g int equal_callname(addr left, addr right);

/* function */
_g void getglobal_callname(addr pos, addr *ret);
_g int getglobalcheck_callname_(addr pos, addr *ret);
_g int setglobal_callname_(addr pos, addr value);
_g int remtype_global_callname_(addr pos);

_g void getglobal_parse_callname(addr pos, addr *value);
_g int getglobalcheck_parse_callname_(addr pos, addr *ret);
_g int setglobal_parse_callname_(addr pos, addr value);

/* name */
_g void name_callname_alloc(LocalRoot local, addr pos, addr *ret);
_g void name_callname_local(LocalRoot local, addr pos, addr *ret);
_g void name_callname_heap(addr pos, addr *ret);

#endif

