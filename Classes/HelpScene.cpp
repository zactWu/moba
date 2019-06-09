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
  
    addChild(createText());
    addChild(createBGImage(),50);
    
    equipment = MenuItemImage::create("shop/shopbutton.png","shop/shopbutton.png",CC_CALLBACK_1(HelpScene::gotoshop,this));
    equipment->setScale(0.2);
    equipment->setPosition(1050,625);
    equipment->setTag(0);
    auto menu = Menu::create(equipment, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,100);
   
    
    return true;
}

//一起搬过去XD
void HelpScene::gotoshop(Ref* psender){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto winsize = Director::getInstance()->getWinSize();
    if (equipment->getTag() == 0)
    {
        Sprite* window = Sprite::create("shop/window.png");
        auto contentsize = window->getContentSize();
        window->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        window->setScale(winsize.width*0.8/contentsize.width,winsize.height*0.5/contentsize.height);
        this->addChild(window, 0);
        ShopLayer* scrollView = ShopLayer::createLayer(hero);
        window->setName("rm1");
        scrollView->setName("rm2");
        this->addChild(scrollView,51);
        equipment->setTag(1);
    }
    else
    {
        this->removeChildByName("rm1");
        this->removeChildByName("rm2");
        equipment->setTag(0);
    }
    
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


