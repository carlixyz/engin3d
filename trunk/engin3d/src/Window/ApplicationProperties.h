#ifndef APROP_H
#define	APROP_H

#include <string>

struct cApplicationProperties
{
// General Application Parameters
	std::string macApplicationName;
	unsigned muiWidth;
	unsigned muiHeight;
	unsigned muiBits;
	bool mbFullscreen;
};
#endif
