AC_INIT(src/oroborus.c)
dnl Grab the package and version from the top debian/changelog entry.
AM_INIT_AUTOMAKE(`sed 's/ .*$//; q' debian/changelog`, `sed 's/^.*(\(.*\)).*$/\1/; q' debian/changelog`)

dnl create a src/config.h file (Automake will add -DHAVE_CONFIG_H)
AM_CONFIG_HEADER(src/config.h)

AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

YEAR=`date +%Y`
AC_DEFINE_UNQUOTED(YEAR, $YEAR, [year when build])

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC

AC_PATH_XTRA
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/param.h sys/time.h time.h sys/mkdev.h sys/sysmacros.h string.h memory.h fcntl.h dirent.h sys/ndir.h ndir.h alloca.h locale.h )

AC_HEADER_MAJOR
AC_FUNC_ALLOCA
AC_STRUCT_TM
AC_STRUCT_ST_BLOCKS
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS(mkfifo)
AC_CHECK_FUNC(mknod)

AC_CHECK_LIB(X11, main,, AC_MSG_ERROR([libX11 is missing from your system: Make sure you have both the  X Window System libraries and development headers correctly installed]), $X_CFLAGS $X_PRE_LIBS $X_EXTRA_LIBS $X_LIBS)
AC_CHECK_LIB(Xext, main,, AC_MSG_ERROR([The X Window System installed on this machine does not support the shape extension]), $X_CFLAGS $X_PRE_LIBS $X_EXTRA_LIBS $X_LIBS)
AC_CHECK_LIB(SM, main,AC_DEFINE(HAVE_X11_SM_SMLIB_H, 1, [Have X11 SM lib ?]), AC_MSG_ERROR([The X Window System installed on this machine does not support the session management extension]), $X_CFLAGS $X_PRE_LIBS $X_EXTRA_LIBS $X_LIBS)
AC_CHECK_LIB(Xpm, main,, AC_MSG_ERROR([libXpm is missing from your system: Make sure you have both the Xpm libraries and development headers correctly installed]), $X_CFLAGS $X_PRE_LIBS $X_EXTRA_LIBS $X_LIBS)

AC_CHECK_FUNCS(malloc setlocale strchr strdup strrchr strstr)

AC_ARG_ENABLE(debug, [  --enable-debug          use this option if you want to debug Oroborus], AC_DEFINE(DEBUG, 1, [use this option if you want to debug Oroborus]))

AC_OUTPUT(Makefile src/Makefile defaults/defaults defaults/Makefile schemes/Makefile themes/Makefile themes/cruxish/Makefile themes/next/Makefile themes/oroborus/Makefile themes/platinum/Makefile themes/slimline/Makefile themes/windows/Makefile themes/agua/Makefile themes/QNX/Makefile themes/e017/Makefile themes/pillage/Makefile themes/gorillaworm/Makefile themes/beos/Makefile man/Makefile themes/defold/Makefile themes/mkultra/Makefile themes/bluecurve/Makefile themes/Elberg_Blue/Makefile themes/Elberg_Green/Makefile themes/Elberg_Red/Makefile )
