#include "ContactManager.h"

using namespace std;

ContactManager::ContactManager(void)
{
}


ContactManager::~ContactManager(void)
{
}


void ContactManager::init(void)
{
	for (_index = 0; ; _index++)
	{
		string name = cocos2d::UserDefault::getInstance()->getStringForKey(generateKey("name", _index).c_str());
		string number = cocos2d::UserDefault::getInstance()->getStringForKey(generateKey("number", _index).c_str());
		string image = cocos2d::UserDefault::getInstance()->getStringForKey(generateKey("image", _index).c_str());
		if (name.empty() && number.empty() && image.empty()) break;
		if (name.empty()) continue;

		CONTACT *contact = new CONTACT;
		contact->name = name;
		contact->number = number;
		contact->image = image;

		_contacts.insert(make_pair(_index, contact));
	}
}


CONTACT *ContactManager::addContact(std::string name, std::string number, std::string image)
{
	for (auto con : _contacts)
	{
		if (con.second->number == number)
		{
			con.second->name = name;
			con.second->image = image;
			cocos2d::UserDefault::getInstance()->setStringForKey(generateKey("name", con.first).c_str(), name);
			cocos2d::UserDefault::getInstance()->setStringForKey(generateKey("image", con.first).c_str(), image);
			return NULL;
		}
	}

	CONTACT *contact = new CONTACT;
	contact->name = name;
	contact->number = number;
	contact->image = image;

	_contacts.insert(make_pair(_index, contact));

	cocos2d::UserDefault::getInstance()->setStringForKey(generateKey("name", _index).c_str(), name);
	cocos2d::UserDefault::getInstance()->setStringForKey(generateKey("number", _index).c_str(), number);
	cocos2d::UserDefault::getInstance()->setStringForKey(generateKey("image", _index).c_str(), image);

	_index++;
	return contact;
}


void ContactManager::delContact(size_t index)
{
	std::map<size_t, CONTACT *>::iterator itr = _contacts.find(index);
	if (itr != _contacts.end())
	{
		cocos2d::UserDefault::getInstance()->setStringForKey(generateKey("name", itr->first).c_str(), "");
		CONTACT *contact = itr->second;
		_contacts.erase(itr);
		delete contact;
	}
}


std::string ContactManager::generateKey(std::string key, size_t index)
{
	ostringstream oss;
	oss << "mother_" << key << "_" << std::setw(3) << std::setfill('0') << index;
	return oss.str();
}
