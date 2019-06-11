//
//  Help.cpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//

#include "HelpScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ShopLayer.h"

USING_NS_CC;

Scene* HelpScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelpScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool HelpScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    const auto content1 = Label::createWithTTF("1.Run server, open client and choose hero in Setting, then enjoy the game!", "fonts/Quicksand-Bold.ttf", 24);
    const auto content2 = Label::createWithTTF("2.Control your hero by mouse and release skills by Q W E on keyboard!", "fonts/Quicksand-Bold.ttf", 24);
    const auto content3 = Label::createWithTTF("3.Don't forget to use chatbox button in the left-bottom side to chat!", "fonts/Quicksand-Bold.ttf", 24);
    const auto content4 = Label::createWithTTF("4.The crazy creeps will march after a while, be careful of them!", "fonts/Quicksand-Bold.ttf", 24);
    const auto content5 = Label::createWithTTF("5.Heartfelt thanks to all developer and supporter of Glory!", "fonts/Quicksand-Bold.ttf", 24);
    auto contentbg = Sprite::create("shop/window.png");
    
    content1->setPosition(570,500);
    content2->setPosition(570,400);
    content3->setPosition(570,300);
    content4->setPosition(570,200);
    content5->setPosition(570,100);
    contentbg->setPosition(570,300);
    contentbg->setScale(3.3);
    addChild(content1,2);
    addChild(content2,2);
    addChild(content3,2);
    addChild(content4,2);
    addChild(content5,2);
    addChild(contentbg,1);
    addChild(createText(),2);
    addChild(createBGImage());
    

    return true;
}


cocos2d::Menu* HelpScene::createText()
{
    const auto buttons = Menu::create();
    
    const auto backButton=MenuItemLabel::create(Label::createWithTTF("Back", "fonts/OpenSans-Regular.ttf", 25),
                                                CC_CALLBACK_1(HelpScene::menuBackCallback, this));
    const auto title = MenuItemLabel::create(Label::createWithTTF("Help", "fonts/Quicksand-Bold.ttf", 45));
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto baseY = visibleSize.height * 0.85f;
    
    backButton->setPosition(backButton->getContentSize().width / 2 + 60, baseY + 30);
    title->setPosition(570, 600);
    title->setColor(Color3B(0,0,0));
    
    buttons->addChild(backButton, 1);
    buttons->addChild(title,1);
    buttons->setPosition(0, 0);
    
    return buttons;
}

cocos2d::Sprite* HelpScene::createBGImage()
{
    auto size = Director::getInstance()->getVisibleSize();
    auto background = Sprite::create("Startscene/background.jpg");
    background->setLocalZOrder(-1);
    background->setPosition(size.width / 2, size.height / 2);
    return background;
}

void HelpScene::menuBackCallback(Ref* pSender)
{
    Director::getInstance()->popScene();
}


