/****************************************************************
* unistd.h
*****************************************************************/
#ifndef G__UNISTD_H
#define G__UNISTD_H

#pragma include_noerr <systypes.h>

/* NOTE: posix.dl is not generated by default. 
 * Goto $CINTSYSDIR/lib/posix directory and do 'sh setup' if you use UNIX. */
#ifndef G__POSIX_H
#pragma include_noerr "posix.dll"
#endif

#ifndef __MAKECINT__
#pragma ifndef G__POSIX_H /* G__POSIX_H is defined in posix.dl */
#pragma message Note: posix.dll is not found. Do 'sh setup' in $CINTSYSDIR/lib/posix directory if you use UNIX.
#pragma endif
#endif

#endif
