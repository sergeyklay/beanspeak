/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

extern zend_class_entry *beanspeak_exceptioninterface_ce_ptr;

BEANSPEAK_INIT_CLASS(Beanspeak_ExceptionInterface);

/* {{{ beanspeak_exception_method_entry */
BEANSPEAK_INIT_FUNCS(beanspeak_exception_method_entry) {
	PHP_FE_END
};
/* }}} */
