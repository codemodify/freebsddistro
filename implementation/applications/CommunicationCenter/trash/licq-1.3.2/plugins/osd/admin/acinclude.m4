## -*- mode: m4 -*-
dnl Copyright (c) 2000 Dirk Mueller <dirk@licq.org>

dnl Check for sys_errlist[] and sys_nerr, check for declaration
dnl Check nicked from aclocal.m4 used by GNU bash 2.01
AC_DEFUN(LICQ_SYS_ERRLIST,
[AC_MSG_CHECKING([for sys_errlist and sys_nerr])
AC_CACHE_VAL(ac_cv_sys_errlist,
[AC_TRY_LINK([#include <errno.h>],
[extern char *sys_errlist[];
 extern int sys_nerr;
 char *msg = sys_errlist[sys_nerr - 1];],
    ac_cv_sys_errlist=yes, ac_cv_sys_errlist=no)])dnl
AC_MSG_RESULT($ac_cv_sys_errlist)
if test $ac_cv_sys_errlist = yes; then
  AC_DEFINE_UNQUOTED(HAVE_SYS_ERRLIST, 1, [Define if sys_errlist available])
fi
])

AC_DEFUN(LICQ_FIND_FILE,
[
$3=""
for i in $2;
do
  for j in $1;
  do
    if test -r "$i/$j"; then
      $3=$i
      break 2
    fi
  done
done
])

AC_DEFUN(LICQ_CHECK_GETHOSTBYNAME,
[
  AC_CHECK_LIB(nsl, gethostbyname, LIBS="$LIBS -lnsl")
dnl             AC_CHECK_FUNC(gethostbyname, ,AC_MSG_ERROR([Unable to find gethostbyname on your system!])))
])

AC_DEFUN(LICQ_CHECK_LICQ,
[
  AC_MSG_CHECKING([for licq header files])

  AC_ARG_WITH(licq-includes,
    [  --with-licq-includes    where the Licq daemon includes are located.],
    [  licq_licq_includes="$withval" ])

  if test -n "$licq_licq_includes"; then
    relpath="`(cd $licq_licq_includes && pwd) 2>/dev/null`"
    if test -n "$relpath"; then
      licq_licq_includes="$relpath"
    fi
    LICQ_FIND_FILE(licq_icqd.h, $licq_licq_includes, licq_inc)
  else
    licq_paths="$prefix/include/licq \
                /usr/local/include/licq \
                /usr/include/licq"

    relpath=`cd $srcdir/../../include && pwd`

    if test -r $relpath/licq_icqd.h; then
      LICQ_FIND_FILE(licq_icqd.h, $relpath, licq_inc)
    else
      LICQ_FIND_FILE(licq_icqd.h, $licq_paths, licq_inc)
    fi
  fi

  if test -z "$licq_inc"; then
    have_licq_inc=no
  else
    have_licq_inc=yes
  fi
  if test "$have_licq_inc" = yes; then
    AC_MSG_RESULT(["$licq_inc"])
    LICQ_INCLUDES="$licq_inc"
  else
    AC_MSG_RESULT(["not found"])
  fi
  
  AC_SUBST(LICQ_INCLUDES)

  if test "$have_licq_inc" = "no"; then
    AC_MSG_ERROR([
      I can't find the Licq header files. Make sure that you ran
      "make install" for the Licq daemon or installed the licq header
      files RPM.

      Most likely you just need to set --prefix to the correct value.

      You could also try the --with-licq-includes=<path> configure switch to
      overwrite this check if you have installed them in an unusual place.
    ])
  fi
])

dnl Check if it is possible to turn off exceptions/rtti support
AC_DEFUN(LICQ_CXX_FLAGS,
[AC_CACHE_CHECK(whether ${CXX-g++} accepts -fno-rtti -fno-exceptions -fno-check-new, licq_prog_cxx_fno_exceptions,
[echo 'void f(){}' > conftest.cc
if test -z "`${CXX-g++} -fno-rtti -fno-exceptions -fno-check-new -c conftest.cc 2>&1`"; then
  licq_prog_cxx_fno_exceptions=yes
else
  licq_prog_cxx_fno_exceptions=no
fi
rm -f conftest*
])

if test "$licq_prog_cxx_fno_exceptions" = "yes"; then
  CXXFLAGS="${CXXFLAGS} -fno-rtti -fno-exceptions -fno-check-new"
fi

  AC_ARG_ENABLE(debug,
    [  --enable-debug          creates debugging code],
    [ licq_use_debug="$enableval"], [licq_use_debug="no"])

if test "$licq_use_debug" = "no"; then
  CXXFLAGS="`echo $CXXFLAGS | sed s/-g//`"
  CFLAGS="`echo $CFLAGS | sed s/-g//`"
fi
])


dnl Check if it is possible to turn off exceptions support
AC_DEFUN(LICQDAEMON_CXX_FLAGS,
[AC_CACHE_CHECK(whether ${CXX-g++} accepts -fno-exceptions -fno-check-new, licqdaemon_prog_cxx_fno_exceptions,
[echo 'void f(){}' > conftest.cc
if test -z "`${CXX-g++} -fno-exceptions -fno-check-new -c conftest.cc 2>&1`"; then
  licqdaemon_prog_cxx_fno_exceptions=yes
else
  licqdaemon_prog_cxx_fno_exceptions=no
fi
rm -f conftest*
])

if test "$licqdaemon_prog_cxx_fno_exceptions" = "yes"; then
  CXXFLAGS="${CXXFLAGS} -fno-exceptions -fno-check-new"
fi

  AC_ARG_ENABLE(debug,
    [  --enable-debug          creates debugging code],
    [ licqdaemon_use_debug="$enableval"], [licqdaemon_use_debug="no"])

if test "$licqdaemon_use_debug" = "no"; then
  CXXFLAGS="`echo $CXXFLAGS | sed s/-g//`"
  CFLAGS="`echo $CFLAGS | sed s/-g//`"
fi
])

dnl Check if the type socklen_t is defined anywhere
AC_DEFUN(AC_C_SOCKLEN_T,
[AC_CACHE_CHECK(for socklen_t, ac_cv_c_socklen_t,
[ 
  AC_TRY_COMPILE([
    #include <sys/types.h>
    #include <sys/socket.h>
  ],[
    socklen_t foo;
  ],[
    ac_cv_c_socklen_t=yes
  ],[
    ac_cv_c_socklen_t=no
  ])
])
if test $ac_cv_c_socklen_t = no; then
  AC_DEFINE(socklen_t, int, [define to int if socklen_t not available])
fi

])

dnl Check if the type socklen_t is defined anywhere
AC_DEFUN(AC_C_SOCKLEN_T_AND_SOCKS,
  [AC_CACHE_CHECK(for SOCKS socklen_t, ac_cv_c_socklen_t,

  ]);

])
