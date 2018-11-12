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
// TODO:
// #include "beanspeak/exception.h"

/* {{{ class entries */
PHP_BEANSPEAK_API zend_class_entry *beanspeak_client_ce_ptr;
PHP_BEANSPEAK_API zend_class_entry *beanspeak_exceptioninterface_ce_ptr;
PHP_BEANSPEAK_API zend_class_entry *beanspeak_invalidargumentexception_ce_ptr;
/* }}} */

zend_object_handlers beanspeak_object_handlers;
zend_object_handlers beanspeak_client_object_handlers;

/* {{{ prop handler tables */
static HashTable beanspeak_client_prop_handlers;
/* }}} */

typedef int (*beanspeak_read_t)(beanspeak_object_t *obj, zval *retval);
typedef int (*beanspeak_write_t)(beanspeak_object_t *obj, zval *newval);

typedef struct _beanspeak_prop_handler {
	beanspeak_read_t read_func;
	beanspeak_write_t write_func;
} beanspeak_prop_handler;

/* {{{ beanspeak_dtor_prop_handler */
static void
beanspeak_dtor_prop_handler(zval *zv)
{
	free(Z_PTR_P(zv));
}
/* }}} */

/* {{{ beanspeak_read_prop_error */
static int
beanspeak_read_prop_error(beanspeak_object_t *obj, zval *retval)
{
	zend_throw_error(NULL, "Cannot read property");
	return FAILURE;
}
/* }}} */

/* {{{ beanspeak_write_prop_error */
static int
beanspeak_write_prop_error(beanspeak_object_t *obj, zval *newval)
{
	zend_throw_error(NULL, "Cannot write property");
	return FAILURE;
}
/* }}} */

/* {{{ beanspeak_register_prop_handler */
static void
beanspeak_register_prop_handler(HashTable *handler, char *name, size_t name_len, beanspeak_read_t read_func,
	beanspeak_write_t write_func)
{
	beanspeak_prop_handler hnd;
	zend_string *str;

	hnd.read_func = read_func ? read_func : beanspeak_read_prop_error;
	hnd.write_func = write_func ? write_func : beanspeak_write_prop_error;

	str = zend_string_init_interned(name, name_len, 1);

	zend_hash_add_mem(handler, str, &hnd, sizeof(beanspeak_prop_handler));
	zend_string_release_ex(str, 1);
}
/* }}} */


static HashTable prop_handler_storage;

/* {{{ beanspeak_prop_handler_storage_init */
static void
beanspeak_prop_handler_storage_init()
{
	zend_hash_init(&prop_handler_storage, 0, NULL, NULL, 1);
}
/* }}} */

/* {{{ beanspeak_prop_handler_storage_destroy */
static void
beanspeak_prop_handler_storage_destroy()
{
	zend_hash_destroy(&prop_handler_storage);
}
/* }}} */

/* {{{ beanspeak_prop_handler_storage_push */
static void
beanspeak_prop_handler_storage_push(zend_string *ce_name, HashTable *handler)
{
	zend_hash_add_ptr(&prop_handler_storage, ce_name, handler);
}
/* }}} */

/* {{{ beanspeak_prop_handler_storage_find */
static void*
beanspeak_prop_handler_storage_find(zend_string *ce_name)
{
	return zend_hash_find_ptr(&prop_handler_storage, ce_name);
}
/* }}} */

/* {{{ beanspeak_objects_set_class */
static beanspeak_object_t*
beanspeak_objects_set_class(zend_class_entry *ce_ptr)
{
	beanspeak_object_t *intern = ecalloc(1, sizeof(beanspeak_object_t) + zend_object_properties_size(ce_ptr));
	zend_class_entry *base_class = ce_ptr;

	while (
		(
			base_class->type != ZEND_INTERNAL_CLASS ||
			base_class->info.internal.module->module_number != beanspeak_module_entry.module_number
		) &&
		base_class->parent != NULL
	) {
		base_class = base_class->parent;
	}

	intern->prop_handler = beanspeak_prop_handler_storage_find(base_class->name);

	zend_object_std_init(&intern->zo, ce_ptr);
	object_properties_init(&intern->zo, ce_ptr);

	return intern;
}
/* }}} */

/* {{{ beanspeak_get_object_handlers */
static zend_object_handlers*
beanspeak_get_object_handlers(void) {
	return &beanspeak_object_handlers;
}
/* }}} */

/* {{{ beanspeak_client_create_object */
zend_object*
beanspeak_create_object(zend_class_entry* ce_ptr)
{
	beanspeak_object_t* intern = beanspeak_objects_set_class(ce_ptr);

	intern->zo.handlers = beanspeak_get_object_handlers();
	return &intern->zo;
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
	beanspeak_object_handlers.clone_obj = NULL; /* TODO: has no clone implementation */

	memcpy(&beanspeak_client_object_handlers, &beanspeak_object_handlers, sizeof(zend_object_handlers));
	beanspeak_client_object_handlers.dtor_obj  = zend_objects_destroy_object;

	beanspeak_prop_handler_storage_init();

	BEANSPEAK_REGISTER_CLASS(Beanspeak, Client, beanspeak, client, beanspeak_client_method_entry, 0);
	beanspeak_client_ce_ptr->create_object = beanspeak_create_object;
	zend_hash_init(&beanspeak_client_prop_handlers, 0, NULL, beanspeak_dtor_prop_handler, 1);
	// TODO: Init client props
	beanspeak_prop_handler_storage_push(beanspeak_client_ce_ptr->name, &beanspeak_client_prop_handlers);

	// TODO:
	// BEANSPEAK_INIT(Beanspeak_ExceptionInterface);
	// BEANSPEAK_INIT(Beanspeak_InvalidArgumentException);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
static PHP_MSHUTDOWN_FUNCTION(beanspeak)
{
	UNREGISTER_INI_ENTRIES();

	// TODO: Destroy handlers

	beanspeak_prop_handler_storage_destroy();

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
