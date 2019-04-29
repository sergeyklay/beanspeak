/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>

#include "php_beanspeak.h"
#include "beanspeak/client.h"
#include "beanspeak/exception.h"

PHP_INI_BEGIN()
/* TODO */
PHP_INI_END()

PHP_RINIT_FUNCTION(beanspeak)
{
#if defined(ZTS) && defined(COMPILE_DL_BEANSPEAK)
	ZEND_TSRMLS_CACHE_UPDATE()
#endif

	return SUCCESS;
}

static PHP_MINIT_FUNCTION(beanspeak)
{
	REGISTER_INI_ENTRIES();

	if (beanspeak_init_client(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) {
		return FAILURE;
	}

	if (beanspeak_init_exception_iface(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) {
		return FAILURE;
	}

   	if (beanspeak_init_invalid_args_e(INIT_FUNC_ARGS_PASSTHRU)== FAILURE) {
		return FAILURE;
	}

	return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(beanspeak)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}

PHP_MINFO_FUNCTION(beanspeak)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_BEANSPEAK_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, "beanspeak support", "enabled");
	php_info_print_table_row(2, "Author", PHP_BEANSPEAK_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_BEANSPEAK_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

static const zend_module_dep beanspeak_deps[] = {
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("standard")
	ZEND_MOD_END
};

zend_module_entry beanspeak_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	beanspeak_deps,
	PHP_BEANSPEAK_EXTNAME,
	NULL,
	PHP_MINIT(beanspeak),
	PHP_MSHUTDOWN(beanspeak),
	PHP_RINIT(beanspeak),
	NULL,
	PHP_MINFO(beanspeak),
	PHP_BEANSPEAK_VERSION,
	NO_MODULE_GLOBALS,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_BEANSPEAK
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(beanspeak)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
