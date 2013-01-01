#include "geohash.h"
#include <stdio.h>

int
main(void)
{
        double lat, lon;
        char geohash[64] = {0};

        if (geohash_decode("c216ne", &lat, &lon) != GEOHASH_OK)
		printf("decode error\n");
	else
		printf("lat: %f, lon: %f\n", lat, lon);

        if (geohash_encode(lat, lon, geohash, sizeof(geohash)) != GEOHASH_OK)
		printf("encode error\n");
	else
		printf("hash: %s\n", geohash);

        return 0;
}
