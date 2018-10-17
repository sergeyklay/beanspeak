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
	zval		options;		/* connection options */
	zval		usedTube;		/* current used tube */
	zval		watchedTubes;	/* current watched tubes */
	zend_object	zo;
} beanspeak_client_object_t;

BEANSPEAK_INIT_CLASS(Beanspeak_Client);

BEANSPEAK_INIT_FUNCS(beanspeak_client_method_entry) {
	PHP_FE_END
};
