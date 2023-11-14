#ifndef OTC_SYSTEM_STAT_HH
#define OTC_SYSTEM_STAT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/stat.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991-1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/stat.hh"
#endif
#endif

#if !defined(S_IREAD) && defined(S_IRUSR)
#define S_IREAD S_IRUSR
#endif
#if !defined(S_IWRITE) && defined(S_IWUSR)
#define S_IWRITE S_IWUSR
#endif
#if !defined(S_IEXEC) && defined(S_IXUSR)
#define S_IEXEC S_IXUSR
#endif

#if !defined(S_IFMT) && defined(_IFMT)
#define S_IFMT _IFMT
#endif
#if !defined(S_IFDIR) && defined(_IFDIR)
#define S_IFDIR _IFDIR
#endif
#if !defined(S_IFREG) && defined(_IFREG)
#define S_IFREG _IFREG
#endif
#if !defined(S_IFCHR) && defined(_IFCHR)
#define S_IFCHR _IFCHR
#endif

#if !defined(S_IFMT) && defined(_S_IFMT)
#define S_IFMT _S_IFMT
#endif
#if !defined(S_IFDIR) && defined(_S_IFDIR)
#define S_IFDIR _S_IFDIR
#endif
#if !defined(S_IFREG) && defined(_S_IFREG)
#define S_IFREG _S_IFREG
#endif
#if !defined(S_IFCHR) && defined(_S_IFCHR)
#define S_IFCHR _S_IFCHR
#endif

/* ------------------------------------------------------------------------- */

typedef struct stat otclib_stat;

class OSE_EXPORT OTC_Stat
    // = TITLE
    //	   Class wrapper around the system stat structure.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DECSCRIPTION
    //	   Encapsulates the system stat structure to make deriving information
    //	   about a file easier. Note that it is still necessary to use the
    //	   system <stat()> function to fill out information in the class.
    //
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //	   OTC_Stat info;
    //	   if (stat("/tmp",info) == 0)
    //	   {
    //	     if (info.isDirectory())
    //	       cout << "Is a directory" << endl;
    //	   }
    // = END<CODE>
    //
    // = SEE ALSO
    //	   <stat(2)>
{
  public:

			operator otclib_stat*()
				{ return &stat_; }

			OTC_Stat();

			OTC_Stat(OTC_Stat const& theStat);

    OTC_Stat&		operator=(OTC_Stat const& theStat);

    // = ATTRIBUTES

    dev_t		dev() const
				{ return stat_.st_dev; }
				// Returns the device id for device which
				// the file resides upon.

    u_short		mode() const
				{ return stat_.st_mode; }
				// Returns the files mode bits.

    u_short		permissions() const
				{ return u_short(mode() & 0777); }
				// Returns the bits of the mode
				// giving the file permissions.

    u_short		type() const
				{ return u_short(mode() & S_IFMT); }
				// Returns the bits of the mode
				// giving the file type.

    off_t		size() const
				{ return stat_.st_size; }
				// Returns the size of the file.

    time_t		atime() const
				{ return stat_.st_atime; }
				// Returns the last access time of the file.

    time_t		mtime() const
				{ return stat_.st_mtime; }
				// Returns the last modification time of the
				// file.

    time_t		ctime() const
				{ return stat_.st_ctime; }
				// Returns the last time which the status of
				// the file was changed.

    // The following simplify questions commonly asked about files.

    bool		isDirectory() const
				{ return (type() & S_IFDIR) == S_IFDIR; }
				// Returns <true> if file is a directory.

    bool		isRegularFile() const
				{ return (type() & S_IFREG) == S_IFREG; }
				// Returns <true> if file is a regular file.

    bool		isCharacterSpecial() const
				{ return (type() & S_IFCHR) == S_IFCHR; }
				// Returns <true> if file is a character
				// special file.

#if defined(S_IFIFO)
    bool		isFifo() const
				{ return (type() & S_IFIFO) == S_IFIFO; }
				// Returns <true> if file is a fifo.
#endif

#if defined(S_IFBLK)
    bool		isBlockSpecial() const
				{ return (type() & S_IFBLK) == S_IFBLK; }
				// Returns <true> if file is a block special
				// file.
#endif

    bool		isUserRead() const
				{ return (mode() & S_IREAD) != 0; }
				// Returns <false> if not readable by
				// the user who owns the file.

    bool		isUserWrite() const
				{ return (mode() & S_IWRITE) != 0; }
				// Returns <false> if not writable by
				// the user who owns the file.

    bool		isUserExec() const
				{ return (mode() & S_IEXEC) != 0; }
				// Returns <false> if not executable
				// by the user who owns the file.

#if defined(OSE_SYS_UNIX)

    // Following functions are only available on UNIX like systems.
    // Some functions are not available on older SystemV UNIX systems.

    ino_t		ino() const
				{ return stat_.st_ino; }
				// Returns the inode of the file.

    short		nlink() const
				{ return stat_.st_nlink; }
				// Returns the number of hardlinks to the
				// file.

    uid_t		uid() const
				{ return stat_.st_uid; }
				// Returns the owners user id.

    gid_t		gid() const
				{ return stat_.st_gid; }
				// Returns the group user id.

#ifdef HAVE_ST_RDEV
    dev_t		rdev() const
				{ return stat_.st_rdev; }
				// Returns the device type.
#endif

#ifdef HAVE_ST_BLKSIZE
    long		blksize() const
				{ return stat_.st_blksize; }
				// Returns the optimal blocksize for file
				// system i/o operations.
#endif

#ifdef HAVE_ST_BLOCKS
    long		blocks() const
				{ return stat_.st_blocks; }
				// Returns the number of blocks allocated to
				// the file.
#endif

    // The following simplify questions commonly asked about files.

#ifdef S_IFLNK
    bool		isSymbolicLink() const
				{ return (type() & S_IFLNK) == S_IFLNK; }
				// Returns <true> if file is a symbolic link.
#endif

#ifdef S_IFSOCK
    bool		isSocket() const
				{ return (type() & S_IFSOCK) == S_IFSOCK; }
				// Returns <true> if file is a socket.
#endif

    bool		isSetUid() const
				{ return (mode() & S_ISUID) == S_ISUID; }
				// Returns <true> if file is setuid.

    bool		isSetGid() const
				{ return (mode() & S_ISGID) == S_ISGID; }
				// Returns <true> if file is setgid.

    bool		isSaveTxt() const
				{ return (mode() & S_ISVTX) == S_ISVTX; }
				// Returns <true> if file is has its
				// stickybit set.

    bool		isGroupRead() const
				{ return (mode() & 0040) != 0; }
				// Returns <false> if not readable by
				// the users in the group of the file.

    bool		isGroupWrite() const
				{ return (mode() & 0020) != 0; }
				// Returns <false> if not writable by
				// the users in the group of the file.

    bool		isGroupExec() const
				{ return (mode() & 0010) != 0; }
				// Returns <false> if not executable
				// by the users in the group of the file.

    bool		isOtherRead() const
				{ return (mode() & 0004) != 0; }
				// Returns <false> if not readable by
				// any users.

    bool		isOtherWrite() const
				{ return (mode() & 0002) != 0; }
				// Returns <false> if not writable by
				// any users.

    bool		isOtherExec() const
				{ return (mode() & 0001) != 0; }
				// Returns <false> if not executable by
				// any users.

#endif

  private:

    otclib_stat		stat_;
				// Stat structure.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_STAT_HH */
