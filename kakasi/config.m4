dnl $Id$
dnl config.m4 for extension kakasi

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(kakasi, for kakasi support,
Make sure that the comment is aligned:
[  --with-kakasi             Include kakasi support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(kakasi, whether to enable kakasi support,
dnl Make sure that the comment is aligned:
dnl [  --enable-kakasi           Enable kakasi support])

CC=/usr/bin/gcc
CFLAGS="-O3 -lkakasi"

if test "$PHP_KAKASI" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-kakasi -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/kakasi.h"  # you most likely want to change this
  dnl if test -r $PHP_KAKASI/$SEARCH_FOR; then # path given as parameter
  dnl   KAKASI_DIR=$PHP_KAKASI
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for kakasi files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       KAKASI_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$KAKASI_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the kakasi distribution])
  dnl fi

  dnl # --with-kakasi -> add include path
  dnl PHP_ADD_INCLUDE($KAKASI_DIR/include)

  dnl # --with-kakasi -> check for lib and symbol presence
  dnl LIBNAME=kakasi # you may want to change this
  dnl LIBSYMBOL=kakasi # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $KAKASI_DIR/lib, KAKASI_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_KAKASILIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong kakasi lib version or lib not found])
  dnl ],[
  dnl   -L$KAKASI_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(KAKASI_SHARED_LIBADD)

  PHP_NEW_EXTENSION(kakasi, kakasi.c, $ext_shared)
fi
