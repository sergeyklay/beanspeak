/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_CLIENT_H
#define PHP_BEANSPEAK_CLIENT_H 1

#include "../php_beanspeak.h"

BEANSPEAK_API extern zend_class_entry *beanspeak_client_ce_ptr;
extern zend_object_handlers beanspeak_client_handlers;

typedef struct {
	zval			socket;			/* current socket connection */
	zend_string*	host;			/* the beanstalkd server hostname or IP address to connect to */
	uint16_t		port;			/* the port of the server to connect to */
	uint32_t		timeout;		/* timeout in seconds when establishing the connection */
	bool			persistent;		/* whether to make the connection persistent or not */
	zend_string*	usedTube;		/* current used tube */
	zval			watchedTubes;	/* current watched tubes */
	zend_object		zo;
} beanspeak_client_object_t;

int beanspeak_init_client(INIT_FUNC_ARGS);

PHP_METHOD(Beanspeak_Client, __construct);

/* {{{ ARG_INFO */
ZEND_BEGIN_ARG_INFO_EX(arginfo_beanspeak_client_construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dsn, IS_STRING, 1)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ beanspeak_client_me */
static const zend_function_entry beanspeak_client_me[] = {
	PHP_ME(Beanspeak_Client, __construct, arginfo_beanspeak_client_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
/* }}} */

#endif /* PHP_BEANSPEAK_CLIENT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
