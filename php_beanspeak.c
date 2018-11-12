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
#include <Zend/zend_types.h>
#include <Zend/zend_object_handlers.h>
#include <Zend/zend_operators.h>
#include <Zend/zend_portability.h>
#include <Zend/zend_exceptions.h>
#include <ext/standard/info.h>
#include <ext/spl/spl_exceptions.h>

#include "php_beanspeak.h"

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
	int type;
} beanspeak_prop_handler;

static void /* {{{ */
dtor_prop_handler(zval *zv)
{
	free(Z_PTR_P(zv));
}
/* }}} */

static zend_always_inline void /* {{{ */
register_prop_handler(
	HashTable *prop_handler,
	char *prop_name,
	beanspeak_read_t read_func,
	beanspeak_write_t write_func,
	int prop_type
) {
	beanspeak_prop_handler hnd;
	zend_string *str;

	hnd.read_func = read_func;
	hnd.write_func = write_func;
	hnd.type = prop_type;

	str = zend_string_init_interned(prop_name, strlen(prop_name), 1);

	zend_hash_add_mem(prop_handler, str, &hnd, sizeof(beanspeak_prop_handler));
	zend_string_release_ex(str, 1);
}
/* }}} */

static HashTable prop_handler_storage;

static beanspeak_object_t* /* {{{ */
objects_set_class(zend_class_entry *ce_ptr)
{
#if PHP_VERSION_ID >= 70300
	beanspeak_object_t *intern = zend_object_alloc(sizeof(beanspeak_object_t), ce_ptr);
#else
	beanspeak_object_t *intern = ecalloc(1, sizeof(beanspeak_object_t) + zend_object_properties_size(ce_ptr));
#endif

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

	intern->prop_handler = zend_hash_find_ptr(&prop_handler_storage, base_class->name);

	zend_object_std_init(&intern->zo, ce_ptr);
	object_properties_init(&intern->zo, ce_ptr);

	return intern;
}
/* }}} */

static zend_object_handlers* /* {{{ */
get_object_handlers(void) {
	return &beanspeak_object_handlers;
}
/* }}} */

/* {{{ beanspeak_client_create_object */
zend_object*
beanspeak_create_object(zend_class_entry* ce_ptr)
{
	beanspeak_object_t* intern = objects_set_class(ce_ptr);

	intern->zo.handlers = get_object_handlers();
	return &intern->zo;
}
/* }}} */

static void /* {{{ */
objects_free_storage(zend_object *object)
{
	beanspeak_object_t *intern = beanspeak_obj_from_zo(object);

	zend_object_std_dtor(&intern->zo);
}

static zval* /* {{{ */
read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
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

static void /* {{{ */
write_property(zval *object, zval *member, zval *value, void **cache_slot)
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

/* {{{ beanspeak_get_exception_ce */
zend_class_entry*
beanspeak_get_exception_ce(beanspeak_exception_type_t type)
{
	switch (type) {
		default:
		case INVALID_ARGUMENT:
			return beanspeak_invalidargumentexception_ce_ptr;
	}
}
/* }}} */

/* {{{ beanspeak_throw_exception */
zend_object*
beanspeak_throw_exception(beanspeak_exception_type_t type, const char *fmt, ...)
{
	char *msg;
	zend_object *exception;
	va_list argv;

	va_start(argv, fmt);
	vspprintf(&msg, 0, fmt, argv);
	va_end(argv);

	exception = zend_throw_exception(beanspeak_get_exception_ce(type), msg, type);
	efree(msg);

	return exception;
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

/* {{{ PHP_MINIT_FUNCTION */
static PHP_MINIT_FUNCTION(beanspeak)
{
	REGISTER_INI_ENTRIES();

	memcpy(&beanspeak_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	beanspeak_object_handlers.offset = (int) XtOffsetOf(beanspeak_object_t, zo);
	beanspeak_object_handlers.free_obj = objects_free_storage;
	beanspeak_object_handlers.read_property = read_property;
	beanspeak_object_handlers.write_property = write_property;
	beanspeak_object_handlers.clone_obj = NULL; /* TODO: has no clone implementation */

	memcpy(&beanspeak_client_object_handlers, &beanspeak_object_handlers, sizeof(zend_object_handlers));
	beanspeak_client_object_handlers.dtor_obj  = zend_objects_destroy_object;

	zend_hash_init(&prop_handler_storage, 0, NULL, NULL, 1);

	/* Create and register 'Beanspeak\Client' class */
	BEANSPEAK_REGISTER_CLASS(Beanspeak, Client, beanspeak, client, beanspeak_client_method_entry, 0);
	beanspeak_client_ce_ptr->create_object = beanspeak_create_object;

	/* Declare 'Beanspeak\Client' class properties */
	zend_hash_init(&beanspeak_client_prop_handlers, 0, NULL, dtor_prop_handler, 1);

	register_prop_handler(&beanspeak_client_prop_handlers, "socket", beanspeak_client_socket_read, NULL, IS_NULL);
	register_prop_handler(&beanspeak_client_prop_handlers, "host", beanspeak_client_host_read, NULL, IS_STRING);
	register_prop_handler(&beanspeak_client_prop_handlers, "port", beanspeak_client_port_read, NULL, IS_LONG);
	register_prop_handler(&beanspeak_client_prop_handlers, "timeout", beanspeak_client_timeout_read, NULL, IS_LONG);
	register_prop_handler(&beanspeak_client_prop_handlers, "persistent", beanspeak_client_persistent_read, NULL, _IS_BOOL);
	register_prop_handler(&beanspeak_client_prop_handlers, "usedTube", beanspeak_client_usedTube_read, NULL, IS_STRING);
	register_prop_handler(&beanspeak_client_prop_handlers, "watchedTubes", beanspeak_client_watchedTubes_read, NULL, IS_ARRAY);

	zend_declare_property_null(beanspeak_client_ce_ptr, ZEND_STRL("socket"), ZEND_ACC_PRIVATE);
	zend_declare_property_string(beanspeak_client_ce_ptr, ZEND_STRL("host"), "127.0.0.1", ZEND_ACC_PRIVATE);
	zend_declare_property_long(beanspeak_client_ce_ptr, ZEND_STRL("port"), 11300, ZEND_ACC_PRIVATE);
	zend_declare_property_long(beanspeak_client_ce_ptr, ZEND_STRL("timeout"), 60, ZEND_ACC_PRIVATE);
	zend_declare_property_bool(beanspeak_client_ce_ptr, ZEND_STRL("persistent"), true, ZEND_ACC_PRIVATE);
	zend_declare_property_string(beanspeak_client_ce_ptr, ZEND_STRL("usedTube"), "default", ZEND_ACC_PRIVATE);
	zend_declare_property_null(beanspeak_client_ce_ptr, ZEND_STRL("watchedTubes"), ZEND_ACC_PRIVATE);

	zend_hash_add_ptr(&prop_handler_storage, beanspeak_client_ce_ptr->name, &beanspeak_client_prop_handlers);

	/* Create and register 'Beanspeak\Exception\ExceptionInterface' interface */
	BEANSPEAK_REGISTER_CLASS(Beanspeak\\Exception, ExceptionInterface, beanspeak,
							 exceptioninterface, beanspeak_exception_method_entry, ZEND_ACC_INTERFACE);
	zend_class_implements(beanspeak_exceptioninterface_ce_ptr, 1, zend_ce_throwable);

	/* Create and register 'Beanspeak\Exception\InvalidArgumentException' class */
	BEANSPEAK_REGISTER_CLASS_EX(Beanspeak\\Exception, InvalidArgumentException, beanspeak, invalidargumentexception,
								spl_ce_InvalidArgumentException, beanspeak_exception_method_entry, 0);
	zend_class_implements(beanspeak_invalidargumentexception_ce_ptr, 1, beanspeak_exceptioninterface_ce_ptr);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
static PHP_MSHUTDOWN_FUNCTION(beanspeak)
{
	UNREGISTER_INI_ENTRIES();

	zend_hash_destroy(&beanspeak_client_prop_handlers);
	zend_hash_destroy(&prop_handler_storage);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
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

/* {{{ beanspeak_functions[] */
static const zend_function_entry beanspeak_functions[] = {
	PHP_FE_END
};
/* }}} */

/* {{{ beanspeak_module_entry */
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
