/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_H
# define PHP_BEANSPEAK_H 1

# include <Zend/zend_modules.h>

# include "beanspeak/helpers.h"

extern zend_module_entry beanspeak_module_entry;
# define phpext_beanspeak_ptr &beanspeak_module_entry

# define PHP_BEANSPEAK_VERSION     "1.0.0"
# define PHP_BEANSPEAK_EXTNAME     "beanspeak"
# define PHP_BEANSPEAK_AUTHOR      "Serghei Iakovlev"
# define PHP_BEANSPEAK_DESCRIPTION "A PHP client library for the beanstalkd queue server."

# if defined(ZTS) && defined(COMPILE_DL_BEANSPEAK)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif /* PHP_BEANSPEAK_H */
