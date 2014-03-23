#ifndef __Helper_H__
#define __Helper_H__


enum ActionType
{
	eBeginType = -1,
	
	eCopyContent,

	eEndType,
	eUnsupportedType = eEndType
};

static const char* ActionTypeName[ eEndType ] = 
{
	"copyContent"
};

ActionType getActionTypeByName( const QString actionName )
{
	int control = static_cast<int>( eBeginType + 1 );

	for( ; control != eEndType; ++control )
	{
		if( actionName == ActionTypeName[control] )
			return static_cast<ActionType>( control );
	}

	return eUnsupportedType;
}


#endif
