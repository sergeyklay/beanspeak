/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_CLASSES_H
#define PHP_BEANSPEAK_CLASSES_H 1

#include <Zend/zend_types.h>

/* {{{ class entries */
extern PHP_BEANSPEAK_API zend_class_entry *beanspeak_client_ce_ptr;
extern PHP_BEANSPEAK_API zend_class_entry *beanspeak_exceptioninterface_ce_ptr;
extern PHP_BEANSPEAK_API zend_class_entry *beanspeak_invalidargumentexception_ce_ptr;
/* }}} */

#endif // PHP_BEANSPEAK_CLASSES_H
