//
//  SettingScene.cpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//

#include "SettingScene.h"
#include "GlobalVal.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

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
    auto hero1 = Sprite::create("SettingScene/test1.png");
    auto hero2 = Sprite::create("SettingScene/test2.png");
    auto hero3 = Sprite::create("SettingScene/test3.png");
    hero1->setPosition(Vec2(320,350));
    hero2->setPosition(Vec2(570,350));
    hero3->setPosition(Vec2(820,350));
    addChild(hero1);
    addChild(hero2);
    addChild(hero3);
    addCheckBox();
    
    
    return true;
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
        checkbox[i]->setScale(0.15);
        checkbox[i]->setPosition(Vec2(320+250*i,150));
        checkbox[i]->setTag(i+1);
        checkbox[i]->addEventListenerCheckBox(this, checkboxselectedeventselector(SettingScene::CheckButtontouchEvent));
        this->addChild(checkbox[i], 1);
    }
}

void SettingScene::CheckButtontouchEvent(CheckBox* sender, CheckBox::EventType type)
{
    int tag = sender->getTag();
    for (int i=0; i<3; i++) {//for循环实现单选
        checkbox[i]->setSelected(false);//先让全部按钮设为处于未选中状态
        if (i+1 == tag) {//通过tag值进入判断
            checkbox[i]->setSelected(true);//让当前按钮为选中
            hero_id = tag;
            log("hero is %d", hero_id);
            }
        }
}
