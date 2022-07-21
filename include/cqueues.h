#ifndef CQUEUES_H_INCLUDED
#define CQUEUES_H_INCLUDED
#include "cstructurescommon.h"

int pushQueue(meta_t*,  const void*);
void* popQueue(meta_t* meta);

#endif // CQUEUES_H_INCLUDED
