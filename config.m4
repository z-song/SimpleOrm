dnl $Id$
dnl config.m4 for extension SimpleOrm

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(SimpleOrm, for SimpleOrm support,
dnl Make sure that the comment is aligned:
dnl [  --with-SimpleOrm             Include SimpleOrm support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(SimpleOrm, whether to enable SimpleOrm support,
Make sure that the comment is aligned:
[  --enable-SimpleOrm           Enable SimpleOrm support])

if test "$PHP_SIMPLEORM" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-SimpleOrm -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/SimpleOrm.h"  # you most likely want to change this
  dnl if test -r $PHP_SIMPLEORM/$SEARCH_FOR; then # path given as parameter
  dnl   SIMPLEORM_DIR=$PHP_SIMPLEORM
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for SimpleOrm files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SIMPLEORM_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SIMPLEORM_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the SimpleOrm distribution])
  dnl fi

  dnl # --with-SimpleOrm -> add include path
  dnl PHP_ADD_INCLUDE($SIMPLEORM_DIR/include)

  dnl # --with-SimpleOrm -> check for lib and symbol presence
  dnl LIBNAME=SimpleOrm # you may want to change this
  dnl LIBSYMBOL=SimpleOrm # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SIMPLEORM_DIR/lib, SIMPLEORM_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SIMPLEORMLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong SimpleOrm lib version or lib not found])
  dnl ],[
  dnl   -L$SIMPLEORM_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SIMPLEORM_SHARED_LIBADD)

  PHP_NEW_EXTENSION(SimpleOrm, SimpleOrm.c, $ext_shared)
fi
