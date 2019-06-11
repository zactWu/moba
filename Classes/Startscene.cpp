//
//  Startscene.cpp
//  Glory
//
//  Created by 武信庭 on 2019/5/21.
//

#include "Startscene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"


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



void Startscene::menuPlayCallback(cocos2d::Ref * pSender) {
    const auto scene = GameScene::createScene();
    Director::getInstance()->pushScene(scene);
}


void Startscene::menuSettingCallback(cocos2d::Ref * pSender) {
    const auto scene = SettingScene::createScene();
    Director::getInstance()->pushScene(scene);
}

void Startscene::menuHelpCallback(cocos2d::Ref * pSender) {
    const auto scene = HelpScene::createScene();
    Director::getInstance()->pushScene(scene);
}

void Startscene::menuExitCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}

cocos2d::Menu* Startscene::createText() {
    auto buttons = Menu::create();
    auto label1content = Label::createWithTTF("Play", "fonts/Quicksand-Bold.ttf", 39);
    auto title = MenuItemLabel::create(Label::createWithTTF("Glory", "fonts/KingArthurLegend.ttf", 45));
    auto label1 = MenuItemLabel::create(label1content,CC_CALLBACK_1(Startscene::menuPlayCallback, this));
    auto label2 = MenuItemLabel::create(Label::createWithTTF("Setting", "fonts/OpenSans-Regular.ttf", 25),CC_CALLBACK_1(Startscene::menuSettingCallback, this));
    auto label3 = MenuItemLabel::create(Label::createWithTTF("Help", "fonts/OpenSans-Regular.ttf", 25),CC_CALLBACK_1(Startscene::menuHelpCallback, this));
    auto closeItem = MenuItemLabel::create(Label::createWithTTF("Exit", "fonts/OpenSans-Regular.ttf", 25),CC_CALLBACK_1(Startscene::menuExitCallback, this));
    
    
    title->setPosition(570, 600);
    label1->setPosition(570, 430);
    label3->setPosition(570, 75);
    label2->setPosition(370, 75);
    closeItem->setPosition(770, 75);
    
    label1content->enableOutline(Color4B(0,0,0,255),2);
    title->setColor(Color3B(0,0,0));
    label1->setColor(Color3B(34,139,34));
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






