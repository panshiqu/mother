#pragma once
#include "cocos2d.h"
#include "Mother.h"

#include<iomanip>

class ContactManager
{
public:
	ContactManager(void);
	virtual ~ContactManager(void);
	SINGLETON(ContactManager)

	void init(void);

	CONTACT *addContact(std::string name, std::string number, std::string image);
	void delContact(size_t index);

	std::map<size_t, CONTACT *> &getContacts(void)	{ return _contacts; }
	size_t getIndex(void)	{ return _index; }

private:
	std::string generateKey(std::string key, size_t index);

private:
	std::map<size_t, CONTACT *> _contacts;
	size_t _index;
};

