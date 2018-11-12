/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_EXCEPTION_H
#define PHP_BEANSPEAK_EXCEPTION_H 1

#include "../php_beanspeak.h"

typedef enum beanspeak_exception_type {
	INVALID_ARGUMENT
} beanspeak_exception_type_t;

extern PHP_BEANSPEAK_API zend_class_entry *beanspeak_exceptioninterface_ce_ptr;
extern PHP_BEANSPEAK_API zend_class_entry *beanspeak_invalidargumentexception_ce_ptr;

extern zend_class_entry *exception_ce(beanspeak_exception_type_t type);
extern zend_object *throw_exception(beanspeak_exception_type_t type, const char *fmt, ...);

BEANSPEAK_INIT_CLASS(ExceptionInterface);
BEANSPEAK_INIT_CLASS(InvalidArgumentException);

/* {{{ beanspeak_exception_method_entry */
static const zend_function_entry beanspeak_exception_method_entry[] = {
	PHP_FE_END
};
/* }}} */

#endif /* PHP_BEANSPEAK_EXCEPTION_H */
