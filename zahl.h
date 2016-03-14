/* See LICENSE file for copyright and license details. */

/* Warning: libzahl is not thread-safe. */
/* Caution: Do not use libzahl for cryptographic applications, use a specialised library. */

#ifndef ZAHL_H
#define ZAHL_H 1


#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <limits.h>



#ifndef ZAHL_INLINE
# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define ZAHL_INLINE  static inline
# else
#  define ZAHL_INLINE  static
# endif
#endif



/* You should pretend like this typedef does not exist. */
typedef uint64_t zahl_char_t;

/* This structure should be considered opaque. */
typedef struct {
        int sign;
#if INT_MAX != LONG_MAX
	int padding__;
#endif
        size_t used;
        size_t alloced;
        zahl_char_t *chars;
} z_t[1];



enum zprimality {
	NONPRIME = 0,                   /* The number is definitely composite. */
	PROBABLY_PRIME,                 /* The number is probably prime. */
	PRIME                           /* The number is definitely prime. */
};

enum zranddev {
	FAST_RANDOM = 0,                /* Random numbers are generated directly from /dev/urandom. */
	SECURE_RANDOM                   /* Random numbers are generated directly from /dev/random. */
};

enum zranddist {
	QUASIUNIFORM = 0,               /* Almost uniformly random, per the usual recommendation. */
	UNIFORM                         /* Actually uniformly random. */
};

enum zerror {
	ZERROR_ERRNO_SET = 0,           /* Please refer to errno. */
	ZERROR_0_POW_0,                 /* Indeterminate form: 0:th power of 0. (Translatable to EDOM.) */
	ZERROR_0_DIV_0,                 /* Indeterminate form: 0 divided by 0. (Translatable to EDOM.) */
	ZERROR_DIV_0,                   /* Undefined result: Division by 0. (Translatable to EDOM.) */
	ZERROR_NEGATIVE                 /* Argument must be non-negative. (Translatable to EDOM or EINVAL.) */
};



/* The parameters in the functions below are numbers a, b, c, ... */


/* Library initialisation and destruction. */

void zsetup(jmp_buf);                   /* Prepare libzahl for use. */
void zunsetup(void);                    /* Free resources used by libzahl */


/* Memory functions. */

ZAHL_INLINE void zinit(z_t);            /* Prepare a for use. */
ZAHL_INLINE void zswap(z_t, z_t);       /* (a, b) := (b, a) */
void zfree(z_t);                        /* Free resources in a. */
size_t zsave(z_t, void *);              /* Store a into b (if !!b), and return number of written bytes. */
size_t zload(z_t, const void *);        /* Restore a from b, and return number of read bytes. */


/* Assignment functions. */

void zset(z_t, z_t);                    /* a := b */
void zsetu(z_t, uint64_t);              /* a := b */
ZAHL_INLINE void zseti(z_t, int64_t);   /* a := b */

/* Comparison functions. */

int zcmp(z_t, z_t);                     /* signum (a - b) */
int zcmpu(z_t, uint64_t);               /* signum (a - b) */
int zcmpi(z_t, int64_t);                /* signum (a - b) */
int zcmpmag(z_t, z_t);                  /* signum (|a| - |b|) */


/* Arithmetic functions. */

ZAHL_INLINE void zabs(z_t, z_t);        /* a := |b| */
ZAHL_INLINE void zneg(z_t, z_t);        /* a := -b */
void zadd(z_t, z_t, z_t);               /* a := b + c */
void zsub(z_t, z_t, z_t);               /* a := b - c */
void zmul(z_t, z_t, z_t);               /* a := b * c */
void zmodmul(z_t, z_t, z_t, z_t);       /* a := (b * c) % d */
void zdiv(z_t, z_t, z_t);               /* a := b / c */
void zdivmod(z_t, z_t, z_t, z_t);       /* a := c / d, b = c % d */
void zmod(z_t, z_t, z_t);               /* a := b % c */
void zsqr(z_t, z_t);                    /* a := b² */
void zmodsqr(z_t, z_t, z_t);            /* a := b² % c */
void zpow(z_t, z_t, z_t);               /* a := b ↑ c */
void zmodpow(z_t, z_t, z_t, z_t);       /* a := (b ↑ c) % d */
void zpowu(z_t, z_t, unsigned long long int);
void zmodpowu(z_t, z_t, unsigned long long int, z_t);

/* These are used internally and may be removed in a future version. */
void zadd_unsigned(z_t, z_t, z_t);      /* a := |b| + |c| */
void zsub_unsigned(z_t, z_t, z_t);      /* a := |b| - |c| */


/* Bitwise operations. */

void zand(z_t, z_t, z_t);               /* a := b & c */
void zor(z_t, z_t, z_t);                /* a := b | c */
void zxor(z_t, z_t, z_t);               /* a := b ^ c */
void znot(z_t, z_t);                    /* a := ~b */
void zlsh(z_t, z_t, size_t);            /* a := b << c */
void zrsh(z_t, z_t, size_t);            /* a := b >> c */
void ztrunc(z_t, z_t, size_t);          /* a := b & ((1 << c) - 1) */
int zbtest(z_t, size_t);                /* (a >> b) & 1 */
void zsplit(z_t, z_t, z_t, size_t);     /* a := c >> d, b := c - (a << d) */
ZAHL_INLINE size_t zlsb(z_t);           /* Index of first set bit, SIZE_MAX if none are set. */
ZAHL_INLINE size_t zbits(z_t);          /* ⌊log₂ |a|⌋ + 1, 1 if a = 0 */

/* If d > 0: a := b | (1 << c), if d = 0: a := b & ~(1 << c), if d < 0: a := b ^ (1 << c) */
void zbset(z_t, z_t, size_t, int);


/* Number theory. */

ZAHL_INLINE int zeven(z_t);             /* Is a even? */
ZAHL_INLINE int zodd(z_t);              /* Is a odd? */
ZAHL_INLINE int zeven_nonzero(z_t);     /* Is a even? Assumes a ≠ 0. */
ZAHL_INLINE int zodd_nonzero(z_t);      /* Is a odd? Assumes a ≠ 0. */
ZAHL_INLINE int zzero(z_t);             /* Is a zero? */
ZAHL_INLINE int zsignum(z_t);           /* a/|a|, 0 if a is zero. */
void zgcd(z_t, z_t, z_t);               /* a := gcd(b, c) */

/* NONPRIME if b ∉ ℙ, PROBABLY_PRIME, if b ∈ ℙ with (1 − 4↑−c) certainty, 2 if PRIME ∈ ℙ.
 * If NONPRIME is returned the witness of b's compositeness is stored in a. */
enum zprimality zptest(z_t, z_t, int);


/* Random number generation. */

/* Pick a randomly from [0, d] ∩ ℤ. */
void zrand(z_t, enum zranddev, enum zranddist, z_t);


/* String conversion. */

char *zstr(z_t, char *);                /* Write a in decimal onto b. */
int zsets(z_t, const char *);           /* a := b */

/* Length of a in radix b. */
size_t zstr_length(z_t, unsigned long long int);


/* Error handling functions. */

enum zerror zerror(const char **);      /* Return the current error code, and unless !a, a description in *a. */
void zperror(const char *);             /* Identical to perror(3p) except it supports libzahl errors. */



/* ------------------------------- Implementations of inline functions. ------------------------------- */


#if defined(__GNUC__) || defined(__clang__)
# define ZAHL_UNLIKELY(expr)  __builtin_expect(!!(expr), 0)
# define ZAHL_LIKELY(expr)    __builtin_expect(!!(expr), 1)
#else
# define ZAHL_UNLIKELY(expr)  (expr)
# define ZAHL_LIKELY(expr)    (expr)
#endif


ZAHL_INLINE void zinit(z_t a)         { a->alloced = 0; a->chars = 0; }
ZAHL_INLINE int zeven(z_t a)          { return !a->sign || !(a->chars[0] & 1); }
ZAHL_INLINE int zodd(z_t a)           { return a->sign && (a->chars[0] & 1); }
ZAHL_INLINE int zeven_nonzero(z_t a)  { return !(a->chars[0] & 1); }
ZAHL_INLINE int zodd_nonzero(z_t a)   { return (a->chars[0] & 1); }
ZAHL_INLINE int zzero(z_t a)          { return !a->sign; }
ZAHL_INLINE int zsignum(z_t a)        { return a->sign; }
ZAHL_INLINE void zabs(z_t a, z_t b)   { if (a != b) zset(a, b); a->sign = !!a->sign; }
ZAHL_INLINE void zneg(z_t a, z_t b)   { if (a != b) zset(a, b); a->sign = -a->sign; }


ZAHL_INLINE void
zswap(z_t a, z_t b)
{
	z_t t;
	t->sign = a->sign;
	a->sign = b->sign;
	b->sign = t->sign;
	t->used = b->used;
	b->used = a->used;
	a->used = t->used;
	t->alloced = a->alloced;
	a->alloced = b->alloced;
	b->alloced = t->alloced;
	t->chars = b->chars;
	b->chars = a->chars;
	a->chars = t->chars;
}


ZAHL_INLINE void
zseti(z_t a, int64_t b)
{
	if (ZAHL_UNLIKELY(b >= 0)) {
		zsetu(a, (uint64_t)b);
	} else {
		zsetu(a, (uint64_t)-b);
		a->sign = -1;
	}
}


ZAHL_INLINE size_t
zlsb(z_t a)
{
#if defined(__GNUC__) || defined(__clang__)
	size_t i = 0;
	if (ZAHL_UNLIKELY(zzero(a)))
		return SIZE_MAX;
	for (; !a->chars[i]; i++);
	i *= 8 * sizeof(zahl_char_t);
	i += (size_t)__builtin_ctzll(a->chars[i]);
	return i;
#else
	size_t i = 0;
	zahl_char_t x;
	if (ZAHL_UNLIKELY(zzero(a)))
		return SIZE_MAX;
	for (; !a->chars[i]; i++);
	i *= 8 * sizeof(zahl_char_t);
	x = ~(a->chars[i]);
	for (; x & 1; x >>= 1, i++);
	return i;
#endif
}


ZAHL_INLINE size_t
zbits(z_t a)
{
#if defined(__GNUC__) || defined(__clang__)
	size_t rc;
	if (ZAHL_UNLIKELY(zzero(a)))
		return 1;
	while (!a->chars[a->used - 1])  a->used--; /* TODO should not be necessary */
	rc = a->used * 8 * sizeof(zahl_char_t);
	rc -= (size_t)__builtin_clzll(a->chars[a->used - 1]);
	return rc;
#else
	size_t rc;
	zahl_char_t x;
	if (ZAHL_UNLIKELY(zzero(a)))
		return 1;
	while (!a->chars[a->used - 1])  a->used--; /* TODO should not be necessary */
	x = a->chars[a->used - 1];
	rc = (a->used - 1) * 8 * sizeof(zahl_char_t);
	for (; x; x >>= 1, rc++);
	return rc;
#endif
}



#endif
