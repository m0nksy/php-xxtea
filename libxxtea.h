/***********************************************************************
    Copyright 2016-2020 Xu Xianhua
    These sources is free software. Redistributions of source code must
    retain the above copyright notice. Redistributions in binary form
    must reproduce the above copyright notice. You can redistribute it
    freely. You can use it with any free or commercial software.
    These sources is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY. Without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*************************************************************************/

#ifndef XXTEA_H

#define XXTEA_H
#define XXTEA_MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z)))
#define XXTEA_DELTA 0x9e3779b9
#define XXTEA_ROUNDS(n) (6 + 52/(n))
#define XXTEA_E(sum) (((sum) >> 2) & 3)
#define KEY_SIZE 16

#include <stddef.h>

#ifdef _MSC_VER
typedef unsigned __int32 xxtea_long;
#else

#if defined(__FreeBSD__) && __FreeBSD__ < 5
#include <inttypes.h>
#else
#include <stdint.h>
#endif /* End of __FreeBSD__ */

typedef uint32_t xxtea_long;

#endif /* End of  _MSC_VER */

extern int xxtea_bytes_encode(const char *, int, const char *, int, char **);
extern int xxtea_bytes_decode(const char *, int, const char *, int, char **);

#endif /* End of XXXTEA_H */