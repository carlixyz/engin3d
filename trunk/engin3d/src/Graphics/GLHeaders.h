#ifndef GLHEADERS_H
#define GLHEADERS_H

#include <windows.h>
#include "GLee/glee.h"
#include <Cg/cg.h>		// Header Files For The Com Graphics Library (Shaders)
#include <Cg/cgGL.h>
#include <gl/gl.h>		// Header File For The OpenGL32 Library
#include <gl/glu.h>		// Header File For The GLu32 Library

#endif

/* In case of issues with the Manifest creation (RELEASE MODE) use this..
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
/*
// Another option is : go Project Properties > Herramienta Manifiesto > Utilizar archivos de respuesta UNICODE > NO !