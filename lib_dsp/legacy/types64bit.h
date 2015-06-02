/*****************************************************************************\
Include File: types64bit.h


Description: Type definitions, to allow interworking between different processors (E.G. gcc running on DELL Optiplex 780)

\*****************************************************************************/

#ifndef _TYPES64BIT_H_
#define _TYPES64BIT_H_

#ifdef __XC__
// XC File

/** 8-bit types */
typedef signed char S8_T;
typedef unsigned char U8_T;

/** 16-bit types */
typedef signed short S16_T;
typedef unsigned short U16_T;

/** 32-bit types */
typedef signed int S32_T;
typedef unsigned U32_T;
typedef float R32_T;

#if (XCC_VERSION_MAJOR >= 1200)
/** 64-bit types */
typedef signed long long S64_T;
typedef unsigned long long U64_T;
#endif // (XCC_VERSION_MAJOR >= 1200)

// NB 8-Byte Integer  and  Floating-Point  types NOT supported in pre v12.6.0 of XC compiler

#else //if __XC__
// 'C' File

typedef signed char S8_T;
typedef signed short S16_T;
typedef signed int S32_T;
typedef signed long long S64_T;

typedef unsigned char U8_T;
typedef unsigned short U16_T;
typedef unsigned U32_T;
typedef unsigned long long U64_T;

typedef float R32_T;
typedef double R64_T;
typedef long double R128_T;

#endif // else __XC__

#endif /* ifndef _TYPES64BIT_H_ */

/*****************************************************************************/
#ifdef MB // Cut'n'Paste these into start of main as check.
	printf("S8_T=%d \n" ,sizeof( S8_T ) );
	printf("S16_T=%d \n" ,sizeof( S16_T ) );
	printf("S32_T=%d \n" ,sizeof( S32_T ) );
	printf("S64_T=%d \n" ,sizeof( S64_T ) );

	printf("U8_T=%d \n" ,sizeof( U8_T ) );
	printf("U16_T=%d \n" ,sizeof( U16_T ) );
	printf("U32_T=%d \n" ,sizeof( U32_T ) );
	printf("U64_T=%d \n" ,sizeof( U64_T ) );

	printf("R32_T=%d \n" ,sizeof( R32_T ) );
	printf("R64_T=%d \n" ,sizeof( R64_T ) );
	printf("R128_T=%d \n" ,sizeof( R128_T ) );
#endif //MB
/*****************************************************************************/
// types64bit.h
