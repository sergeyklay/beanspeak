/*
 * This file is part of the beanspeak extension for PHP.
 *
 * (c) Serghei Iakovlev <sadhooklay@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PHP_BEANSPEAK_BEANSPEAK_HELPERS_H
# define PHP_BEANSPEAK_BEANSPEAK_HELPERS_H 1

# define BEANSPEAK_INIT_CLASS(name) \
	int beanspeak_ ##name## _init(INIT_FUNC_ARGS)

# define BEANSPEAK_INIT(name) \
	if (beanspeak_ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) { \
		return FAILURE; \
	}

#endif /* PHP_BEANSPEAK_BEANSPEAK_HELPERS_H */
