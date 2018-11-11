/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_H
#define PHP_BEANSPEAK_H 1

#include <php.h>
#include <Zend/zend.h>
#include <Zend/zend_modules.h>
#include <Zend/zend_operators.h>

#ifdef HAVE_STDINT_H
#	include <stdint.h>
#else
typedef __int16 int16_t;
typedef unsigned __int16 int16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

#ifdef HAVE_STDBOOL_H
#	include <stdbool.h>
#else
typedef enum {false = 0, true = 1} bool;
#endif

#ifdef PHP_WIN32
#	define PHP_BEANSPEAK_API __declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_BEANSPEAK_API __attribute__ ((visibility("default")))
#elif defined(PHPAPI)
#	define PHP_BEANSPEAK_API PHPAPI
#else
#	define PHP_BEANSPEAK_API
#endif

#include "beanspeak/common.h"

extern zend_module_entry beanspeak_module_entry;
#define phpext_beanspeak_ptr &beanspeak_module_entry

#define PHP_BEANSPEAK_VERSION		"1.0.0"
#define PHP_BEANSPEAK_EXTNAME		"beanspeak"
#define PHP_BEANSPEAK_AUTHOR		"Serghei Iakovlev"
#define PHP_BEANSPEAK_DESCRIPTION	"A PHP client library for the beanstalkd queue server."

#if defined(ZTS) && defined(COMPILE_DL_BEANSPEAK)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif /* PHP_BEANSPEAK_H */
