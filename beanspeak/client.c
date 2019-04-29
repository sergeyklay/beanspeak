/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif

#include <php.h>
#include <Zend/zend_operators.h>
#include <ext/standard/url.h>

#include "../php_beanspeak.h"
#include "client.h"
#include "exception.h"

zend_object_handlers beanspeak_client_handlers;
zend_class_entry *beanspeak_client_ce_ptr;

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
	zval_dtor(&obj->watchedTubes);

	if (obj->host) {
		zend_string_release(obj->host);
	}

	if (obj->usedTube) {
		zend_string_release(obj->usedTube);
	}

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
	zend_declare_property_bool(ce_ptr, ZEND_STRL("persistent"), true, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(ce_ptr, ZEND_STRL("usedTube"), "default", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(ce_ptr, ZEND_STRL("watchedTubes"), ZEND_ACC_PROTECTED TSRMLS_CC);
}
/* }}} */

/* {{{ beanspeak_init_client
 * Create and register 'Beanspeak\Client' class. */
int beanspeak_init_client(INIT_FUNC_ARGS) {
	zend_class_entry ce;

	memset(&ce, 0, sizeof(zend_class_entry));
	INIT_NS_CLASS_ENTRY(ce, "Beanspeak", "Client", beanspeak_client_me);
	beanspeak_client_ce_ptr = zend_register_internal_class(&ce);

	if (UNEXPECTED(!beanspeak_client_ce_ptr)) {
		zend_error(E_ERROR, "Beanspeak\\Client registration has failed.");
		return FAILURE;
	}

	beanspeak_client_ce_ptr->ce_flags |= 0;
	beanspeak_client_ce_ptr->create_object = beanspeak_client_create_object;

	memcpy(&beanspeak_client_handlers, zend_get_std_object_handlers(),
		   sizeof(beanspeak_client_handlers));

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

	if (Z_ISUNDEF_P(this_ptr)) {
		object_init_ex(this_ptr, beanspeak_client_ce_ptr);
	}

	/* php_url_parse_ex() crashes by processing chars exceed len */
	if (strlen(dsn_str) != dsn_len) {
		throw_exception(INVALID_ARGUMENT, "Client DSN contains invalid characters (\\0).");
		return FAILURE;
	}

	if (!(uri = php_url_parse_ex(dsn_str, dsn_len))) {
		throw_exception(INVALID_ARGUMENT, "The beanstalkd connection DSN is invalid: '%s'.", dsn_str);
		return FAILURE;
	}

	/* Check protocol.
	 * Right now only 'tcp' is allowed:
	 *   tcp://10.0.0.8:11300
	 * todo: add support of unix sockets
	 */
	if (uri->scheme) {
#if PHP_VERSION_ID >= 70300
		if (strncasecmp("unix", ZSTR_VAL(uri->scheme), sizeof("unix")) == 0) {
#else
		if (strncasecmp("unix", uri->scheme, sizeof("unix")) == 0) {
#endif
			throw_exception(INVALID_ARGUMENT, "Protocol 'unix' currently disabled in Beanspeak Client.");
			php_url_free(uri);
			return FAILURE;
		}

#if PHP_VERSION_ID >= 70300
		if (strncasecmp("tcp", ZSTR_VAL(uri->scheme), sizeof("tcp")) != 0) {
			throw_exception(INVALID_ARGUMENT,
				"Invalid DSN scheme. Supported schemes are either 'tcp' or 'unix' (disabled), got '%s'.",
				ZSTR_VAL(uri->scheme)
			);
#else
		if (strncasecmp("tcp", uri->scheme, sizeof("tcp")) != 0) {
			throw_exception(INVALID_ARGUMENT,
				 "Invalid DSN scheme. Supported schemes are either 'tcp' or 'unix' (disabled), got '%s'.",
				 uri->scheme
			);
#endif
			php_url_free(uri);
			return FAILURE;
		}
	}

	if (uri->host) {
#if PHP_VERSION_ID >= 70300
		zend_update_property_string(beanspeak_client_ce_ptr, this_ptr, ZEND_STRL("host"), ZSTR_VAL(uri->host));
#else
		zend_update_property_string(beanspeak_client_ce_ptr, this_ptr, ZEND_STRL("host"), uri->host);
#endif
	} else if (uri->path && !uri->host) {
		/* allow simple 'hostname' format, which php_url_parse_ex() treats as a path, not host */
#if PHP_VERSION_ID >= 70300
		zend_update_property_string(beanspeak_client_ce_ptr, this_ptr, ZEND_STRL("host"), ZSTR_VAL(uri->path));
#else
		zend_update_property_string(beanspeak_client_ce_ptr, this_ptr, ZEND_STRL("host"), uri->path);
#endif
	} else {
		throw_exception(INVALID_ARGUMENT, "Invalid Client DSN scheme: missed host part.");
		php_url_free(uri);
		return FAILURE;
	}

	/* extract the port number as a long */
	if (uri->port) {
		zend_update_property_long(beanspeak_client_ce_ptr, this_ptr, ZEND_STRL("port"), (uint16_t)uri->port);
	}

	/* todo: get 'timeout' and 'persistent' from the uri->query as well */

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

 /*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
