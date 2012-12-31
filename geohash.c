/* This is a public domain geohash implement write by WEI Zhicheng */
#include <math.h>
#include <float.h>
#include <string.h>

#include "geohash.h"

static char base32en[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'j', 'k', 'm', 'n', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

static char base32de[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	10, 11, 12, 13, 14, 15, 16, -1, 17, 18,
	-1, 19, 20, -1, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31
};

#define PRECISION	.00000000001	/* XXX not validated */

static int
max(int x, int y)
{
	if (x < y)
		return y;
	return x;
}

static double
pround(double x, int precision)
{
	double div = pow(10, precision);
	return round(x * div) / div;
}

/* Quick and Dirty Floating-Point to Fractional Precision */
static double
fprec(double x)
{
        int i;
        double int_part, frac_part;

        frac_part = modf(x, &int_part);

	/* check fractional is really close 0.0 or 1.0 */
	for (i = 0; fabs(frac_part - 0.0) > FLT_EPSILON && 
		    fabs(frac_part - 1.0) > FLT_EPSILON; i++) {
                frac_part *= 10;
                frac_part = modf(frac_part, &int_part);
        }
	return pow(10, -i);
}

void 
geohash_encode(double latitude, double longitude, char *hash, size_t len)
{
	double lat[] = {-90.0, 90.0};
	double lon[] = {-180.0, 180.0};
	double mid;

	double precision;

	char mask[] = {16, 8, 4, 2, 1};

	int i = 0;
	int n = 0;
	int idx = 0;
	int even = 0;

	int right;

	precision = fmin(fprec(latitude), fprec(longitude));
	precision = fmax(PRECISION, precision);

	len -= 1;
	while (i < len) {
		if ((lat[1] - lat[0]) < precision && (lon[1] - lon[0]) < precision)
			break;
			
		if ((even = !even)) {
			mid = (lon[0] + lon[1]) / 2.0;
			right = (longitude > mid);
			if (right)
				idx |= mask[n];
			lon[!right] = mid;
		} else {
			mid = (lat[0] + lat[1]) / 2.0;
			right = (latitude > mid);
			if (right)
				idx |= mask[n];
			lat[!right] = mid;
		}

		if (n++ >= 4) {
			hash[i++] = base32en[idx];
			idx = n = 0;
		}
	}
	hash[i] = 0;
}

void
geohash_decode(char *hash, double *latitude, double *longitude)
{
	size_t len = strlen(hash);

	double lat[] = {-90.0, 90.0};
	double lon[] = {-180.0, 180.0};
	double mid;

	double lat_err, lon_err;

	char mask[] = {16, 8, 4, 2, 1};

	int i = 0;
	int n = 0;
	int idx = 0;
	int even = 0;

	int right;

	while (i < len) {
		idx = base32de[hash[i] - '0'];
		if ((even = !even)) {
			mid = (lon[0] + lon[1]) / 2.0;
			right = (idx & mask[n]);
			lon[!right] = mid;
		} else {
			mid = (lat[0] + lat[1]) / 2.0;
			right = (idx & mask[n]);
			lat[!right] = mid;
		}
		if (n++ >= 4) {
			i++;
			idx = n = 0;
		}
	}
	lat_err = (lat[1] - lat[0]) / 2.0;
	*latitude = pround((lat[0] + lat[1]) / 2.0,
			max(1, (int)round(-log10(lat_err))) - 1);
	
	lon_err = (lon[1] - lon[0]) / 2.0;
	*longitude = pround((lon[0] + lon[1]) / 2.0, 
			max(1, (int)round(-log10(lon_err))) - 1);
}

