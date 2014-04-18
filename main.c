#include "geohash.h"
#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
        double lat, lon;
        char geohash[64] = {0};

	if (argc == 4 && strncmp(argv[1], "-e", 2) == 0)  {
		lat = strtod(argv[2], NULL);
		lon = strtod(argv[3], NULL);
		if (geohash_encode(lat, lon, geohash, sizeof(geohash)) != GEOHASH_OK)
			printf("encode error\n");
		else
			printf("geohash: %s\n", geohash);
	} else if (argc == 3 && strncmp(argv[1], "-d", 2) == 0) {
		if (geohash_decode(argv[2], &lat, &lon) != GEOHASH_OK)
			printf("decode error\n");
		else
			printf("lat: %.14f, lon: %.14f\n", lat, lon);
	} else {
		fprintf(stderr, "usage: geohash -e 45.370000 -121.700000\n"
				"		   (latitude) (longitude)\n"
				"		-d c216ne\n");
	}

        return 0;
}
