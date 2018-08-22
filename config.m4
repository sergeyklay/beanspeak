dnl $Id$
dnl config.m4 for extension beanspeak

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(beanspeak, for beanspeak support,
dnl Make sure that the comment is aligned:
dnl [  --with-beanspeak             Include beanspeak support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(beanspeak, whether to enable beanspeak support,
dnl Make sure that the comment is aligned:
dnl [  --enable-beanspeak           Enable beanspeak support])

if test "$PHP_BEANSPEAK" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-beanspeak -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/beanspeak.h"  # you most likely want to change this
  dnl if test -r $PHP_BEANSPEAK/$SEARCH_FOR; then # path given as parameter
  dnl   BEANSPEAK_DIR=$PHP_BEANSPEAK
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for beanspeak files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BEANSPEAK_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BEANSPEAK_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the beanspeak distribution])
  dnl fi

  dnl # --with-beanspeak -> add include path
  dnl PHP_ADD_INCLUDE($BEANSPEAK_DIR/include)

  dnl # --with-beanspeak -> check for lib and symbol presence
  dnl LIBNAME=beanspeak # you may want to change this
  dnl LIBSYMBOL=beanspeak # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BEANSPEAK_DIR/$PHP_LIBDIR, BEANSPEAK_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BEANSPEAKLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong beanspeak lib version or lib not found])
  dnl ],[
  dnl   -L$BEANSPEAK_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(BEANSPEAK_SHARED_LIBADD)

  PHP_NEW_EXTENSION(beanspeak, beanspeak.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
