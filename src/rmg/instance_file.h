#pragma once

#include "../GenericParser2.h"

class CRMInstance;

class CRMInstanceFile
{
public:

	CRMInstanceFile ( );
	~CRMInstanceFile ( );

	bool			Open			( const char* instance );
	void			Close			( void );
	CRMInstance*	CreateInstance	( const char* name );

protected:

	CGenericParser2		mParser;
	CGPGroup*			mInstances;
};
