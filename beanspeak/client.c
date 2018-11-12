/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifdef HAVE_CONFIG_H
#	include "../config.h"
#endif

#include <php.h>
#include <ext/standard/url.h>
#include "../php_beanspeak.h"

PHP_METHOD(Beanspeak_Client, __construct);

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_beanspeak_client_construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dsn, IS_STRING, 1)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ beanspeak_client_method_entry[] */
const zend_function_entry beanspeak_client_method_entry[] = {
	PHP_ME(Beanspeak_Client, __construct, arginfo_beanspeak_client_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
/* }}} */

#include "exception.h"

zend_class_entry *beanspeak_client_ce_ptr;

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
