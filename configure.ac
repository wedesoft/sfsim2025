dnl Run `./autogen.sh` to generate a configure script.
dnl Then run `./configure` to generate a Makefile.
dnl Finally run `make` to build the project.
AC_PREREQ([2.69])
AC_INIT([sfsim2025], [0.1.0], [jan@wedesoft.de])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([--warnings=no-portability foreign])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AC_PROG_CC
AC_PROG_INSTALL
AC_LANG(C)
PKG_PROG_PKG_CONFIG
AM_CONFIG_HEADER(config.h)

AC_CHECK_LIB([m], [sqrt])

PKG_CHECK_MODULES(BOEHM, bdw-gc >= 7.4.2)
AC_SUBST(BOEHM_CFLAGS)
AC_SUBST(BOEHM_LIBS)

PKG_CHECK_MODULES(ATLAS, lapack-atlas >= 3.10.3)
AC_SUBST(ATLAS_CFLAGS)
AC_SUBST(ATLAS_LIBS)

PKG_CHECK_MODULES(SDL, sdl2 >= 2.0.8)
AC_SUBST(SDL_CFLAGS)
AC_SUBST(SDL_LIBS)

PKG_CHECK_MODULES(MAGICK, ImageMagick >= 6.9.10)
AC_SUBST(MAGICK_CFLAGS)
AC_SUBST(MAGICK_LIBS)

AX_HAVE_OPENGL
AC_MSG_CHECKING([for GL])
if test "x$have_GL" != "xyes"; then
  AC_MSG_RESULT([no])
  AC_MSG_ERROR([Could not find GL])
else
  AC_MSG_RESULT([yes])
fi
AC_MSG_CHECKING([for GLU])
if test "x$have_GLU" != "xyes"; then
  AC_MSG_RESULT([no])
  AC_MSG_ERROR([Could not find GLU])
else
  AC_MSG_RESULT([yes])
fi
AC_MSG_CHECKING([for glut])
if test "x$have_glut" != "xyes"; then
  AC_MSG_RESULT([no])
  AC_MSG_ERROR([Could not find glut])
else
  AC_MSG_RESULT([yes])
fi

PKG_CHECK_MODULES(GLEW, glew >= 2.0.0)
AC_SUBST(GLEW_CFLAGS)
AC_SUBST(GLEW_LIBS)

AC_OUTPUT(Makefile
          sfsim2025/Makefile
          tests/Makefile)
