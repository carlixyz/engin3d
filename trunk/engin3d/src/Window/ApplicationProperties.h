#ifndef APROP_H
#define	APROP_H

#include <string>
#include <tinystr.h>
#include <tinyxml.h>

struct cApplicationProperties
{
// General Application Parameters
	std::string macApplicationName;
	unsigned muiWidth;
	unsigned muiHeight;
	unsigned muiBits;
	bool mbFullscreen;

// inline File properties Loader 
	inline bool LoadXML(const std::string &File){ // inline foo: We call this just only one time so why not..
	  TiXmlDocument doc( File );
	  
  if ( doc.LoadFile() )
  {
	  TiXmlHandle docHandle( &doc ); //  document´s handle
	  TiXmlElement *pProperty; 
	  TiXmlAttribute* pAttrib; 
	
	  pProperty = docHandle.FirstChildElement("Application").FirstChildElement("Window").FirstChildElement("Property").ToElement();
	 
	   // seeking in: <DOC> / <Application> / <Window> / <property (Attrib)> 

	  for( assert(pProperty) ; pProperty ; pProperty = pProperty->NextSiblingElement() )
	  {
		pAttrib = pProperty->FirstAttribute();

		if(  pAttrib->NameTStr() == "Name" ){
			 this->macApplicationName = pAttrib->ValueStr(); // Properties.macApplicationName
			  continue;}

		else if( pAttrib->NameTStr() == "Width"  ){
			 this->muiWidth = atoi( pAttrib->Value() ); // Properties.muiWidth
			 continue;}		

		else if( pAttrib->NameTStr() == "Height" ){
			this->muiHeight = atoi( pAttrib->Value()); // Properties.muiHeight
			 continue;}
       
		else if( pAttrib->NameTStr() ==  "Bits"  ) {
			 this->muiBits = atoi( pAttrib->Value() ) ; // Properties.muiBits
			 continue;}

   		else if( pAttrib->NameTStr() ==  "Fullscreen"   )
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