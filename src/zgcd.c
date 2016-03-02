/* See LICENSE file for copyright and license details. */
#include "internals"

#define u  libzahl_tmp_gcd_u
#define v  libzahl_tmp_gcd_v


void
zgcd(z_t a, z_t b, z_t c)
{
	/*
	 * Binary GCD algorithm.
	 */

	size_t shifts = 0, i = 0;
	zahl_char_t uv, bit;
	int neg;

	if (!zcmp(b, c)) {
		if (a != b)
			zset(a, b);
		return;
	}
	if (zzero(b)) {
		if (a != c)
			zset(a, c);
		return;
	}
	if (zzero(c)) {
		if (a != b)
			zset(a, b);
		return;
	}

	zabs(u, b);
	zabs(v, c);
	neg = zsignum(b) < 0 && zsignum(c) < 0;

	for (;; i++) {
		uv = (i < u->used ? u->chars[i] : 0)
		   | (i < v->used ? v->chars[i] : 0);
		for (bit = 1; bit; bit <<= 1, shifts++)
			if (uv & bit)
				goto loop_done;
	}
loop_done:
	zrsh(u, u, shifts);
	zrsh(v, v, shifts);

	zrsh(u, u, zlsb(u));
	do {
		zrsh(v, v, zlsb(v));
		if (zcmpmag(u, v) > 0) /* Both are non-negative. */
			zswap(u, v);
		zsub_unsigned(v, v, u);
	} while (!zzero(v));

	zlsh(a, u, shifts);
	SET_SIGNUM(a, neg ? -1 : 1);
}
