#include "geohash.h"
#include <stdio.h>

int
main(void)
{
        double lat, lon;
        char geohash[64] = {0};

        geohash_decode("c216ne", &lat, &lon);
        printf("lat: %f, lon: %f\n", lat, lon);

        geohash_encode(lat, lon, geohash, sizeof(geohash));
        printf("hash: %s\n", geohash);

        return 0;
}
