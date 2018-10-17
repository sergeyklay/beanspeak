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
#include "class_entries.h"

static zend_object* create_object(zend_class_entry* ce)
{
	// TODO
}

BEANSPEAK_INIT_CLASS(Beanspeak_Client) {
	BEANSPEAK_REGISTER_CLASS(Beanspeak, Client, beanspeak, client, beanspeak_client_method_entry, 0);

	zend_declare_property_null(beanspeak_client_ce, ZEND_STRL("socket"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(beanspeak_client_ce, ZEND_STRL("options"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(beanspeak_client_ce, ZEND_STRL("usedTube"), "default", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(beanspeak_client_ce, ZEND_STRL("watchedTubes"), ZEND_ACC_PROTECTED TSRMLS_CC);

	// beanspeak_client_ce->create_object = create_object;

	return SUCCESS;
}

