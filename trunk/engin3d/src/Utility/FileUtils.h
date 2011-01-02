#ifndef FILE_UTILS_H
#define	FILE_UTILS_H

#include <string>

class cFileUtils
{
	public:
		static std::string GetDirectory(const std::string lacFile);
		static std::string GetFileName(const std::string lacFile);
};


#endif // FILE_UTILS_H