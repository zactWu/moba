//
//  ShopLayer.cpp
//  Glory
//
//  Created by 武信庭 on 2019/6/7.
//
#define Shopitem 101

#include "ShopLayer.h"
#include<cmath>


Hero* hero;
ShopLayer* ShopLayer::createLayer(Hero* hero1)
{
    hero = hero1;
    return ShopLayer::create();
}

bool ShopLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        Size winSize = Director::getInstance()->getWinSize();
        ScrollView * scrollView = ScrollView::create(Size(winSize.width*0.8, winSize.height*0.5));
        scrollView->setBounceable(true);
        scrollView->ignoreAnchorPointForPosition(false);
        scrollView->setPosition(Point(winSize.width/2, winSize.height/2));
        scrollView->setDirection(ScrollView::Direction::VERTICAL);
        
        Layer * layer = Layer::create();
        auto layerSize = layer->getContentSize();
        layer->setPosition(0, 0);
        scrollView->setBounceable(true);
        
        //1.鞋子
        Label* move = Label::createWithTTF("Travel Boots", "fonts/aslangbary.otf", 35);
        move ->setPosition(0.15 * winSize.width,  winSize.height);layer->addChild(move, Shopitem);
        auto shoes = MenuItemImage::create("shop/shoe.png", "shop/shoe.png");
        shoes->setScale(0.4f);shoes->setPosition(0.3*winSize.width, winSize.height);shoes->setTag(1);
        auto content_shoes = MenuItemLabel::create(Label::createWithTTF("Speed+30                  $500", "fonts/Quicksand-Bold.ttf", 20));content_shoes->setPosition(0.5*winSize.width, winSize.height);
        auto buy_shoes = MenuItemImage::create("shop/buy.png","shop/buy.png");
        buy_shoes->setScale(0.3f);buy_shoes->setPosition(0.7*winSize.width, winSize.height);
        
        //2.净魂之刃
        Label* swordname = Label::createWithTTF("Diffusal Blade", "fonts/aslangbary.otf", 35);
        swordname->setPosition(0.15 * winSize.width, 0.85* winSize.height);layer->addChild(swordname, Shopitem);
        auto sword = MenuItemImage::create("shop/sword.png", "shop/sword.png");
        sword->setScale(0.4f);sword->setPosition(0.3*winSize.width,0.85*winSize.height);sword->setTag(2);
        auto content_sword = MenuItemLabel::create(Label::createWithTTF("Attack+5                  $500", "fonts/Quicksand-Bold.ttf", 20));content_sword->setPosition(0.5*winSize.width, 0.85*winSize.height);
        auto buy_sword = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,300));
        buy_sword->setScale(0.3f);buy_sword->setPosition(0.7*winSize.width, 0.85*winSize.height);
        
        //3.先锋盾
        Label* shieldname = Label::createWithTTF("Vanguard", "fonts/aslangbary.otf", 35);
        shieldname->setPosition(0.15 * winSize.width, 0.7* winSize.height);layer->addChild(shieldname, Shopitem);
        auto shield = MenuItemImage::create("shop/shield.png", "shop/shield.png");
        shield->setScale(0.4f);shield->setPosition(0.3*winSize.width,0.7*winSize.height);shield->setTag(3);
        auto content_shield = MenuItemLabel::create(Label::createWithTTF("Defence+5                $300", "fonts/Quicksand-Bold.ttf", 20));content_shield->setPosition(0.5*winSize.width, 0.7*winSize.height);
        auto buy_shield = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,300));
        buy_shield->setScale(0.3f);buy_shield->setPosition(0.7*winSize.width, 0.7*winSize.height);
        
        //4.大根
        Label* wandname = Label::createWithTTF("Power Of Dagon", "fonts/aslangbary.otf", 35);
        wandname->setPosition(0.15 * winSize.width, 0.55* winSize.height);layer->addChild(wandname, Shopitem);
        auto wand = MenuItemImage::create("shop/wand.png", "shop/wand.png");
        wand->setScale(0.4f);wand->setPosition(0.3*winSize.width,0.55*winSize.height);wand->setTag(4);
        auto content_wand = MenuItemLabel::create(Label::createWithTTF("Attackrange+10                   $300", "fonts/Quicksand-Bold.ttf", 20));content_wand->setPosition(0.5*winSize.width, 0.55*winSize.height);
        auto buy_wand = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,300));
        buy_wand->setScale(0.3f);buy_wand->setPosition(0.7*winSize.width, 0.55*winSize.height);
        
        //5.散夜对剑
        Label* doubleswordname = Label::createWithTTF("Double Sword", "fonts/aslangbary.otf", 35);
        doubleswordname->setPosition(0.15 * winSize.width, 0.4* winSize.height);layer->addChild(doubleswordname, Shopitem);
        auto doublesword = MenuItemImage::create("shop/doublesword.png", "shop/doublesword.png");
        doublesword->setScale(0.4f);doublesword->setPosition(0.3*winSize.width,0.4*winSize.height);doublesword->setTag(5);
        auto content_doublesword = MenuItemLabel::create(Label::createWithTTF("Attack+5 Attackrange+10        $550", "fonts/Quicksand-Bold.ttf", 20));content_doublesword->setPosition(0.5*winSize.width, 0.4*winSize.height);
        auto buy_doublesword = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,550));
        buy_doublesword->setScale(0.3f);buy_doublesword->setPosition(0.7*winSize.width, 0.4*winSize.height);
        
        //6.龙心
        Label* heartname = Label::createWithTTF("Heart Of Tarrasque", "fonts/aslangbary.otf", 35);
        heartname->setPosition(0.15 * winSize.width, 0.25* winSize.height);layer->addChild(heartname, Shopitem);
        auto heart = MenuItemImage::create("shop/heart.png", "shop/heart.png");
        heart->setScale(0.4f);heart->setPosition(0.3*winSize.width,0.25*winSize.height);heart->setTag(6);
        auto content_heart = MenuItemLabel::create(Label::createWithTTF("Max HP+25                  $600", "fonts/Quicksand-Bold.ttf", 20));content_heart->setPosition(0.5*winSize.width, 0.25*winSize.height);
        auto buy_heart = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,600));
        buy_heart->setScale(0.3f);buy_heart->setPosition(0.7*winSize.width, 0.25*winSize.height);
        
        //7.阿哈利姆神杖
        Label* sceptername = Label::createWithTTF("Aghanim's Scepter", "fonts/aslangbary.otf", 35);
        sceptername->setPosition(0.15 * winSize.width, 0.1* winSize.height);layer->addChild(sceptername, Shopitem);
        auto scepter = MenuItemImage::create("shop/scepter.png", "shop/scepter.png");
        scepter->setScale(0.4f);scepter->setPosition(0.3*winSize.width,0.1*winSize.height);scepter->setTag(7);
        auto content_scepter = MenuItemLabel::create(Label::createWithTTF("All Promote!                         $1000", "fonts/Quicksand-Bold.ttf", 20));content_scepter->setPosition(0.5*winSize.width, 0.1*winSize.height);
        auto buy_scepter = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,1000));
        buy_scepter->setScale(0.3f);buy_scepter->setPosition(0.7*winSize.width, 0.1*winSize.height);
        
       
        Menu* shop = Menu::create(shoes,content_shoes,buy_shoes,
                                  sword,content_sword,buy_sword,
                                  shield,content_shield,buy_shield,
                                  wand,content_wand,buy_wand,
                                  doublesword,content_doublesword,buy_doublesword,
                                  heart,content_heart,buy_heart,
                                  scepter,content_scepter,buy_scepter,
                                  NULL
                                  );
        
        shop->setPosition(Point::ZERO);
        layer->addChild(shop);
        
        scrollView->setContainer(layer);
        scrollView->setContentSize(Size(winSize.width/2, 110*winSize.height/100));
        scrollView->setContentOffset(Point(0, 0));
        this->addChild(scrollView,150);
        mscrollView = scrollView;
        bRet = true;
    } while (0);
    
    return bRet;
}

void ShopLayer::buyCallback(Ref* psender,int price)
{
    removeChildByTag(100);
    Size winSize = Director::getInstance()->getWinSize();
    if (hero->_money < price)
    {
        Label* label = Label::createWithSystemFont("You Need More Money!", "fonts/Quicksand-Bold.ttf", 24);
        label->setPosition(0.5*winSize.width, 0.9*winSize.height);
        label->setColor(Color3B(139,0,0));
        label->setTag(100);
        this->addChild(label,200);
        return;
    }
    else{
        Label* label = Label::createWithSystemFont("It Belongs To You Now!", "fonts/Quicksand-Bold.ttf", 24);
        label->setPosition(0.5*winSize.width, 0.9*winSize.height);
        label->setColor(Color3B(139,0,0));
        label->setTag(100);
        this->addChild(label,200);
    }
    hero->_money -= price;
    auto itemSelect = (MenuItem*)psender;
    int num = itemSelect->getTag();
    switch (num)
    {
        case 1:
            hero->_speed += 30;//购买鞋子没反应
            break;
        case 2:
            hero->_attack += 5;
            break;
        case 3:
            hero->_defense += 5;
            break;
        case 4:
            hero->_attackRange += 10;
            break;
        case 5:
            hero->_attack += 5;
            hero->_attackRange += 10;
            break;
        case 6:
            hero->_life_max += 25;
            break;
        case 7:
            hero->_speed += 20;
			hero->_attack += 3;
			hero->_defense += 3;
			hero->_attackRange += 8;
			hero->_life_max += 20;
            break;
    }
    return;
}
