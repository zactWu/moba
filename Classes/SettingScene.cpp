//
//  SettingScene.cpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//

#include "SettingScene.h"

USING_NS_CC;

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

cocos2d::Sprite* SettingScene::createBGImage()
{
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

