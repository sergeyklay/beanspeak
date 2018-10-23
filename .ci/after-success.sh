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
	output=${PROJECT_ROOT}/coverage.info

	lcov --no-checksum --directory ${PROJECT_ROOT}/beanspeak --directory ${PROJECT_ROOT} --capture --compat-libtool --output-file ${output}

	lcov --remove ${output} "/usr*" \
		--remove ${output} "*/.phpenv/*" \
		--remove ${output} "/home/travis/build/include/*" \
		--compat-libtool \
		--output-file ${output}

	coveralls-lcov ${output}
fi

if [ ! -z "${CODECOV_TOKEN}" ]; then
	curl -sSL https://codecov.io/bash -o ./codecov
	chmod +x ./codecov

	./codecov -s ${PROJECT_ROOT}
else
	echo "Skip uploading code coverage..."
fi
