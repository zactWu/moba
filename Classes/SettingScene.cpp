//
//  SettingScene.cpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//

#include "SettingScene.h"
#include "GlobalVal.h"
#include "ui/CocosGUI.h"
#include <string>
USING_NS_CC;
using namespace cocos2d::ui;
std::string ipMessage = "127.0.0.1";
Scene* SettingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SettingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool SettingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    addChild(createText());
    addChild(createBGImage());
    auto hero1 = Sprite::create("SettingScene/warrior.jpg");
    auto hero2 = Sprite::create("SettingScene/archer.jpg");
    auto hero3 = Sprite::create("SettingScene/magician.jpg");
    auto dark = Label::createWithTTF("Dark", "fonts/Quicksand-Bold.ttf", 25);
    auto bright = Label::createWithTTF("Bright", "fonts/Quicksand-Bold.ttf", 25);
    auto heroselect = Label::createWithTTF("Your Hero", "fonts/Montserrat-Medium.ttf", 27);
    auto e_heroselect = Label::createWithTTF("Enemy Hero", "fonts/Montserrat-Medium.ttf", 27);
    auto side = Label::createWithTTF("Side", "fonts/Montserrat-Medium.ttf", 27);
    hero1->setScale(0.2);
    hero2->setScale(0.2);
    hero3->setScale(0.2);
    hero1->setPosition(Vec2(320,480));
    hero2->setPosition(Vec2(570,480));
    hero3->setPosition(Vec2(820,480));
    dark->setPosition(Vec2(370,150));
    bright->setPosition(Vec2(670,150));
    heroselect->setPosition(Vec2(150,350));
    e_heroselect->setPosition(Vec2(150,250));
    side->setPosition(Vec2(150,150));
    /*heroselect->setColor(Color3B(25,25,112));
    e_heroselect->setColor(Color3B(25,25,112));
    side->setColor(Color3B(25,25,112));*/
    side->enableOutline(Color4B(255,0,0,255),3);
    heroselect->enableOutline(Color4B(255,0,0,255),3);
    e_heroselect->enableOutline(Color4B(255,0,0,255),3);
    addChild(hero1);
    addChild(hero2);
    addChild(hero3);
    addChild(dark);
    addChild(bright);
    addChild(heroselect);
    addChild(e_heroselect);
    addChild(side);
    addCheckBox();
    
	auto ip = cocos2d::ui::TextField::create("ip", "Arial", 30);
	ip->setMaxLength(100);
	//下面设置键盘Enter监听，用于聊天
	this->addChild(ip, 10);
	ip->setPosition(Vec2(150, 50));
	ip->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		});
	auto ketboard_listener = EventListenerKeyboard::create();
	ketboard_listener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* event) {
		if (keycode == EventKeyboard::KeyCode::KEY_ENTER)
		{
			ipMessage = ip->getString();
			
			ip->setString("");
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ketboard_listener, this);
}

cocos2d::Menu* SettingScene::createText() {
    const auto buttons = Menu::create();
    
    const auto backButton = MenuItemLabel::create(
                                                  Label::createWithTTF("Back", "fonts/OpenSans-Regular.ttf", 25),
                                                  CC_CALLBACK_1(SettingScene::menuBackCallback, this));
    const auto title = MenuItemLabel::create(Label::createWithTTF("Setting", "fonts/Quicksand-Bold.ttf", 45));
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto baseY = visibleSize.height * 0.85f;
    
    backButton->setPosition(backButton->getContentSize().width / 2 + 30, baseY + 30);
    title->setPosition(570, 600);
    ccColor3B color = ccc3(0,0,0);
    title->setColor(ccc3(0,0,0));
    
    buttons->addChild(backButton, 1);
    buttons->addChild(title,1);
    buttons->setPosition(0, 0);
    
    return buttons;
}

cocos2d::Sprite* SettingScene::createBGImage(){
    auto size = Director::getInstance()->getVisibleSize();
    auto background = Sprite::create("Startscene/background.jpg");
    background->setLocalZOrder(-1);
    background->setPosition(size.width / 2, size.height / 2);
    return background;
}

void SettingScene::menuBackCallback(Ref* pSender)
{
    Director::getInstance()->popScene();
}

void  SettingScene::addCheckBox()
{
    for(int i=0;i<3;i++){
        checkbox[i] = CheckBox::create("SettingScene/dchoose.jpg", "SettingScene/choose.jpg", TextureResType::LOCAL);  
        checkbox[i]->setScale(0.08);
        checkbox[i]->setPosition(Vec2(320+250*i,350));
        checkbox[i]->setTag(i+1);
        checkbox[i]->addEventListenerCheckBox(this, checkboxselectedeventselector(SettingScene::CheckButtontouchEvent));
        this->addChild(checkbox[i], 1);
    }
    for(int i=0;i<3;i++){
        e_checkbox[i] = CheckBox::create("SettingScene/dchoose.jpg", "SettingScene/choose.jpg", TextureResType::LOCAL);
        e_checkbox[i]->setScale(0.08);
        e_checkbox[i]->setPosition(Vec2(320+250*i,250));
        e_checkbox[i]->setTag(i+11);
        e_checkbox[i]->addEventListenerCheckBox(this, checkboxselectedeventselector(SettingScene::e_CheckButtontouchEvent));
        this->addChild(e_checkbox[i], 1);
    }
    for(int i=0;i<2;i++){
        c_checkbox[i] = CheckBox::create("SettingScene/dchoose.jpg", "SettingScene/choose.jpg", TextureResType::LOCAL);
        c_checkbox[i]->setScale(0.08);
        c_checkbox[i]->setPosition(Vec2(450+300*i,150));
        c_checkbox[i]->setTag(i+21);
        c_checkbox[i]->addEventListenerCheckBox(this, checkboxselectedeventselector(SettingScene::c_CheckButtontouchEvent));
        this->addChild(c_checkbox[i], 1);
    }
}

void SettingScene::CheckButtontouchEvent(CheckBox* sender, CheckBox::EventType type)
{
    int tag = sender->getTag();
    for (int i=0; i<3; i++) {//for循环实现单选
        checkbox[i]->setSelected(false);//先让全部按钮设为处于未选中状态
        if (i+1 == tag) {
            checkbox[i]->setSelected(true);//让当前按钮为选中
            hero_id = tag;
            log("hero is %d", hero_id);
            }
        }
}

void SettingScene::e_CheckButtontouchEvent(CheckBox* sender, CheckBox::EventType type)
{
    int tag = sender->getTag();
    for (int i=0; i<3; i++) {//for循环实现单选
        e_checkbox[i]->setSelected(false);//先让全部按钮设为处于未选中状态
        if(i+11 == tag){
            e_checkbox[i]->setSelected(true);//让当前按钮为选中
            en_hero_id = tag-10;
            log("enemy hero is %d", en_hero_id);
        }
    }
}

void SettingScene::c_CheckButtontouchEvent(CheckBox* sender, CheckBox::EventType type)
{
    int tag = sender->getTag();
    for (int i=0; i<2; i++) {//for循环实现单选
        c_checkbox[i]->setSelected(false);//先让全部按钮设为处于未选中状态
        if(i+21 == tag){
            c_checkbox[i]->setSelected(true);//让当前按钮为选中
            this_computer_side = tag-21;
            log("computer is %d", this_computer_side);
        }
    }
}

