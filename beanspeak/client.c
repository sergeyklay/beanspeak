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

static zend_object_handlers Beanspeak_Client_handlers;

/* {{{ create_object */
static zend_object* create_object(zend_class_entry* ce)
{
	Beanspeak_Client_object* intern;

	intern = ecalloc(1, sizeof(Beanspeak_Client_object) + zend_object_properties_size(ce));

	ZVAL_NULL(&intern->socket);
	ZVAL_NULL(&intern->usedTube);
	ZVAL_NULL(&intern->watchedTubes);

	array_init_size(&intern->options, 0);

	zend_object_std_init(&intern->zo, ce);
	object_properties_init(&intern->zo, ce);

	intern->zo.handlers = &Beanspeak_Client_handlers;

	return &intern->zo;
}
/* }}} */

BEANSPEAK_INIT_CLASS(Beanspeak_Client) {
	BEANSPEAK_REGISTER_CLASS(Beanspeak, Client, beanspeak, client, beanspeak_client_method_entry, 0);

	beanspeak_client_ce->create_object = create_object;

	memcpy(&Beanspeak_Client_handlers, zend_get_std_object_handlers(), sizeof(Beanspeak_Client_handlers));

	Beanspeak_Client_handlers.offset   = XtOffsetOf(Beanspeak_Client_object, zo);
	// Beanspeak_Client_handlers.free_obj = free_object;
	Beanspeak_Client_handlers.dtor_obj = zend_objects_destroy_object;

	return SUCCESS;
}

