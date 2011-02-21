#ifndef MY_DEBUG_H
#define MY_DEBUG_H
// "arranged" By CarliXYZ graving lots of bits from here and there, too mixed up to say nothing
// tip: the next lines are equivalent
// Debug.ToOuput()  << something;
// Debug::ToOuput() << something;  
// Debug()			<< something;

#include <sstream>
#include <windows.h>

class Debug	
{
public:

   static Debug &ToOutput(void);					// Use Debug::ToOuput() << something << etc << etc ;
   static void ToOutput(const char *format, ...);	// Use Debug::ToOuput( "something %i <%f> ", etc, etc );
   static void ToMsgBox(const char *format, ...);	// same as the last one but sending it to a Windows MessageBox
   
   template <class T>
   inline Debug &operator << (const T & t) // templates needs definitions in headers
   {

		static std::ostringstream oss;  // create a stream
		oss << t;						// insert value to stream
		OutputDebugString( oss.str().c_str() ); // Target Output stream
		return *this;
   }

   inline Debug &operator << (Debug &(*function)(Debug &dbgInstance)) { return function (*this); }// Modifiers implementions
   static Debug &Endl (Debug &dbgInstance);	// Use Debug() << Debug::Endl << something << Debug::Endl ;
 
   // and so on..
};

#endif // MY_DEBUG_H

// TO DO:  Add some ToFile( name.xml, string, ...) Foo 