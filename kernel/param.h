// param.h

#ifndef PARAM_H_
#define PARAM_H_

#define N_PROCESS          16 // maximum number of processes
#define N_PER_PROC_FILES    8 // maximum number of opened files per process
#define N_FILES            64 // maximum opened files on the system
#define N_INODES           24 // max number of active i-nodes
#define N_DEV              10 // max major device number
#define ROOTDEV             1 // device number of the system root disk
#define MAX_ARGS            8 // max execution arguments
#define FS_SIZE           128 // size of the file system in blocks
#define MAX_OPENED_BLOCKS   5 // maximum number of blocks any operation writes
#define LOG_SIZE           16 // maximum data blocks for logs
#define N_BUFFERS          16 // size of the disk block cache
#define MAX_PATH_NAME      64 // maximum file path name
#define USER_STACK_SIZE     1 // user stack pages

#endif // PARAM_H_
