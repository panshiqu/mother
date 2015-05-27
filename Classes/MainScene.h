#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Mother.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"
#endif

class MainScene : public cocos2d::Scene
{
public:
	MainScene();
	virtual ~MainScene();
	CREATE_FUNC(MainScene)

	virtual bool init();

	void onSelectButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void onAddSaveButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void onEditButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type, CONTACT *contact);
	void onDeleteButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type, size_t index);
	void onContactButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type, std::string number);
	void onExitButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

public:
	void setImagePath(const char *imagePath)	{ _imagePath = imagePath; }

private:
	void addListView(size_t index, CONTACT *contact);
	void addPageView(CONTACT *contact);

private:
	std::string _imagePath;
	cocos2d::ui::EditBox *_editName;
	cocos2d::ui::EditBox *_editNumber;
	cocos2d::ui::ListView *_listView;
	cocos2d::ui::PageView *_pageView;
};

