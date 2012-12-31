/* This is a public domain geohash implement write by WEI Zhicheng */
#ifndef __GEOHASH_H__
#define __GEOHASH_H__

#include <stdlib.h>

void
geohash_encode(double latitude, double longitude, char *hash, size_t len);

void
geohash_decode(char *hash, double *latitude, double *longitude);

#endif /* __GEOHASH_H__ */

