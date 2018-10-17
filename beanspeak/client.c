/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif

#include <php.h>

#include "../php_beanspeak.h"
#include "client.h"

zend_object_handlers beanspeak_client_handlers;

/* {{{ beanspeak_client_create_object */
static zend_always_inline beanspeak_client_object_t* beanspeak_client_fetch_object(zend_object* obj)
{
	return (beanspeak_client_object_t*)((char*)(obj) - XtOffsetOf(beanspeak_client_object_t, zo));
}
/* }}} */

/* {{{ beanspeak_client_create_object */
static zend_object* beanspeak_client_create_object(zend_class_entry* ce_ptr)
{
	beanspeak_client_object_t* intern;

	intern = ecalloc(1, sizeof(beanspeak_client_object_t) + zend_object_properties_size(ce_ptr));

	zend_object_std_init(&intern->zo, ce_ptr);
	object_properties_init(&intern->zo, ce_ptr);

	intern->zo.handlers = &beanspeak_client_handlers;

	return &intern->zo;
}
/* }}} */

/* {{{ beanspeak_client_create_object */
static void beanspeak_client_object_free(zend_object *object)
{
	beanspeak_client_object_t* obj = beanspeak_client_fetch_object(object);

	zval_dtor(&obj->socket);
	zval_dtor(&obj->options);
	zval_dtor(&obj->usedTube);
	zval_dtor(&obj->watchedTubes);

	zend_object_std_dtor(&obj->zo);
}
/* }}} */

/* {{{ beanspeak_client_init_properties
 * Declare 'Beanspeak\Client' class properties.
 */
static void beanspeak_client_init_properties(zend_class_entry *ce_ptr)
{
	zend_declare_property_null(ce_ptr, ZEND_STRL("socket"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(ce_ptr, ZEND_STRL("options"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(ce_ptr, ZEND_STRL("usedTube"), "default", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(ce_ptr, ZEND_STRL("watchedTubes"), ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* }}} */

/* Create and register 'Beanspeak\Client' class. */
BEANSPEAK_INIT_CLASS(Beanspeak_Client) {
	BEANSPEAK_REGISTER_CLASS(Beanspeak, Client, beanspeak, client, beanspeak_client_method_entry, 0);

	beanspeak_client_ce_ptr->create_object = beanspeak_client_create_object;

	memcpy(&beanspeak_client_handlers, zend_get_std_object_handlers(), sizeof(beanspeak_client_handlers));

	/* offset of real object header (usually zero) */
	beanspeak_client_handlers.offset = (int) XtOffsetOf(beanspeak_client_object_t, zo);

	/* general object functions */
	beanspeak_client_handlers.free_obj  = beanspeak_client_object_free;
	beanspeak_client_handlers.dtor_obj  = zend_objects_destroy_object;
	beanspeak_client_handlers.clone_obj = NULL; /* has no clone implementation */

	beanspeak_client_init_properties(beanspeak_client_ce_ptr);

	return SUCCESS;
}

