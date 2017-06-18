#ifndef TYPE_DEF_H
#define TYPE_DEF_H

#include <stdlib.h>
#include <stdio.h>

typedef signed char			int8;
typedef unsigned char		uint8;
typedef signed short		int16;
typedef unsigned short		uint16;
typedef signed int			int32;
typedef unsigned int		uint32;
typedef unsigned long		ulong;

#ifdef WIN32
typedef __int64				int64;
typedef unsigned __int64	uint64;
#else
typedef long long			int64;
typedef unsigned long long	uint64;
#endif

typedef float				float32;
typedef double				float64;

#ifndef NULL
#define NULL	0
#endif

#endif



