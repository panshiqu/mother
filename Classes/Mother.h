#ifndef  _MOTHER_H_
#define  _MOTHER_H_
#include <string>

#define LISTVIEW_HEIGHT 60.0f

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

#endif
