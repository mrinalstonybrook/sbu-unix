#ifndef _SYSCALL_DICTIONARY_H
#define _SYSCALL_DICTIONARY_H
#define EXIT 0
#define SLEEP 1
#define PRINTF 10
#define MALLOC 12
#define SCANF 13
#define CLRSCR 11
#define SHELL 14

/***Keeping [20-50] for TARFS,FILE_SYSTEM,NETWORKS *******/
#define TARFS_OPEN_DIR 20
#define TARFS_READ_DIR 21
#define TARFS_CLOSE_DIR 22
#define TARFS_READ_DIR_LS 23
#define TARFS_OPEN_FILE 24 
#define TARFS_READ_FILE 25
#define TARFS_CLOSE_FILE 26

#define FS_OPEN_DIR 27
#define FS_READ_DIR 28
#define FS_CLOSE_DIR 29
#define FS_READ_DIR_LS 30
#define FS_OPEN_FILE  31
#define FS_READ_FILE  32
#define FS_CLOSE_FILE 33
#define FS_WRITE_FILE 33
#define MAKE_DIR 34
#define PS 65

#define GETPID 60
/****************************************************/

////////////    NOTE :: USE AFTER 50 i.e. starting from 51 /////////////////////
#endif
