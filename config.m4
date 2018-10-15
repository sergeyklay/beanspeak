dnl config.m4 for extension beanspeak

PHP_ARG_ENABLE(beanspeak, whether to enable beanspeak support,
[  --enable-beanspeak           Enable beanspeak support])

if test "$PHP_BEANSPEAK" != "no"; then
  AC_DEFINE(HAVE_BEANSPEAK, 1, [Whether you have beanspeak])
  PHP_NEW_EXTENSION(beanspeak, beanspeak.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
