///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains preprocessor stuff. This should be the first included header.
 *	\file		IcePreprocessor.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEPREPROCESSOR_H__
#define __ICEPREPROCESSOR_H__

	// Check platform
	#if defined( _WIN32 ) || defined( WIN32 ) || defined( WINDOWS ) 
		#pragma message("Compiling on Windows...")
		#define PLATFORM_WINDOWS
	#else
		#pragma message("Compiling on unknown platform...")
	#endif

	// Check compiler
	#if defined(_MSC_VER)
		#pragma message("Compiling with VC++...")
		#define COMPILER_VISUAL_CPP
	#else
		#pragma message("Compiling with unknown compiler...")
	#endif

	// Check compiler options
	
	#ifndef THIS_FILE
		#define THIS_FILE			__FILE__
	#endif
	#define FUNCTION				extern "C"

	// Cosmetic stuff [mainly useful with multiple inheritance]
	#define	override(baseclass)	virtual


	#define inline_				inline

	// Down the hatch
	#pragma inline_depth( 255 )

#endif // __ICEPREPROCESSOR_H__
