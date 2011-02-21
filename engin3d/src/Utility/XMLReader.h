#ifndef C_XML_READER_H
#define C_XML_READER_H

#include <tinystr.h>
#include <tinyxml.h>

class cXMLReader	// Abstract Interface 
{
public:
// General Application Parameters

// inline File properties Loader 
	virtual	bool LoadXML(const char *File) = 0;
};
#endif