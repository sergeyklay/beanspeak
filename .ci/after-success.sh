#!/usr/bin/env bash
#
# This file is part of the beanspeak extension for PHP.
#
# (c) Serghei Iakovlev <sadhooklay@gmail.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../")

if [ "${REPORT_COVERAGE}" = "1" ]; then
	lcov --no-checksum --directory ${PROJECT_ROOT}/beanspeak --directory ${PROJECT_ROOT} --capture --compat-libtool --output-file coverage.info
	lcov --remove coverage.info "/usr*" --remove coverage.info "*/.phpenv/*" --remove coverage.info "/home/travis/build/include/*" --compat-libtool --output-file coverage.info
	coveralls-lcov coverage.info
fi
