dnl Process this file with autoconf to produce a configure script.

AC_INIT([src/gauss.cc])

dnl Avoid all the -D stuff
AM_CONFIG_HEADER(config.h)

dnl automake stuff

AM_INIT_AUTOMAKE(subs, 1.4.37)

AC_PROG_MAKE_SET

dnl PGPLOT libraries

PGPLOT_LIBS=`./pgplot_libs`

dnl Compiler checks
AC_PROG_CXX
AC_PROG_CC

dnl Initialise libtool
AC_PROG_LIBTOOL

dnl Make C++ the default

AC_LANG_CPLUSPLUS

AC_REQUIRE_CPP

dnl Checks for libraries.

AC_CHECK_LIB([csla], [main], [], AC_MSG_ERROR(cannot link to the sla C library))

AC_CHECK_LIB([pcrecpp], [main], [], AC_MSG_ERROR(cannot link to the pcre C++ library))

AC_CHECK_LIB([cpgplot], [main], [LIBS="$PGPLOT_LIBS $LIBS"], [AC_MSG_ERROR(cannot link to the pgplot libraries)], [$PGPLOT_LIBS])

dnl Checks for ANSI C header files.
AC_CHECK_HEADERS([stdlib.h float.h math.h sstream string fstream iostream], [], AC_MSG_ERROR(missing header; please fix))

AC_CHECK_HEADERS([pcrecpp.h slalib.h], [], AC_MSG_ERROR(missing header; please fix))

dnl Installation program
AC_PROG_INSTALL

dnl The Makefiles to create

AC_OUTPUT([Makefile src/Makefile doc/Makefile include/Makefile]) 
