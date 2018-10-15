/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <php.h>
#include <ext/standard/info.h>

#include "php_beanspeak.h"

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(beanspeak)
{
#if defined(ZTS) && defined(COMPILE_DL_BEANSPEAK)
	ZEND_TSRMLS_CACHE_UPDATE()
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(beanspeak)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_BEANSPEAK_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, php_sprintf("%s support", PHP_BEANSPEAK_EXTNAME), "enabled");
	php_info_print_table_row(2, "Author", PHP_BEANSPEAK_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_BEANSPEAK_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_end();

	// DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ arginfo
 */
/* }}} */

/* {{{ beanspeak_functions[]
 */
static const zend_function_entry beanspeak_functions[] = {
	PHP_FE_END
};
/* }}} */

/* {{{ beanspeak_module_entry
 */
zend_module_entry beanspeak_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_BEANSPEAK_EXTNAME,			/* Extension name */
	beanspeak_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(beanspeak),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(beanspeak),			/* PHP_MINFO - Module info */
	PHP_BEANSPEAK_VERSION,			/* Version */
	NO_MODULE_GLOBALS,				/* Globals, and the latter is a pointer directly to the globals */
	NULL,							/* This function is called by Zend after request shutdown. It is rarely used */
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_BEANSPEAK
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(beanspeak)
#endif
