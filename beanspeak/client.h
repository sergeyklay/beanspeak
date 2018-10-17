/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

extern zend_class_entry *beanspeak_client_ce;

typedef struct {
	zval		socket;
	zval		options;
	zval		usedTube;
	zval		watchedTubes;
	zend_object	zo;
} Beanspeak_Client_object;

BEANSPEAK_INIT_CLASS(Beanspeak_Client);

BEANSPEAK_INIT_FUNCS(beanspeak_client_method_entry) {
	PHP_FE_END
};
