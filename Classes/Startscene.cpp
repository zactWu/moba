//
//  Startscene.cpp
//  Glory
//
//  Created by 武信庭 on 2019/5/21.
//

#include "Startscene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Startscene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = Startscene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool Startscene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    addChild(createText());
    addChild(createBGImage());
    
    return true;
}


void Startscene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}

cocos2d::Menu* Startscene::createText() {
    auto buttons = Menu::create();
    
    auto title = MenuItemLabel::create(Label::createWithTTF("Glory", "fonts/Marker Felt.ttf", 45));
    auto label1 = MenuItemLabel::create(Label::createWithTTF("Play", "fonts/Marker Felt.ttf", 37));
    auto label2 = MenuItemLabel::create(Label::createWithTTF("Settings", "fonts/arial.ttf", 25));
    auto label3 = MenuItemLabel::create(Label::createWithTTF("Help", "fonts/arial.ttf", 25));
    auto closeItem = MenuItemLabel::create(
                                        Label::createWithTTF("Exit", "fonts/arial.ttf", 25),
                                           CC_CALLBACK_1(Startscene::menuCloseCallback, this));
    
    title->setPosition(570, 600);
    label1->setPosition(570, 430);
    label2->setPosition(label2->getContentSize().width / 2 + 200, 75);
    label3->setPosition(label3->getContentSize().width / 2 + 550, 75);
    closeItem->setPosition(closeItem->getContentSize().width / 2 + 900, 75);
    
    ccColor3B color = ccc3(0,0,0);
    title->setColor(ccc3(0,0,0));
    label1->setColor(ccc3(34,139,34));
    buttons->addChild(title, 1);
    buttons->addChild(label1, 1);
    buttons->addChild(label2, 1);
    buttons->addChild(label3, 1);
    buttons->addChild(closeItem, 1);
    
    buttons->setPosition(0, 0);
    
    return buttons;
}

cocos2d::Sprite* Startscene::createBGImage() {
    auto size = Director::getInstance()->getVisibleSize();
    auto background = Sprite::create("Startscene/background.jpg");
    background->setLocalZOrder(-1);
    background->setPosition(size.width / 2, size.height / 2);
    return background;
}






