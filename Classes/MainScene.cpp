#include "MainScene.h"
#include "ContactManager.h"

USING_NS_CC;

using namespace cocos2d::ui;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	extern "C"
	{
		void Java_org_cocos2dx_cpp_AppActivity_setImagePath(JNIEnv *env, jobject thiz, jstring str)
		{
			// 网上搜集
			char *rtn = NULL;
			jclass clsstring = (*env).FindClass("java/lang/String");
			jstring strencode = (*env).NewStringUTF("utf-8");
			jmethodID mid = (*env).GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
			jbyteArray barr= (jbyteArray)(*env).CallObjectMethod(str, mid, strencode);
			jsize alen = (*env).GetArrayLength(barr);
			jbyte* ba = (*env).GetByteArrayElements(barr, JNI_FALSE);
			if (alen > 0)
			{
				rtn = (char*)malloc(alen + 1);
				memcpy(rtn, ba, alen);
				rtn[alen] = 0;
			}

			(*env).ReleaseByteArrayElements(barr, ba, 0);
			MainScene *runningScene = (MainScene *)Director::getInstance()->getRunningScene();
			runningScene->setImagePath(rtn);
		}
	}
#endif

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}


bool MainScene::init()
{
	if (!Scene::init()) return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	std::map<size_t, CONTACT *> &contacts = ContactManager::getInstance().getContacts();

	// 创建翻页容器
	_pageView = PageView::create();
	_pageView->setContentSize(visibleSize);
	_pageView->setPosition(Vec2(0, 0));
	this->addChild(_pageView);

	auto *layout = Layout::create();
	layout->setPosition(Vec2(0, 0));
	layout->setContentSize(visibleSize);
	layout->setBackGroundImage("background.png");
	layout->setBackGroundImageScale9Enabled(true);

	// 创建首页列表容器
	_listView = ListView::create();
	_listView->setBounceEnabled(true);
	_listView->setContentSize(visibleSize);
	layout->addChild(_listView);

	// 添加退出按钮
	auto *exitButton = Button::create("button.png");
	exitButton->setPosition(Vec2(visibleSize.width / 2.0f, 100.0f / 2.0f));
	exitButton->addTouchEventListener(CC_CALLBACK_2(MainScene::onExitButton, this));
	exitButton->setTitleColor(Color3B::RED);
	exitButton->setTitleText("Exit");
	layout->addChild(exitButton);

	auto *layoutList = Layout::create();
	layoutList->setLayoutType(Layout::Type::ABSOLUTE);
	layoutList->setContentSize(Size(visibleSize.width, LISTVIEW_HEIGHT));

	// 名称编辑框
	_editName = EditBox::create(Size(visibleSize.width / 5.0f, LISTVIEW_HEIGHT), "background_green.png");
	_editName->setAnchorPoint(Vec2(0.0f, 0.0f));
	_editName->setFont("Arial", 24);
	_editName->setPlaceHolder("Name:");
	_editName->setPlaceholderFont("Arial", 24);
	_editName->setPlaceholderFontColor(Color4B::WHITE);
	layoutList->addChild(_editName);

	// 联系方式编辑框
	_editNumber = EditBox::create(Size(visibleSize.width / 2.5f, LISTVIEW_HEIGHT), "background_green.png");
	_editNumber->setPosition(Vec2(visibleSize.width / 30.0f + visibleSize.width / 5.0f, 0));
	_editNumber->setAnchorPoint(Vec2(0.0f, 0.0f));
	_editNumber->setFont("Arial", 24);
	_editNumber->setPlaceHolder("Number:");
	_editNumber->setPlaceholderFont("Arial", 24);
	_editNumber->setPlaceholderFontColor(Color4B::WHITE);
	layoutList->addChild(_editNumber);

	// 选择联系人照片
	auto *selectButton = Button::create("button_select.png");
	selectButton->setScale(LISTVIEW_HEIGHT / selectButton->getContentSize().height);
	selectButton->setContentSize(Size(LISTVIEW_HEIGHT, LISTVIEW_HEIGHT));
	selectButton->setPosition(Vec2(visibleSize.width - LISTVIEW_HEIGHT * 2.0f, 0));
	selectButton->addTouchEventListener(CC_CALLBACK_2(MainScene::onSelectButton, this));
	layoutList->addChild(selectButton);

	// 添加保存按钮
	auto *addSaveButton = Button::create("button_save.jpg");
	addSaveButton->setScale(LISTVIEW_HEIGHT / addSaveButton->getContentSize().height);
	addSaveButton->setContentSize(Size(LISTVIEW_HEIGHT, LISTVIEW_HEIGHT));
	addSaveButton->setPosition(Vec2(visibleSize.width - LISTVIEW_HEIGHT, 0));
	addSaveButton->addTouchEventListener(CC_CALLBACK_2(MainScene::onAddSaveButton, this));
	layoutList->addChild(addSaveButton);

	_listView->pushBackCustomItem(layoutList);

	for (auto contact : contacts)
		addListView(contact.first, contact.second);

	_pageView->addPage(layout);

	for (auto contact : contacts)
		addPageView(contact.second);

	// 强制翻至第二页
	_pageView->scrollToPage(1);

	return true;
}


void MainScene::onSelectButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Button *button = dynamic_cast<Button *>(sender);
		if (button->getTouchBeganPosition() == button->getTouchEndPosition())
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			jobject jobj;
			JniMethodInfo minfo;
			bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");

			if (isHave) {
				CCLOG("getInstance\n");
				jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
			}

			isHave = JniHelper::getMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity", "onSelect", "()V");

			if (isHave) {
				CCLOG("onSelect\n");
				minfo.env->CallVoidMethod(jobj, minfo.methodID);
			}
#endif
		}
	}
}


void MainScene::onAddSaveButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Button *button = dynamic_cast<Button *>(sender);
		if (button->getTouchBeganPosition() == button->getTouchEndPosition())
		{
			size_t index = ContactManager::getInstance().getIndex();
			CONTACT *contact = ContactManager::getInstance().addContact(_editName->getText(), _editNumber->getText(), _imagePath);

			if (contact != NULL)
			{
				addListView(index, contact);
				addPageView(contact);
			}

			_editName->setText("");
			_editNumber->setText("");
			_imagePath = "";
		}
	}
}


void MainScene::onEditButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type, CONTACT *contact)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Button *button = dynamic_cast<Button *>(sender);
		if (button->getTouchBeganPosition() == button->getTouchEndPosition())
		{
			_editName->setText(contact->name.c_str());
			_editNumber->setText(contact->number.c_str());
			_imagePath = contact->image.c_str();
		}
	}
}


void MainScene::onDeleteButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type, size_t index)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Button *button = dynamic_cast<Button *>(sender);
		if (button->getTouchBeganPosition() == button->getTouchEndPosition())
		{
			ContactManager::getInstance().delContact(index);

			size_t index = _listView->getCurSelectedIndex();
			_pageView->removePageAtIndex(index);
			_listView->removeItem(index);
		}
	}
}


void MainScene::onContactButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type, std::string number)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Button *button = dynamic_cast<Button *>(sender);
		if (button->getTouchBeganPosition() == button->getTouchEndPosition())
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			jobject jobj;
			JniMethodInfo minfo;
			bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");

			if (isHave) {
				CCLOG("getInstance\n");
				jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
			}

			isHave = JniHelper::getMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity", "callNumber", "(Ljava/lang/String;)V");

			if (isHave) {
				CCLOG("callNumber\n");
				std::ostringstream oss;
				oss << "tel:" << number;
				jstring jmsg = minfo.env->NewStringUTF(oss.str().c_str());
				minfo.env->CallVoidMethod(jobj, minfo.methodID, jmsg);
			}
#endif
		}
	}
}


void MainScene::addListView(size_t index, CONTACT *contact)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto *layout = Layout::create();
	layout->setLayoutType(Layout::Type::ABSOLUTE);
	layout->setContentSize(Size(visibleSize.width, LISTVIEW_HEIGHT));

	// 名称
	std::string name = "  " + contact->name;
	auto *textName = Text::create(name, "Arial", 24);
	textName->setAnchorPoint(Vec2(0.0f, 0.0f));
	layout->addChild(textName);

	// 联系方式
	auto *textNumber = Text::create(contact->number, "Arial", 24);
	textNumber->setPosition(Vec2(visibleSize.width / 30.0f + visibleSize.width / 5.0f, 0));
	textNumber->setAnchorPoint(Vec2(0.0f, 0.0f));
	layout->addChild(textNumber);

	// 编辑按钮
	auto *editButton = Button::create("button_edit.jpg");
	editButton->setScale(LISTVIEW_HEIGHT / editButton->getContentSize().height);
	editButton->setContentSize(Size(LISTVIEW_HEIGHT, LISTVIEW_HEIGHT));
	editButton->setPosition(Vec2(visibleSize.width - LISTVIEW_HEIGHT * 2.0f, 0));
	editButton->addTouchEventListener(CC_CALLBACK_2(MainScene::onEditButton, this, contact));
	layout->addChild(editButton);

	// 删除按钮
	auto *deleteButton = Button::create("button_delete.png");
	deleteButton->setScale(LISTVIEW_HEIGHT / deleteButton->getContentSize().height);
	deleteButton->setContentSize(Size(LISTVIEW_HEIGHT, LISTVIEW_HEIGHT));
	deleteButton->setPosition(Vec2(visibleSize.width - LISTVIEW_HEIGHT, 0));
	deleteButton->addTouchEventListener(CC_CALLBACK_2(MainScene::onDeleteButton, this, index));
	layout->addChild(deleteButton);

	_listView->pushBackCustomItem(layout);
}


void MainScene::addPageView(CONTACT *contact)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto *layout = Layout::create();
	layout->setPosition(Vec2(0, 0));
	layout->setContentSize(visibleSize);
	layout->setBackGroundImage("background.png");
	layout->setBackGroundImageScale9Enabled(true);

	// 创建联系人按钮图标
	auto *contactButton = Button::create(contact->image);
	contactButton->addTouchEventListener(CC_CALLBACK_2(MainScene::onContactButton, this, contact->number));
	contactButton->setScale(visibleSize.width / contactButton->getContentSize().width);
	contactButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	layout->addChild(contactButton);

	_pageView->addPage(layout);
}


void MainScene::onExitButton(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		Button *button = dynamic_cast<Button *>(sender);
		if (button->getTouchBeganPosition() == button->getTouchEndPosition())
		{
			Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	}
}
