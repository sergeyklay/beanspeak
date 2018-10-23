/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

typedef enum beanspeak_exception_type {
	INVALID_ARGUMENT
} beanspeak_exception_type_t;

extern zend_class_entry *beanspeak_exceptioninterface_ce_ptr;
extern zend_class_entry *beanspeak_invalidargumentexception_ce_ptr;

extern zend_class_entry *exception_ce(beanspeak_exception_type_t type);
extern zend_object *throw_exception(beanspeak_exception_type_t type, const char *fmt, ...);

BEANSPEAK_INIT_CLASS(Beanspeak_ExceptionInterface);
BEANSPEAK_INIT_CLASS(Beanspeak_InvalidArgumentException);

/* {{{ beanspeak_exception_method_entry */
BEANSPEAK_INIT_FUNCS(beanspeak_exception_method_entry) {
	PHP_FE_END
};
/* }}} */
