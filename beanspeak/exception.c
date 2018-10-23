/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif

#include <php.h>
#include <Zend/zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>

#include "../php_beanspeak.h"
#include "exception.h"

zend_class_entry *beanspeak_exceptioninterface_ce_ptr;

/* {{{ beanspeak_Beanspeak_Exception_init
 * Create and register 'Beanspeak\ExceptionInterface' interface. */
BEANSPEAK_INIT_CLASS(Beanspeak_ExceptionInterface) {
	BEANSPEAK_REGISTER_CLASS(Beanspeak, ExceptionInterface, beanspeak,
		exceptioninterface, beanspeak_exception_method_entry, ZEND_ACC_INTERFACE);

	zend_class_implements(beanspeak_exceptioninterface_ce_ptr, 1, zend_ce_throwable);

	return SUCCESS;
}
/* }}} */


