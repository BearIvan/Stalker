#ifndef ODE_REDEFINE
#define ODE_REDEFINE

#ifdef  XRGAME_EXPORTS

#ifdef	dSqrt
#undef	dSqrt
#define dSqrt(x) ((float)XrMath::sqrt(x))		/* square root */
#endif

#ifdef  dRecipSqrt
#undef  dRecipSqrt
#define dRecipSqrt(x) ((float)(1.0f/XrMath::sqrt(x)))	/* reciprocal square root */
#endif

#ifdef	dSin
#undef  dSin
#define dSin(x) ((float)XrMath::sin(x))			/* sine */
#endif

#ifdef  dCos
#undef  dCos
#define dCos(x) ((float)XrMath::cos(x))			/* cosine */
#endif

#ifdef  dFabs
#undef  dFabs
#define dFabs(x) ((float)XrMath::abs(x))		/* absolute value */
#endif

#endif//XRGAME_EXPORTS

#endif//ODE_REDEFINE