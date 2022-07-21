#ifndef CQUEUES_H_INCLUDED
#define CQUEUES_H_INCLUDED
#include "cstructurescommon.h"

//queues manipulations
int pushQueue(meta_t*,  const void*);
void* popQueue(meta_t* meta);

//queues functors
int foreachQueue(meta_t* meta, void* parameter, int (*doAction)(void*, void*));

#endif // CQUEUES_H_INCLUDED
