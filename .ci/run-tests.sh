#!/usr/bin/env bash
#
# This file is part of the beanspeak extension for PHP.
#
# (c) Serghei Iakovlev <sadhooklay@gmail.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

PHP_VERNUM="$(`phpenv which php-config` --vernum)"

shopt -s nullglob

export NO_INTERACTION=1
export REPORT_EXIT_STATUS=1
export ZEND_DONT_UNLOAD_MODULES=1
export USE_ZEND_ALLOC=0

if [ "${PHP_VERNUM}" -lt 70300 ]; then
	if [ $(command -v valgrind 2>/dev/null) != "" ]; then
		export TEST_PHP_ARGS=-m
	else
		>&2 echo "Skip check for memory leaks. Valgring does not exist"
	fi
else
	>&2 echo "Skip check for memory leaks due to unstable PHP version"
fi

php run-tests.php \
	-d extension=beanspeak.so \
	-d extension_dir=modules \
	-n \
	-g "FAIL,XFAIL,SKIP,BORK,WARN,LEAK" \
	-p $(phpenv which php) \
	tests

exit $?
