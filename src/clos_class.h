#ifndef __CLOS_CLASS_HEADER__
#define __CLOS_CLASS_HEADER__

#include "execute.h"
#include "local.h"
#include "typedef.h"

#define stdget_class_name_check _n(stdget_class_name_check)
#define stdget_class_name_ _n(stdget_class_name_)
#define stdset_class_name_ _n(stdset_class_name_)
#define stdget_class_direct_slots_ _n(stdget_class_direct_slots_)
#define stdset_class_direct_slots_ _n(stdset_class_direct_slots_)
#define stdget_class_direct_subclasses_ _n(stdget_class_direct_subclasses_)
#define stdset_class_direct_subclasses_ _n(stdset_class_direct_subclasses_)
#define stdget_class_direct_superclasses_ _n(stdget_class_direct_superclasses_)
#define stdset_class_direct_superclasses_ _n(stdset_class_direct_superclasses_)
#define stdget_class_precedence_list_ _n(stdget_class_precedence_list_)
#define stdset_class_precedence_list_ _n(stdset_class_precedence_list_)
#define stdget_class_slots_ _n(stdget_class_slots_)
#define stdset_class_slots_ _n(stdset_class_slots_)
#define stdget_class_finalized_p_ _n(stdget_class_finalized_p_)
#define stdset_class_finalized_p_ _n(stdset_class_finalized_p_)
#define stdget_class_prototype_ _n(stdget_class_prototype_)
#define stdset_class_prototype_ _n(stdset_class_prototype_)
#define stdget_class_default_initargs_ _n(stdget_class_default_initargs_)
#define stdset_class_default_initargs_ _n(stdset_class_default_initargs_)
#define stdget_class_direct_default_initargs_ _n(stdget_class_direct_default_initargs_)
#define stdset_class_direct_default_initargs_ _n(stdset_class_direct_default_initargs_)
#define stdget_class_version_ _n(stdget_class_version_)
#define stdset_class_version_ _n(stdset_class_version_)
#define stdget_class_document_ _n(stdget_class_document_)
#define stdset_class_document_ _n(stdset_class_document_)
#define stdget_class_redefined_class_ _n(stdget_class_redefined_class_)
#define stdset_class_redefined_class_ _n(stdset_class_redefined_class_)
#define clos_subclass_p_ _n(clos_subclass_p_)
#define clos_subtype_p_ _n(clos_subtype_p_)
#define clos_class_p_ _n(clos_class_p_)
#define clos_funcallable_p_ _n(clos_funcallable_p_)
#define clos_generic_p_ _n(clos_generic_p_)
#define clos_method_p_ _n(clos_method_p_)
#define clos_define_combination_p_ _n(clos_define_combination_p_)
#define clos_define_long_combination_p_ _n(clos_define_long_combination_p_)
#define clos_define_short_combination_p_ _n(clos_define_short_combination_p_)
#define clos_combination_p_ _n(clos_combination_p_)
#define clos_long_combination_p_ _n(clos_long_combination_p_)
#define clos_short_combination_p_ _n(clos_short_combination_p_)
#define clos_specializer_p_ _n(clos_specializer_p_)
#define clos_referenced_p_ _n(clos_referenced_p_)
#define clos_built_p_ _n(clos_built_p_)
#define funcallp_ _n(funcallp_)
#define clos_instance_alloc_ _n(clos_instance_alloc_)
#define clos_instance_local_ _n(clos_instance_local_)
#define clos_instance_heap_ _n(clos_instance_heap_)
#define clos_find_slotname _n(clos_find_slotname)
#define clos_precedence_list_redefine_ _n(clos_precedence_list_redefine_)
#define clos_precedence_list_ _n(clos_precedence_list_)
#define clos_compute_slots_ _n(clos_compute_slots_)
#define slotvector_set_location _n(slotvector_set_location)
#define clos_stdclass_direct_slots_ _n(clos_stdclass_direct_slots_)
#define clos_stdclass_prototype_ _n(clos_stdclass_prototype_)
#define clos_stdclass_supers_ _n(clos_stdclass_supers_)
#define build_clos_class _n(build_clos_class)
#define clos_subclass_p_debug _n(clos_subclass_p_debug)
#define clos_subtype_p_debug _n(clos_subtype_p_debug)
#define clos_generic_p_debug _n(clos_generic_p_debug)
#define clos_method_p_debug _n(clos_method_p_debug)
#define clos_define_combination_p_debug _n(clos_define_combination_p_debug)

/* access */
void stdget_class_name_check(addr pos, addr *ret);
int stdget_class_name_(addr pos, addr *ret);
int stdset_class_name_(addr pos, addr value);
int stdget_class_direct_slots_(addr pos, addr *ret);
int stdset_class_direct_slots_(addr pos, addr value);
int stdget_class_direct_subclasses_(addr pos, addr *ret);
int stdset_class_direct_subclasses_(addr pos, addr value);
int stdget_class_direct_superclasses_(addr pos, addr *ret);
int stdset_class_direct_superclasses_(addr pos, addr value);
int stdget_class_precedence_list_(addr pos, addr *ret);
int stdset_class_precedence_list_(addr pos, addr value);
int stdget_class_slots_(addr pos, addr *ret);
int stdset_class_slots_(addr pos, addr value);
int stdget_class_finalized_p_(addr pos, addr *ret);
int stdset_class_finalized_p_(addr pos, addr value);
int stdget_class_prototype_(addr pos, addr *ret);
int stdset_class_prototype_(addr pos, addr value);
int stdget_class_default_initargs_(addr pos, addr *ret);
int stdset_class_default_initargs_(addr pos, addr value);
int stdget_class_direct_default_initargs_(addr pos, addr *ret);
int stdset_class_direct_default_initargs_(addr pos, addr value);
int stdget_class_version_(addr pos, addr *ret);
int stdset_class_version_(addr pos, addr value);
int stdget_class_document_(addr pos, addr *ret);
int stdset_class_document_(addr pos, addr value);
int stdget_class_redefined_class_(addr pos, addr *ret);
int stdset_class_redefined_class_(addr pos, addr value);

/* check */
int clos_subclass_p_(addr clos, addr super, int *ret);
int clos_subtype_p_(addr clos, addr super, int *ret);
int clos_class_p_(addr clos, int *ret);
int clos_funcallable_p_(addr clos, int *ret);
int clos_generic_p_(addr clos, int *ret);
int clos_method_p_(addr clos, int *ret);

int clos_define_combination_p_(addr clos, int *ret);
int clos_define_long_combination_p_(addr pos, int *ret);
int clos_define_short_combination_p_(addr pos, int *ret);
int clos_combination_p_(addr pos, int *ret);
int clos_long_combination_p_(addr pos, int *ret);
int clos_short_combination_p_(addr pos, int *ret);

int clos_specializer_p_(addr clos, int *ret);
int clos_referenced_p_(addr clos, int *ret);
int clos_built_p_(addr clos, int *ret);
int funcallp_(addr pos, int *ret);

/* make-instance */
int clos_instance_alloc_(LocalRoot local, addr clos, addr *ret);
int clos_instance_local_(LocalRoot local, addr clos, addr *ret);
int clos_instance_heap_(addr clos, addr *ret);

/* interface */
int clos_find_slotname(addr slots, size_t size, addr name);
int clos_precedence_list_redefine_(
		LocalRoot local, addr pos, addr *ret, addr x, addr list);
int clos_precedence_list_(LocalRoot local, addr pos, addr *ret);
int clos_compute_slots_(LocalRoot local, addr clos, addr *ret);
void slotvector_set_location(addr slots);
int clos_stdclass_direct_slots_(addr instance, addr slots);
int clos_stdclass_prototype_(addr clos);
int clos_stdclass_supers_(LocalRoot local,
		addr *ret, addr metaclass, addr name, addr slots, addr supers);

/* build */
void build_clos_class(LocalRoot local);

/* debug */
int clos_subclass_p_debug(addr clos, addr super);
int clos_subtype_p_debug(addr clos, addr super);
int clos_generic_p_debug(addr clos);
int clos_method_p_debug(addr clos);
int clos_define_combination_p_debug(addr clos);

#endif

