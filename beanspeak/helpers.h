/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_BEANSPEAK_HELPERS_H
# define PHP_BEANSPEAK_BEANSPEAK_HELPERS_H 1

# include <Zend/zend.h>

# define BEANSPEAK_INIT_CLASS(name) \
	int beanspeak_ ##name## _init(INIT_FUNC_ARGS)

/* class/interface registering */
#define BEANSPEAK_REGISTER_CLASS(ns, cl, lns, n, m, f)					\
	{																	\
		zend_class_entry ce;											\
		memset(&ce, 0, sizeof(zend_class_entry));						\
		INIT_NS_CLASS_ENTRY(ce, #ns, #cl, m);							\
		lns## _ ##n## _ce_ptr = zend_register_internal_class(&ce);		\
		if (UNEXPECTED(!lns## _ ##n## _ce_ptr)) {						\
			zend_error(E_ERROR, "Class '%s' registration was failed.",	\
				ZEND_NS_NAME(#ns, #cl));								\
			return FAILURE;												\
		}																\
		lns## _ ##n## _ce_ptr->ce_flags |= f;							\
	}

/* class/interface registering with parents */
#define BEANSPEAK_REGISTER_CLASS_EX(ns, cl, lns, n, pce, m, f) 						\
	{																				\
		zend_class_entry ce;														\
		if (!pce) {																	\
			zend_error(E_ERROR, "Can't register class '%s' with null parent.",		\
				ZEND_NS_NAME(#ns, #cl));											\
			return FAILURE;															\
		}																			\
		memset(&ce, 0, sizeof(zend_class_entry));									\
		INIT_NS_CLASS_ENTRY(ce, #ns, #cl, m);										\
		lns## _ ##n## _ce_ptr = zend_register_internal_class_ex(&ce, pce); 			\
		if (UNEXPECTED(!lns## _ ##n## _ce_ptr)) {									\
			zend_error(E_ERROR, "Class '%s' registration was failed.",				\
				ZEND_NS_NAME(#ns, #cl));											\
			return FAILURE;															\
		}																			\
		if (!lns## _ ##n## _ce_ptr) {												\
			zend_error(E_ERROR,														\
				"Class to extend '%s' was not found when registering class '%s'.",	\
				(pce ? ZSTR_VAL(pce->name) : "(null)"), ZEND_NS_NAME(#ns, #cl));	\
			return FAILURE;															\
		}																			\
		lns## _ ##n## _ce_ptr->ce_flags |= f;										\
	}

# define BEANSPEAK_INIT(name)												\
	if (beanspeak_ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) {	\
		return FAILURE;														\
	}

#define BEANSPEAK_INIT_FUNCS(class_functions) \
	static const zend_function_entry class_functions[] =

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

#endif /* PHP_BEANSPEAK_BEANSPEAK_HELPERS_H */
