AC_INIT

AC_CONFIG_AUX_DIR(makeit/scripts)
AC_CANONICAL_BUILD

AC_CONFIG_HEADER(include/OSE/config.h)

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

AC_PROG_CPP

AC_STRUCT_ST_BLKSIZE
AC_STRUCT_ST_BLOCKS
AC_STRUCT_ST_RDEV

AC_HEADER_DIRENT

AC_CHECK_HEADERS(memory.h)
AC_CHECK_HEADERS(stropts.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(sys/resource.h)
AC_CHECK_HEADERS(sys/elf.h)
AC_CHECK_HEADERS(sys/select.h)
AC_CHECK_HEADERS(sys/param.h)

AC_CHECK_FUNCS(getcwd)
AC_CHECK_FUNCS(getdtablesize)
AC_CHECK_FUNCS(getpid)
AC_CHECK_FUNCS(gettimeofday)
AC_CHECK_FUNCS(nl_langinfo)
AC_CHECK_FUNCS(poll)
AC_CHECK_FUNCS(select)
AC_CHECK_FUNCS(sigprocmask)
AC_CHECK_FUNCS(sigaction)
AC_CHECK_FUNCS(strcoll)
AC_CHECK_FUNCS(strtoll)
AC_CHECK_FUNCS(strtold)
AC_CHECK_FUNCS(sysconf)
AC_CHECK_FUNCS(uname)

AC_FUNC_STRCOLL

AC_MSG_CHECKING(for sysconf (_SC_OPEN_MAX))
AC_CACHE_VAL(my_cv_sysconf_sc_open_max, [dnl
AC_TRY_LINK([#include <unistd.h>], [int max = sysconf (_SC_OPEN_MAX);],
            [my_cv_sysconf_sc_open_max=yes], [my_cv_sysconf_sc_open_max=no])])
if test $my_cv_sysconf_sc_open_max = yes; then
  AC_DEFINE(HAVE_SYSCONF_SC_OPEN_MAX)
fi
AC_MSG_RESULT($my_cv_sysconf_sc_open_max)

AC_DEFUN(MY_SUBST_TYPE,
[AC_MSG_CHECKING([for $1])
AC_CACHE_VAL(my_cv_type_$1,
changequote(,)dnl
trfrom='[a-z]' trto='[A-Z]'
changequote([,])dnl
AC_HEADER_EGREP($1, sys/types.h,
eval "my_cv_type_$1=yes",
eval "my_cv_type_$1=no"
))
if eval "test \"`echo '$my_cv_type_'$1`\" = yes"; then
  AC_MSG_RESULT(yes)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])

AC_DEFUN(MY_SUBST_TYPES,
[for my_type in $1
do
MY_SUBST_TYPE($my_type,$2,
[changequote(, )dnl
  my_tr_type=NEED_`echo $my_type | tr '[a-z]' '[A-Z]'`
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($my_tr_type)] $3)dnl
done
])

MY_SUBST_TYPES(uid_t)
MY_SUBST_TYPES(pid_t)
MY_SUBST_TYPES(off_t)
MY_SUBST_TYPES(dev_t)
MY_SUBST_TYPES(ino_t)
MY_SUBST_TYPES(u_char)
MY_SUBST_TYPES(u_short)
MY_SUBST_TYPES(u_int)
MY_SUBST_TYPES(u_long)

AC_DEFUN(MY_SUBST_SOCKET_TYPE,
[AC_MSG_CHECKING([for $1])
AC_CACHE_VAL(my_cv_type_$1,
changequote(,)dnl
trfrom='[a-z]' trto='[A-Z]'
changequote([,])dnl
AC_HEADER_EGREP($1, sys/socket.h,
eval "my_cv_type_$1=yes",
eval "my_cv_type_$1=no"
))
if eval "test \"`echo '$my_cv_type_'$1`\" = yes"; then
  AC_MSG_RESULT(yes)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])

AC_DEFUN(MY_SUBST_SOCKET_TYPES,
[for my_type in $1
do
MY_SUBST_SOCKET_TYPE($my_type,$2,
[changequote(, )dnl
  my_tr_type=NEED_`echo $my_type | tr '[a-z]' '[A-Z]'`
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($my_tr_type)] $3)dnl
done
])

MY_SUBST_SOCKET_TYPES(socklen_t)

AC_CHECK_SIZEOF(double)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(long double)

AC_OUTPUT(config.mk include/OSE/version.h modules/ose.mk modules/ose.pkg)

# [ echo cp include/OSE/config.h include/OSE/${ac_cv_build_alias}.h]
# [ cp include/OSE/config.h include/OSE/${ac_cv_build_alias}.h]
