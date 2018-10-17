#!/usr/bin/env bash
#
# This file is part of the beanspeak extension for PHP.
#
# (c) Serghei Iakovlev <sadhooklay@gmail.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

$(phpenv which php) -v
$(phpenv which php) -m

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../")

for i in `find ${PROJECT_ROOT}/tests -name "*.out" 2>/dev/null`; do
	echo "-- START ${i}"; cat ${i}; echo "-- END";
done

for i in `find ${PROJECT_ROOT}/tests -name "*.mem" 2>/dev/null`; do
	echo "-- START ${i}"; cat ${i}; echo "-- END";
done

shopt -s nullglob

export LC_ALL=C

for i in core core.*; do
	if [ -f "$i" -a "$(file "$i" | grep -o 'core file')" ]; then
		gdb -q $(file "${i}" | grep -oE "'[^ ']+" | sed "s/^'//g") "$i" <<EOF
set pagination 0
backtrace full
info registers
x/16i \$pc
thread apply all backtrace
quit
EOF
	fi
done
