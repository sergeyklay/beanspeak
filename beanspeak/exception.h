/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_EXCEPTION_H
#define PHP_BEANSPEAK_EXCEPTION_H 1

#include "../php_beanspeak.h"

typedef enum beanspeak_exception_type {
	INVALID_ARGUMENT
} beanspeak_exception_type_t;

extern zend_class_entry *beanspeak_exception_iface_ce_ptr;
extern zend_class_entry *beanspeak_invalidargumentexception_ce_ptr;

extern zend_class_entry *exception_ce(beanspeak_exception_type_t type);
extern zend_object *throw_exception(beanspeak_exception_type_t type, const char *fmt, ...);

int beanspeak_init_exception_iface(INIT_FUNC_ARGS);
int beanspeak_init_invalid_args_e(INIT_FUNC_ARGS);

/* {{{ beanspeak_exception_me */
static const zend_function_entry beanspeak_exception_me[] = {
	PHP_FE_END
};
/* }}} */

#endif /* PHP_BEANSPEAK_EXCEPTION_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
