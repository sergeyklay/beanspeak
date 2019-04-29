/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif

#include <php.h>
#include <Zend/zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>

#include "../php_beanspeak.h"
#include "exception.h"

zend_class_entry *beanspeak_exception_iface_ce_ptr;
zend_class_entry *beanspeak_invalid_args_ce_ptr;

zend_class_entry*
exception_ce(beanspeak_exception_type_t type)
{
	switch (type) {
		default:
		case INVALID_ARGUMENT:
			return beanspeak_invalid_args_ce_ptr;
	}
}

zend_object*
throw_exception(beanspeak_exception_type_t type, const char *fmt, ...)
{
	char *msg;
	zend_object *exception;
	va_list argv;

	va_start(argv, fmt);
	vspprintf(&msg, 0, fmt, argv);
	va_end(argv);

	exception = zend_throw_exception(exception_ce(type), msg, type);
	efree(msg);

	return exception;
}

/* {{{ beanspeak_init_exception_iface
 * Create and register 'Beanspeak\Exception\ExceptionInterface' interface. */
int beanspeak_init_exception_iface(INIT_FUNC_ARGS) {
	zend_class_entry ce;

	memset(&ce, 0, sizeof(zend_class_entry));
	INIT_NS_CLASS_ENTRY(ce, "Beanspeak\\Exception", "ExceptionInterface",
						beanspeak_exception_me);

	beanspeak_exception_iface_ce_ptr = zend_register_internal_class(&ce);

	if (UNEXPECTED(!beanspeak_exception_iface_ce_ptr)) {
		zend_error(
			E_ERROR,
			"Beanspeak\\Exception\\ExceptionInterface registration has failed."
		);
		return FAILURE;
	}

	beanspeak_exception_iface_ce_ptr->ce_flags |= ZEND_ACC_INTERFACE;

	zend_class_implements(beanspeak_exception_iface_ce_ptr, 1,
						  zend_ce_throwable);

	return SUCCESS;
}
/* }}} */

/* {{{ beanspeak_init_args_e
 * Create and register 'Beanspeak\Exception\InvalidArgumentException' class. */
int beanspeak_init_invalid_args_e(INIT_FUNC_ARGS) {
	zend_class_entry ce;

	if (!spl_ce_InvalidArgumentException) {
		zend_error(
			E_ERROR,
			"Can't register class %s with null parent.",
			"Beanspeak\\Exception\\InvalidArgumentException"
		);
		return FAILURE;
	}

	memset(&ce, 0, sizeof(zend_class_entry));
	INIT_NS_CLASS_ENTRY(ce, "Beanspeak\\Exception", "InvalidArgumentException",
						beanspeak_exception_me);

	beanspeak_invalid_args_ce_ptr = zend_register_internal_class_ex(
		&ce,
		spl_ce_InvalidArgumentException);

	if (UNEXPECTED(!beanspeak_invalid_args_ce_ptr)) {
		zend_error(
			E_ERROR,
			"Class to extend %s was not found when registering %s.",
			ZSTR_VAL(spl_ce_InvalidArgumentException->name),
			"Beanspeak\\Exception\\InvalidArgumentException"
		);
		return FAILURE;
	}

	beanspeak_invalid_args_ce_ptr->ce_flags |= 0;

	zend_class_implements(
		beanspeak_invalid_args_ce_ptr,
		1,
		beanspeak_exception_iface_ce_ptr);

	return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
