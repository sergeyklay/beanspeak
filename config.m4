dnl config.m4 for extension beanspeak

PHP_ARG_ENABLE(beanspeak, whether to enable beanspeak support,
[  --enable-beanspeak           Enable beanspeak support])

if test "$PHP_BEANSPEAK" != "no"; then
  AC_DEFINE(HAVE_BEANSPEAK, 1, [Whether you have beanspeak])

  PHP_BEANSPEAK_CFLAGS="-I@ext_srcdir@/beanspeak"

  EXT_BEANSPEAK_SOURCES="beanspeak.c beanspeak/client.c"
  EXT_BEANSPEAK_HEADERS="php_beanspeak.h beanspeak/client.h beanspeak/helpers.h"

  PHP_NEW_EXTENSION(beanspeak, $EXT_BEANSPEAK_SOURCES, $ext_shared,, $PHP_BEANSPEAK_CFLAGS)
  ifdef([PHP_INSTALL_HEADERS], [
  	PHP_INSTALL_HEADERS([ext/beanspeak], $EXT_BEANSPEAK_HEADERS)
  ])
fi
