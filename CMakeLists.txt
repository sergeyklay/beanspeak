cmake_minimum_required(VERSION 3.6)
project(beanspeak C)

message("Begin cmaking of PHP extension ...")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -O0 -Wall -fvisibility=hidden")

# NOTE: This CMake file is just for syntax highlighting in CLion
include_directories(
    ~/src/php/7.3.0RC3
    ~/src/php/7.3.0RC3/main
    ~/src/php/7.3.0RC3/Zend
    ~/src/php/7.3.0RC3/TSRM
    ~/src/php/7.3.0RC3/ext
    ~/src/php/7.3.0RC3/sapi
)

FILE(GLOB BeanspeakHeaders ./beanspeak/*.h)
FILE(GLOB BeanspeakSources ./beanspeak/*.c)

set(SOURCE_FILES
	beanspeak.c
    php_beanspeak.h
	${BeanspeakHeaders}
	${BeanspeakSources}
)

add_library(beanspeak ${SOURCE_FILES})

message("End cmaking of PHP extension ...")
