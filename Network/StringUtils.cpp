#include "StringUtils.h"

#include <stdarg.h>

void OutputDebugString( const char* inString )
{
	printf( "%s", inString );
}

std::string StringUtils::GetCommandLineArg( int inIndex )
{
	if( inIndex < __argc )
	{
		return std::string( __argv[ inIndex ] );
	}
	
	return std::string();
}


std::string StringUtils::Sprintf( const char* inFormat, ... )
{
	//not thread safe...
	static char temp[ 4096 ];
	
	va_list args;
	va_start (args, inFormat );
	
#if _WIN32
	_vsnprintf_s( temp, 4096, 4096, inFormat, args );
#else
	vsnprintf(temp, 4096, inFormat, args);
#endif
	return std::string( temp );
}

// void StringUtils::Log( const char* inFormat )
// {
// 	OutputDebugString( inFormat );
// 	OutputDebugString( "\n" );
// }

void StringUtils::Log( const char* inFormat, ... )
{
	//not thread safe...
	static char temp[ 4096 ];
	
	va_list args;
	va_start (args, inFormat );
	
#if _WIN32
	_vsnprintf_s( temp, 4096, 4096, inFormat, args );
#else
	vsnprintf(temp, 4096, inFormat, args);
#endif
	OutputDebugString( temp );
	OutputDebugString( "\n" );
}

