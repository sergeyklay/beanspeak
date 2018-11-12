#ifndef PHP_BEANSPEAK_COMMON_H
#define PHP_BEANSPEAK_COMMON_H 1

#include <Zend/zend_types.h>

typedef struct _beanspeak_object {
	HashTable *prop_handler;
	zend_object zo;
} beanspeak_object_t;

static zend_always_inline beanspeak_object_t* /* {{{ */
beanspeak_obj_from_zo(zend_object *obj)
{
	return (beanspeak_object_t*)((char*)(obj) - XtOffsetOf(beanspeak_object_t, zo));
}
/* }}} */

typedef enum _beanspeak_exception_type {
	INVALID_ARGUMENT
} beanspeak_exception_type_t;

#define Z_BEANSPEAK_OBJ_P(zv) beanspeak_obj_from_zo(Z_OBJ_P((zv)))

/* class/interface registering */
#define BEANSPEAK_REGISTER_CLASS(ns, cl, lns, n, m, f)					\
	{																	\
		zend_class_entry ce;											\
		memset(&ce, 0, sizeof(zend_class_entry));						\
		INIT_NS_CLASS_ENTRY(ce, #ns, #cl, m);							\
		lns## _ ##n## _ce_ptr = zend_register_internal_class(&ce);		\
		if (UNEXPECTED(!lns## _ ##n## _ce_ptr)) {						\
			zend_error(E_ERROR, "Class '%s' registration has failed.",	\
				ZEND_NS_NAME(#ns, #cl));								\
			return FAILURE;												\
		}																\
		lns## _ ##n## _ce_ptr->ce_flags |= f;							\
	}

/* class/interface registering with parents */
#define BEANSPEAK_REGISTER_CLASS_EX(ns, cl, lns, n, pce, m, f)						\
	{																				\
		zend_class_entry ce;														\
		if (!pce) {																	\
			zend_error(E_ERROR, "Can't register class '%s' with null parent.",		\
				ZEND_NS_NAME(#ns, #cl));											\
			return FAILURE;															\
		}																			\
		memset(&ce, 0, sizeof(zend_class_entry));									\
		INIT_NS_CLASS_ENTRY(ce, #ns, #cl, m);										\
		lns## _ ##n## _ce_ptr = zend_register_internal_class_ex(&ce, pce);			\
		if (UNEXPECTED(!lns## _ ##n## _ce_ptr)) {									\
			zend_error(E_ERROR,														\
				"Class to extend '%s' was not found when registering class '%s'.",	\
				(pce ? ZSTR_VAL(pce->name) : "NULL"), ZEND_NS_NAME(#ns, #cl));		\
			return FAILURE;															\
		}																			\
		lns## _ ##n## _ce_ptr->ce_flags |= f;										\
	}

#define BEANSPEAK_INIT_THIS()					\
	zval this_zv;								\
	zval *this_ptr = getThis();					\
	if (EXPECTED(this_ptr)) {					\
		ZVAL_OBJ(&this_zv, Z_OBJ_P(this_ptr));	\
		this_ptr = &this_zv;					\
	} else {									\
		ZVAL_NULL(&this_zv);					\
		this_ptr = &this_zv;					\
	}

#define BEANSPEAK_PROPERTY_HANDLER_PROLOG						\
	zval tmp_member;											\
	if (UNEXPECTED(Z_TYPE_P(member) != IS_STRING)) {			\
		ZVAL_STR(&tmp_member, zval_get_string_func(member));	\
		member = &tmp_member;									\
		cache_slot = NULL;										\
	}

#define BEANSPEAK_PROPERTY_HANDLER_EPILOG	\
	if (member == &tmp_member) {			\
		zval_ptr_dtor_str(&tmp_member);		\
	}

#endif // PHP_BEANSPEAK_COMMON_H
