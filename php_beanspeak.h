/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_H
#define PHP_BEANSPEAK_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <php.h>
#include <Zend/zend.h>
#include <Zend/zend_types.h>
#include <Zend/zend_modules.h>
#include <Zend/zend_operators.h>

/* {{{ Extension name */
#define PHP_BEANSPEAK_VERSION "1.0.0"
#define PHP_BEANSPEAK_EXTNAME "beanspeak"
#define PHP_BEANSPEAK_AUTHOR "Serghei Iakovlev"
#define PHP_BEANSPEAK_DESCRIPTION \
	"A PHP client library for the beanstalkd queue server."
/* }}} */

#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
typedef __int16 int16_t;
typedef unsigned __int16 int16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#else
typedef enum {false = 0, true = 1} bool;
#endif

#ifdef PHP_WIN32
#define BEANSPEAK_API __declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define BEANSPEAK_API __attribute__ ((visibility("default")))
#elif defined(PHPAPI)
#define BEANSPEAK_API PHPAPI
#else
#define BEANSPEAK_API
#endif

#define BEANSPEAK_INIT_THIS() \
	zval this_zv; \
	zval *this_ptr = getThis(); \
	if (EXPECTED(this_ptr)) { \
		ZVAL_OBJ(&this_zv, Z_OBJ_P(this_ptr)); \
		this_ptr = &this_zv; \
	} else { \
		ZVAL_NULL(&this_zv); \
		this_ptr = &this_zv; \
	}

extern zend_module_entry beanspeak_module_entry;
#define phpext_beanspeak_ptr &beanspeak_module_entry

#define PHP_BEANSPEAK_VERSION "1.0.0"
#define PHP_BEANSPEAK_EXTNAME "beanspeak"
#define PHP_BEANSPEAK_AUTHOR "Serghei Iakovlev"
#define PHP_BEANSPEAK_DESCRIPTION \
	"A PHP client library for the beanstalkd queue server."

#if defined(ZTS) && defined(COMPILE_DL_BEANSPEAK)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif /* PHP_BEANSPEAK_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
