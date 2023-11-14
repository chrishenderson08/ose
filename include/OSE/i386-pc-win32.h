/* config.h.  Generated automatically by configure.  */
#ifndef OTC_ENVIRON_CONFIG_H
#define OTC_ENVIRON_CONFIG_H

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OSE_DEPENDENCY_MARKER;

/* ------------------------------------------------------------------------- */

/* Directory access. */

/* #undef HAVE_DIRENT_H */
/* #undef HAVE_SYS_NDIR_H */
/* #undef HAVE_SYS_DIR_H */
/* #undef HAVE_NDIR_H */

/* C header files. */

#define HAVE_MEMORY_H 1
/* #undef HAVE_STROPTS_H */
/* #undef HAVE_UNISTD_H */
/* #undef HAVE_SYS_RESOURCE_H */
/* #undef HAVE_SYS_ELF_H */
/* #undef HAVE_SYS_SELECT_H */
/* #undef HAVE_SYS_PARAM_H */

/* Functions we do have. */

#define HAVE_GETCWD 1
/* #undef HAVE_GETDTABLESIZE */
#define HAVE_GETPID 1
/* #undef HAVE_GETTIMEOFDAY */
/* #undef HAVE_NL_LANGINFO */
/* #undef HAVE_POLL */
/* #undef HAVE_SELECT */
/* #undef HAVE_SIGPROCMASK */
/* #undef HAVE_SIGACTION */
#define HAVE_STRCOLL 1
/* #undef HAVE_SYSCONF */
/* #undef HAVE_UNAME */

/* Functions we do not have. */

/* #undef NEED_PTHREAD_RWLOCK_INIT */

/* Options that sysconf accepts. */

/* #undef HAVE_SYSCONF_SC_OPEN_MAX */

/* Types we do not have. */

#define NEED_UID_T 1
#define NEED_PID_T 1
/* #undef NEED_OFF_T */
/* #undef NEED_DEV_T */
/* #undef NEED_INO_T */
#define NEED_U_CHAR 1
#define NEED_U_SHORT 1
#define NEED_U_INT 1
#define NEED_U_LONG 1

#define NEED_SOCKLEN_T 1

/* Parts of the stat structure we have. */

/* #undef HAVE_ST_BLKSIZE */
/* #undef HAVE_ST_BLOCKS */
#define HAVE_ST_RDEV 1

/* Size of standard types. */

#define SIZEOF_DOUBLE 8
#define SIZEOF_LONG_LONG 0
#define SIZEOF_LONG_DOUBLE 8

/* ------------------------------------------------------------------------- */

#endif /* OTC_ENVIRON_CONFIG_H */
