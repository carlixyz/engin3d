#ifndef INTERNALRESOURCE_H
#define INTERNALRESOURCE_H

/*
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
*/

#include "Resource.h"

static const unsigned kuiInvalidKey = 0;

struct cInternalResource
{
	unsigned int muiKey;
	cResource * mpResource;
};

#endif