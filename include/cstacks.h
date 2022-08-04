#ifndef CSTACKS_H_INCLUDED
#define CSTACKS_H_INCLUDED
#include "cstructurescommon.h"

//queues manipulations
int pushStack(meta_t*,  const void*);
void* popStack(meta_t* meta);

//queues functors
int foreachStack(meta_t* meta, void* parameter, int (*doAction)(void*, void*));

#endif // CSTACKS_H_INCLUDED
