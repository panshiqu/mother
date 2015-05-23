#pragma once
#include <string>

#define LISTVIEW_HEIGHT 60.0f
#define TEXT_FONT_SIZE 32

struct CONTACT
{
	std::string name;
	std::string number;
	std::string image;

	CONTACT()
	{
		name = "";
		number = "";
		image = "";
	}
};

// µ¥ÀýÄ£Ê½ºê
#define SINGLETON(classname) \
	static classname &getInstance(void) \
{ \
	static classname instance; \
	return instance; \
}
