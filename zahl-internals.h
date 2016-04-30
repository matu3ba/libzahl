/* See LICENSE file for copyright and license details. */

#ifndef ZAHL_INLINE
# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define ZAHL_INLINE  static inline
# else
#  define ZAHL_INLINE  static
# endif
#endif


#if defined(__GNUC__) || defined(__clang__)
# define ZAHL_LIKELY(expr)                __builtin_expect(!!(expr), 1)
# define ZAHL_UNLIKELY(expr)              __builtin_expect(!!(expr), 0)
# define ZAHL_CONST_P(value)              __builtin_constant_p(value)
#else
# define ZAHL_LIKELY(expr)                (expr)
# define ZAHL_UNLIKELY(expr)              (expr)
#endif


#if defined(__GNUC__) && !defined(__clang__)
# define ZAHL_O0     __attribute__((optimize("O0")))
# define ZAHL_O1     __attribute__((optimize("O1")))
# define ZAHL_O2     __attribute__((optimize("O2")))
# define ZAHL_O3     __attribute__((optimize("O3")))
# define ZAHL_Ofast  __attribute__((optimize("Ofast")))
# define ZAHL_Os     __attribute__((optimize("Os")))
# define ZAHL_Oz     __attribute__((optimize("Os")))
#elif defined(__clang__)
# define ZAHL_O0     __attribute__((optnone))
# define ZAHL_O1     /* Don't know how. */
# define ZAHL_O2     /* Don't know how. */
# define ZAHL_O3     /* Don't know how. */
# define ZAHL_Ofast  /* Don't know how. */
# define ZAHL_Os     /* Don't know how. */
# define ZAHL_Oz     /* Don't know how. */
#else
# define ZAHL_O0     /* Don't know how. */
# define ZAHL_O1     /* Don't know how. */
# define ZAHL_O2     /* Don't know how. */
# define ZAHL_O3     /* Don't know how. */
# define ZAHL_Ofast  /* Don't know how. */
# define ZAHL_Os     /* Don't know how. */
# define ZAHL_Oz     /* Don't know how. */
#endif


#define ZAHL_BITS_PER_CHAR                64
#define ZAHL_LB_BITS_PER_CHAR             6
#define ZAHL_CHAR_MAX                     UINT64_MAX


#define ZAHL_FLOOR_BITS_TO_CHARS(bits)    ((bits) >> ZAHL_LB_BITS_PER_CHAR)
#define ZAHL_CEILING_BITS_TO_CHARS(bits)  (((bits) + (ZAHL_BITS_PER_CHAR - 1)) >> ZAHL_LB_BITS_PER_CHAR)
#define ZAHL_BITS_IN_LAST_CHAR(bits)      ((bits) & (ZAHL_BITS_PER_CHAR - 1))
#define ZAHL_TRUNCATE_TO_CHAR(bits)       ((bits) & ~(size_t)(ZAHL_BITS_PER_CHAR - 1))


#define ZAHL_SET_SIGNUM(a, signum)        ((a)->sign = (signum))
#define ZAHL_SET(a, b)                    do { if ((a) != (b)) zset(a, b); } while (0)
#define ZAHL_ENSURE_SIZE(a, n)            do { if ((a)->alloced < (n)) libzahl_realloc(a, (n)); } while (0)
#define ZAHL_TRIM(a)                      for (; (a)->used && !(a)->chars[(a)->used - 1]; (a)->used--)
#define ZAHL_TRIM_NONZERO(a)              for (; !(a)->chars[(a)->used - 1]; (a)->used--)
#define ZAHL_TRIM_AND_ZERO(a)             do { ZAHL_TRIM(a); if (!(a)->used) ZAHL_SET_SIGNUM(a, 0); } while (0)
#define ZAHL_TRIM_AND_SIGN(a, s)          do { ZAHL_TRIM(a); ZAHL_SET_SIGNUM(a, (a)->used ? (s) : 0); } while (0)
#define ZAHL_SWAP(a, b, t, m)             ((t)->m = (a)->m, (a)->m = (b)->m, (b)->m = (t)->m)


#if defined(__GNUC__) || defined(__clang__)
# if ZAHL_CHAR_MAX == LONG_MAX
#  define ZAHL_ADD_CTZ(r, x)  ((r) += (size_t)__builtin_ctzl(x))
#  define ZAHL_SUB_CLZ(r, x)  ((r) -= (size_t)__builtin_clzl(x))
# else
#  define ZAHL_ADD_CTZ(r, x)  ((r) += (size_t)__builtin_ctzll(x))
#  define ZAHL_SUB_CLZ(r, x)  ((r) -= (size_t)__builtin_clzll(x))
# endif
#else
# define ZAHL_ADD_CTZ(r, x)                                   \
	do {                                                  \
		zahl_char_t zahl_x__ = (x);                   \
		for (; zahl_x__ & 1; zahl_x__ >>= 1, (r)++);  \
	} while (0)
# define ZAHL_SUB_CLZ(r, x)                               \
	do {                                              \
		zahl_char_t zahl_x__ = (x);               \
		(r) -= 8 * sizeof(zahl_char_t);           \
		for (; zahl_x__; zahl_x__ >>= 1, (r)++);  \
	} while (0)
#endif


typedef uint64_t zahl_char_t;

struct zahl {
        int sign;
#if INT_MAX != LONG_MAX
	int padding__;
#endif
        size_t used;
        size_t alloced;
        zahl_char_t *chars;
};


void libzahl_realloc(struct zahl *, size_t);

ZAHL_O2 static inline void
libzahl_memcpy(register zahl_char_t *restrict d, register const zahl_char_t *restrict s, size_t n)
{
	size_t i;
	for (i = 0; i < n; i++)
		d[i] = s[i];
}

ZAHL_O2 static inline void
libzahl_memset(register zahl_char_t *a, register zahl_char_t v, size_t n)
{
	size_t i;
	for (i = 0; i < n; i++)
		a[i] = v;
}
