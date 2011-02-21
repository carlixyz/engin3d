#ifndef APROP_H
#define	APROP_H

#include <string>
#include "..\Utility\XMLReader.h"

struct cApplicationProperties : public cXMLReader
{
// General Application Parameters
	std::string macApplicationName;
	unsigned muiWidth;
	unsigned muiHeight;
	unsigned muiBits;
	bool mbFullscreen;

// inline File properties Loader 
	inline bool LoadXML(const char *File){ // inline foo: We call this just only one time so why not..
	  TiXmlDocument doc( File );
	  
  if ( doc.LoadFile() )
  {
	  TiXmlHandle docHandle( &doc ); //  document´s handle
	  TiXmlElement *pElem; 
	  TiXmlAttribute* pAttrib; 
	
	  pElem = docHandle.FirstChildElement("Application").FirstChildElement("Window").FirstChildElement("Property").ToElement();
	 
	  assert(pElem); // seeking in: <DOC> / <Application> / <Window> / <property (Attrib)> 

	  for(TiXmlElement *pProperty = pElem ; pProperty ; pProperty = pProperty->NextSiblingElement() )
	  {
	  pAttrib = pProperty->FirstAttribute();

  		if( !strcmp( pAttrib->Name(), "Name") ){
			 this->macApplicationName = pAttrib->ValueStr(); // Properties.macApplicationName
			  continue;}

		else if( !strcmp( pAttrib->Name(), "Width" ) ){
			 this->muiWidth = atoi( pAttrib->Value() ); // Properties.muiWidth
			 continue;}		

		else if( !strcmp( pAttrib->Name(), "Height" ) ){
			  this->muiHeight = atoi( pAttrib->Value()); // Properties.muiHeight
			 continue;}
       
		else if( !strcmp( pAttrib->Name(), "Bits" ) ) {
			 this->muiBits = atoi( pAttrib->Value() ) ; // Properties.muiBits
			 continue;}

   		  else if( !strcmp( pAttrib->Name(), "Fullscreen")   )
			 this->mbFullscreen = ( pAttrib->Value() == "true" );// Properties.mbFullscreen
	  
	  }	return true;

  }else return false;
	}
};
#endif
/*	Example:
	cApplicationProperties mProperties;
	mProperties.macApplicationName = "Test 1";
	mProperties.mbFullscreen = false;
	mProperties.muiBits = 16;
	mProperties.muiWidth = 640 ;
	mProperties.muiHeight = 480;
*/