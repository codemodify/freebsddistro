#ifndef __Helper_H__
#define __Helper_H__


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// used to detect the defined controls in the XMl file
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
enum ControlType
{
    eBeginType = -1,
	
    eFontChooser,
    eColorChooser,
    ePathChooser,
    eRadioBoxes,
    eCheckBoxes,
    eTextViewer,
    eLineInput,
    ePlugins,

    eEndType,
    eUnsupportedType = eEndType
};

static const char* ControlTypeName[ eEndType ] = 
{
    "fontChooser", 
    "colorChooser",
    "pathChooser",
    "radioBoxes",
    "checkBoxes",
    "textViewer",
    "lineInput",
    "plugins"
};

ControlType getControlTypeByName( const QString typeName )
{
	int control = static_cast<int>( eBeginType + 1 );

	for( ; control != eEndType; ++control )
	{
		if( typeName == ControlTypeName[control] )
			return static_cast<ControlType>( control );
	}

	return eUnsupportedType;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// used internally to recognize the speciffic key-words in the XML file
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
static const char* c_configuratorNodeName		= "configurator";
static const char* c_sectionNodeName 		= "section";
static const char* c_sectionItemNodeName		= "sectionItem";

static const char* c_attributeTitleText		= "titleText";
static const char* c_attributeName 			= "name";
static const char* c_attributeText 			= "text";
static const char* c_attributeType 			= "type";
static const char* c_attributeValue 			= "value";
static const char* c_attributeUrl 			= "url";

#endif
