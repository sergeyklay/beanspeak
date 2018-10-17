#!/usr/bin/env bash
#
# This file is part of the beanspeak extension for PHP.
#
# (c) Serghei Iakovlev <sadhooklay@gmail.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../")

shopt -s nullglob

export NO_INTERACTION=1
export REPORT_EXIT_STATUS=1
export ZEND_DONT_UNLOAD_MODULES=1
export USE_ZEND_ALLOC=0

PHP_MAJOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 1)"
PHP_MINOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 2)"

if [ ${PHP_MAJOR}.${PHP_MINOR} = "clang" ]; then
	make -C "${PROJECT_ROOT}/tests" TESTS=-m test
else
	make -C "${PROJECT_ROOT}/tests" TESTS=-m test
fi

# PHP 7.3 - 7.4 sill have memory leaks
[[ "${PHP_MAJOR}.${PHP_MINOR}" = "7.4" ]] ||  [[ "${PHP_MAJOR}.${PHP_MINOR}" = "7.3" ]] || make -C "${PROJECT_ROOT}/tests" TESTS=-m test && exit $?


make -C "${PROJECT_ROOT}/tests" test
