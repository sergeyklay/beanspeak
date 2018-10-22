/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

extern zend_class_entry *beanspeak_client_ce_ptr;
extern zend_object_handlers beanspeak_client_handlers;

typedef struct {
	zval		socket;			/* current socket connection */
	zval		host;			/* the beanstalk server hostname or IP address to connect to */
	zval		port;			/* the port of the server to connect to */
	zval		timeout;		/* timeout in seconds when establishing the connection */
	zval		persistent;		/* whether to make the connection persistent or not */
	zval		usedTube;		/* current used tube */
	zval		watchedTubes;	/* current watched tubes */
	zend_object	zo;
} beanspeak_client_object_t;

BEANSPEAK_INIT_CLASS(Beanspeak_Client);

PHP_METHOD(Beanspeak_Client, __construct);

/* {{{ ARG_INFO */
ZEND_BEGIN_ARG_INFO_EX(arginfo_beanspeak_client_construct_arginfo, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dsn, IS_STRING, 1)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ beanspeak_client_method_entry */
BEANSPEAK_INIT_FUNCS(beanspeak_client_method_entry) {
	PHP_ME(Beanspeak_Client, __construct, arginfo_beanspeak_client_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
/* }}} */
