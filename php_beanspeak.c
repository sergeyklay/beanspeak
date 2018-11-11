/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_ini.h>
#include <Zend/zend_hash.h>
#include <Zend/zend_object_handlers.h>
#include <Zend/zend_operators.h>
#include <ext/standard/info.h>

#include "php_beanspeak.h"
#include "beanspeak/client.h"
#include "beanspeak/exception.h"

/* {{{ class entries */
PHP_BEANSPEAK_API zend_class_entry *beanspeak_client_ce_ptr;
PHP_BEANSPEAK_API zend_class_entry *beanspeak_exceptioninterface_ce_ptr;
PHP_BEANSPEAK_API zend_class_entry *beanspeak_invalidargumentexception_ce_ptr;
/* }}} */

zend_object_handlers beanspeak_object_handlers;

static HashTable beanspeak_classes;

/* {{{ beanspeak_classes_ctor */
static void
beanspeak_ce_storage_init()
{
	zend_hash_init(&beanspeak_classes, 0, NULL, NULL, 1);
}
/* }}} */

/* {{{ beanspeak_classes_ctor */
static void
beanspeak_ce_storage_destroy()
{
	zend_hash_destroy(&beanspeak_classes);
}
/* }}} */

/* {{{ prop handler tables */
static HashTable beanspeak_client_prop_handlers;
/* }}} */

typedef int (*beanspeak_read_t)(beanspeak_object_t *obj, zval *retval);
typedef int (*beanspeak_write_t)(beanspeak_object_t *obj, zval *newval);

typedef struct _beanspeak_prop_handler {
	beanspeak_read_t read_func;
	beanspeak_write_t write_func;
} beanspeak_prop_handler;

/* {{{ beanspeak_get_object_handlers */
static zend_object_handlers*
beanspeak_get_object_handlers(void) {
	return &beanspeak_object_handlers;
}
/* }}} */

/* {{{ beanspeak_objects_free_storage */
static void
beanspeak_objects_free_storage(zend_object *object)
{
	beanspeak_object_t *intern = beanspeak_obj_from_zo(object);

	zend_object_std_dtor(&intern->zo);
}

/* {{{ beanspeak_read_property */
static zval*
beanspeak_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
{
	beanspeak_object_t *obj = Z_BEANSPEAK_OBJ_P(object);
	zval *retval;
	beanspeak_prop_handler *hnd = NULL;

	BEANSPEAK_PROPERTY_HANDLER_PROLOG;

	if (obj->prop_handler != NULL) {
		hnd = zend_hash_find_ptr(obj->prop_handler, Z_STR_P(member));
	}

	if (hnd != NULL) {
		int ret = hnd->read_func(obj, rv);
		if (ret == SUCCESS) {
			retval = rv;
		} else {
			retval = &EG(uninitialized_zval);
		}
	} else {
		retval = zend_std_read_property(object, member, type, cache_slot, rv);
	}

	BEANSPEAK_PROPERTY_HANDLER_EPILOG;

	return retval;
}
/* }}} */

/* {{{ beanspeak_write_property */
static void
beanspeak_write_property(zval *object, zval *member, zval *value, void **cache_slot)
{
	beanspeak_object_t *obj = Z_BEANSPEAK_OBJ_P(object);
	beanspeak_prop_handler *hnd = NULL;

	BEANSPEAK_PROPERTY_HANDLER_PROLOG;

	if (obj->prop_handler != NULL) {
		hnd = zend_hash_find_ptr(obj->prop_handler, Z_STR_P(member));
	}

	if (hnd != NULL) {
		hnd->write_func(obj, value);
	} else {
		zend_std_write_property(object, member, value, cache_slot);
	}

	BEANSPEAK_PROPERTY_HANDLER_EPILOG;
}
/* }}} */

/* {{{ INI_ENTRIES */
PHP_INI_BEGIN()

PHP_INI_END()
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(beanspeak)
{
#if defined(ZTS) && defined(COMPILE_DL_BEANSPEAK)
	ZEND_TSRMLS_CACHE_UPDATE()
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
static PHP_MINIT_FUNCTION(beanspeak)
{
	REGISTER_INI_ENTRIES();

	memcpy(&beanspeak_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	beanspeak_object_handlers.offset = (int) XtOffsetOf(beanspeak_object_t, zo);
	beanspeak_object_handlers.free_obj = beanspeak_objects_free_storage;
	beanspeak_object_handlers.read_property = beanspeak_read_property;
	beanspeak_object_handlers.write_property = beanspeak_write_property;

	beanspeak_ce_storage_init();

	BEANSPEAK_INIT(Beanspeak_Client);
	BEANSPEAK_INIT(Beanspeak_ExceptionInterface);
	BEANSPEAK_INIT(Beanspeak_InvalidArgumentException);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
static PHP_MSHUTDOWN_FUNCTION(beanspeak)
{
	UNREGISTER_INI_ENTRIES();

	beanspeak_ce_storage_destroy();

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
	php_info_print_table_header(2, "beanspeak support", "enabled");
	php_info_print_table_row(2, "Author", PHP_BEANSPEAK_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_BEANSPEAK_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
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
	PHP_MINIT(beanspeak) ,			/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(beanspeak),		/* PHP_MSHUTDOWN - Module shutdown */
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
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(beanspeak)
#endif
