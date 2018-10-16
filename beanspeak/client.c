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

BEANSPEAK_INIT_CLASS(Beanspeak_Client) {
	return SUCCESS;
}
