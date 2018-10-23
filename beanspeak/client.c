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
#include <Zend/zend_operators.h>
#include <ext/standard/url.h>

#include "../php_beanspeak.h"
#include "client.h"

zend_object_handlers beanspeak_client_handlers;

/* {{{ beanspeak_client_create_object */
static zend_always_inline beanspeak_client_object_t*
beanspeak_client_fetch_object(zend_object* obj)
{
	return (beanspeak_client_object_t*)((char*)(obj) - XtOffsetOf(beanspeak_client_object_t, zo));
}
/* }}} */

/* {{{ beanspeak_client_create_object */
static zend_object*
beanspeak_client_create_object(zend_class_entry* ce_ptr)
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
static void
beanspeak_client_object_free(zend_object *object)
{
	beanspeak_client_object_t* obj = beanspeak_client_fetch_object(object);

	zval_dtor(&obj->socket);
	zval_dtor(&obj->host);
	zval_dtor(&obj->port);
	zval_dtor(&obj->timeout);
	zval_dtor(&obj->persistent);
	zval_dtor(&obj->usedTube);
	zval_dtor(&obj->watchedTubes);

	zend_object_std_dtor(&obj->zo);
}
/* }}} */

/* {{{ beanspeak_client_init_properties
 * Declare 'Beanspeak\Client' class properties. */
static void
beanspeak_client_init_properties(zend_class_entry *ce_ptr)
{
	zend_declare_property_null(ce_ptr, ZEND_STRL("socket"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(ce_ptr, ZEND_STRL("host"), "127.0.0.1", ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_long(ce_ptr, ZEND_STRL("port"), 11300, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_long(ce_ptr, ZEND_STRL("timeout"), 60, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_bool(ce_ptr, ZEND_STRL("persistent"), 1, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(ce_ptr, ZEND_STRL("usedTube"), "default", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(ce_ptr, ZEND_STRL("watchedTubes"), ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* }}} */

/* {{{ beanspeak_Beanspeak_Client_init
 * Create and register 'Beanspeak\Client' class. */
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
/* }}} */

/* {{{ beanspeak_client_instance */
static int
beanspeak_client_initialize(zval *this_ptr, const char *dsn_str, const size_t dsn_len)
{
	php_url *uri;
	zval host;
	zval port;

	ZVAL_NULL(&host);
	ZVAL_NULL(&port);

	if (Z_ISUNDEF_P(this_ptr)) {
		object_init_ex(this_ptr, beanspeak_client_ce_ptr);
	}

	/* php_url_parse_ex() crashes by processing chars exceed len */
	if (strlen(dsn_str) != dsn_len) {
		php_error_docref(NULL, E_ERROR, "Client DSN contains invalid characters (\\0).");
		return FAILURE;
	}

	if (!(uri = php_url_parse_ex(dsn_str, dsn_len))) {
		php_error_docref(NULL, E_ERROR, "The beanstalkd connection DSN is invalid: '%s'.", dsn_str);
		return FAILURE;
	}

	/* Check protocol.
	 * Right now only 'tcp' is allowed:
	 *   tcp://10.0.0.8:11300
	 * todo: add support of unix sockets
	 */
	if (uri->scheme) {
#if IS_AT_LEAST_PHP_73
		if (strncasecmp("unix", ZSTR_VAL(uri->scheme), sizeof("unix")) == 0) {
#else
		if (strncasecmp("unix", uri->scheme, sizeof("unix")) == 0) {
#endif
			php_error_docref(NULL, E_ERROR, "Protocol 'unix' currently disabled in Beanspeak Client.");
			php_url_free(uri);
			return FAILURE;
		}

#if IS_AT_LEAST_PHP_73
		if (strncasecmp("tcp", ZSTR_VAL(uri->scheme), sizeof("tcp")) < 0) {
			php_error_docref(NULL, E_ERROR,
				"Invalid DSN scheme. Supported schemes are either 'tcp' or 'unix' (disabled), got '%s'.",
				ZSTR_VAL(uri->scheme)
			);
#else
		if (strncasecmp("tcp", uri->scheme, sizeof("tcp")) < 0) {
			php_error_docref(NULL, E_ERROR,
				 "Invalid DSN scheme. Supported schemes are either 'tcp' or 'unix' (disabled), got '%s'.",
				 uri->scheme
			);
#endif
			php_url_free(uri);
			return FAILURE;
		}
	}

	if (uri->host) {
#if IS_AT_LEAST_PHP_73
		ZVAL_STR(&host, uri->host);
#else
		ZVAL_STRING(&host, uri->host);
#endif
	} else if (uri->path && !uri->host) {
		/* allow simple 'hostname' format, which php_url_parse_ex() treats as a path, not host */
#if IS_AT_LEAST_PHP_73
		ZVAL_STR(&host, uri->path);
#else
		ZVAL_STRING(&host, uri->path);
#endif
	} else {
		php_error_docref(NULL, E_ERROR, "Invalid Client DSN scheme: missed host part.");
		php_url_free(uri);
		return FAILURE;
	}

	/* extract the port number as a long */
	if (uri->port) {
		ZVAL_LONG(&port, (long)uri->port);
	}

	/* todo: get 'timeout' and 'persistent' from the uri->query as well */

	if (!ZVAL_IS_NULL(&host)) {
		zend_update_property(beanspeak_client_ce_ptr, this_ptr, ZEND_STRL("host"), &host);
		zval_ptr_dtor(&host);
	}

	if (!ZVAL_IS_NULL(&port)) {
		zend_update_property(beanspeak_client_ce_ptr, this_ptr, ZEND_STRL("port"), &port);
		zval_ptr_dtor(&port);
	}

	php_url_free(uri);

	return SUCCESS;
}
/* }}} */

/* {{{ proto public Beanspeak\Client::__construct([string $dsn = NULL]) */
PHP_METHOD(Beanspeak_Client, __construct) {
	char *dsn_str = "";
	size_t dsn_len = 0;

	BEANSPEAK_INIT_THIS();

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|s!", &dsn_str, &dsn_len) == FAILURE) {
		return;
	}

	if (dsn_len != 0) {
		/* the dsn string was provided. initialize connection options */
		beanspeak_client_initialize(this_ptr, dsn_str, dsn_len);
	}
}
/* }}} */
