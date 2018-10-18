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

#define BEANSPEAK_REGISTER_CLASS(ns, class_name, lower_ns, name, methods, flags)				\
	{																							\
		zend_class_entry ce;																	\
		memset(&ce, 0, sizeof(zend_class_entry));												\
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods);										\
		lower_ns## _ ##name## _ce_ptr = zend_register_internal_class(&ce);						\
		if (UNEXPECTED(!lower_ns## _ ##name## _ce_ptr)) {										\
			char *_n = (#ns);																	\
			char *_c = (#class_name);															\
			zend_error(E_ERROR, "%s\\%s: class registration has failed.", _n, _c);				\
			return FAILURE;																		\
		}																						\
		lower_ns## _ ##name## _ce_ptr->ce_flags |= flags;										\
	}

# define BEANSPEAK_INIT(name)												\
	if (beanspeak_ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) {	\
		return FAILURE;														\
	}

#define BEANSPEAK_INIT_FUNCS(class_functions) \
	static const zend_function_entry class_functions[] =

#define BEANSPEAK_INIT_THIS() 					\
	zval this_zv;								\
	zval *this_ptr = getThis();					\
	if (EXPECTED(this_ptr)) {					\
		ZVAL_OBJ(&this_zv, Z_OBJ_P(this_ptr));	\
		this_ptr = &this_zv;					\
	} else {									\
		this_ptr = NULL;						\
	}

#endif /* PHP_BEANSPEAK_BEANSPEAK_HELPERS_H */
