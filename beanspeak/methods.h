/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_METHODS_H
#define PHP_BEANSPEAK_METHODS_H 1

#include <php.h>
#include <Zend/zend_API.h>

extern const zend_function_entry beanspeak_client_method_entry[];
extern const zend_function_entry beanspeak_exception_method_entry[];

/* Beanspeak\Client methods */
PHP_METHOD(Beanspeak_Client, __construct);

#endif /* PHP_BEANSPEAK_METHODS_H */
