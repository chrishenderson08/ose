AC_INIT

AC_CONFIG_AUX_DIR(scripts)
AC_CANONICAL_BUILD

AC_SUBST(PLATFORM_HOST)

[ PLATFORM_HOST=${ac_cv_build_alias} ]

AC_SUBST(PACKAGE_NAME)
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(PACKAGE_STRING)

AC_SUBST(PACKAGE_MAJOR_VERSION)
AC_SUBST(PACKAGE_MINOR_VERSION)
AC_SUBST(PACKAGE_PATCH_VERSION)

[ PACKAGE_STRING=`cat PACKAGE | sed -e 's/\n//'` ]

[ PACKAGE_NAME=`echo $PACKAGE_STRING | \
 sed -e 's/^\(.*\)-.*$/\1/'` ]

[ PACKAGE_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-\(.*\)$/\1/'` ]

[ PACKAGE_MAJOR_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-\([0-9][0-9]*\)\..*$/\1/'` ]

[ PACKAGE_MINOR_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-.*\.\([0-9][0-9]*\).*$/\1/' -e 's/^\([0-9]\)$/0\1/'` ]

[ PACKAGE_PATCH_VERSION=`echo $PACKAGE_STRING | \
 sed -e 's/^.*-.*pl\([0-9][0-9]*\)$/\1/' -e 's/^\([0-9]\)$/0\1/' | \
 sed -e "s/^$PACKAGE_STRING$/00/"` ]

AC_PROG_INSTALL

AC_OUTPUT(Makefile)
