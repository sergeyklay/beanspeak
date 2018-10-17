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

# define BEANSPEAK_INIT_CLASS(name) \
	int beanspeak_ ##name## _init(INIT_FUNC_ARGS)

#define BEANSPEAK_REGISTER_CLASS(ns, class_name, lower_ns, name, methods, flags) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		lower_ns## _ ##name## _ce = zend_register_internal_class(&ce); \
		if (!UNEXPECTED(!lower_ns## _ ##name## _ce)) { \
			char *_n = (#ns); \
			char *_c = (#class_name); \
			php_error_docref0(NULL, E_ERROR, "%s\%s: class registration has failed.", _n, _c); \
			return FAILURE; \
		}\
		lower_ns## _ ##name## _ce->ce_flags |= flags; \
	}

# define BEANSPEAK_INIT(name) \
	if (beanspeak_ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) { \
		return FAILURE; \
	}

#define BEANSPEAK_INIT_FUNCS(class_functions) \
	static const zend_function_entry class_functions[] =

#endif /* PHP_BEANSPEAK_BEANSPEAK_HELPERS_H */
