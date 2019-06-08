//
//  ShopLayer.cpp
//  Glory
//
//  Created by 武信庭 on 2019/6/7.
//

#include "ShopLayer.h"
#include<cmath>

int money =500;
int speed = 2;
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
        move ->setPosition(0.15 * winSize.width,  winSize.height);layer->addChild(move);
        auto shoes = MenuItemImage::create("shop/shoe.png", "shop/shoe.png");
        shoes->setScale(0.4f);shoes->setPosition(0.3*winSize.width, winSize.height);shoes->setTag(1);
        auto content_shoes = MenuItemLabel::create(Label::createWithTTF("Speed+20                  $250", "fonts/Quicksand-Bold.ttf", 20));content_shoes->setPosition(0.5*winSize.width, winSize.height);
        auto buy_shoes = MenuItemImage::create("shop/buy.png","shop/buy.png");
        buy_shoes->setScale(0.3f);buy_shoes->setPosition(0.7*winSize.width, winSize.height);
        
        //2.净魂之刃
        Label* swordname = Label::createWithTTF("Diffusal Blade", "fonts/aslangbary.otf", 35);
        swordname->setPosition(0.15 * winSize.width, 0.85* winSize.height);layer->addChild(swordname);
        auto sword = MenuItemImage::create("shop/sword.png", "shop/sword.png");
        sword->setScale(0.4f);sword->setPosition(0.3*winSize.width,0.85*winSize.height);sword->setTag(2);
        auto content_sword = MenuItemLabel::create(Label::createWithTTF("Attack+20                  $300", "fonts/Quicksand-Bold.ttf", 20));content_sword->setPosition(0.5*winSize.width, 0.85*winSize.height);
        auto buy_sword = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,300));
        buy_sword->setScale(0.3f);buy_sword->setPosition(0.7*winSize.width, 0.85*winSize.height);
        
        //3.先锋盾
        Label* shieldname = Label::createWithTTF("Vanguard", "fonts/aslangbary.otf", 35);
        shieldname->setPosition(0.15 * winSize.width, 0.7* winSize.height);layer->addChild(shieldname);
        auto shield = MenuItemImage::create("shop/shield.png", "shop/shield.png");
        shield->setScale(0.4f);shield->setPosition(0.3*winSize.width,0.7*winSize.height);shield->setTag(3);
        auto content_shield = MenuItemLabel::create(Label::createWithTTF("Defence+20                $300", "fonts/Quicksand-Bold.ttf", 20));content_shield->setPosition(0.5*winSize.width, 0.7*winSize.height);
        auto buy_shield = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,300));
        buy_shield->setScale(0.3f);buy_shield->setPosition(0.7*winSize.width, 0.7*winSize.height);
        
        //4.大根
        Label* wandname = Label::createWithTTF("Power Of Dagon", "fonts/aslangbary.otf", 35);
        wandname->setPosition(0.15 * winSize.width, 0.55* winSize.height);layer->addChild(wandname);
        auto wand = MenuItemImage::create("shop/wand.png", "shop/wand.png");
        wand->setScale(0.4f);wand->setPosition(0.3*winSize.width,0.55*winSize.height);wand->setTag(4);
        auto content_wand = MenuItemLabel::create(Label::createWithTTF("Spell+20                   $300", "fonts/Quicksand-Bold.ttf", 20));content_wand->setPosition(0.5*winSize.width, 0.55*winSize.height);
        auto buy_wand = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,300));
        buy_wand->setScale(0.3f);buy_wand->setPosition(0.7*winSize.width, 0.55*winSize.height);
        
        //4.散夜对剑
        Label* doubleswordname = Label::createWithTTF("Double Sword", "fonts/aslangbary.otf", 35);
        doubleswordname->setPosition(0.15 * winSize.width, 0.4* winSize.height);layer->addChild(doubleswordname);
        auto doublesword = MenuItemImage::create("shop/doublesword.png", "shop/doublesword.png");
        doublesword->setScale(0.4f);doublesword->setPosition(0.3*winSize.width,0.4*winSize.height);doublesword->setTag(5);
        auto content_doublesword = MenuItemLabel::create(Label::createWithTTF("Attack+20  Spell+20         $550", "fonts/Quicksand-Bold.ttf", 20));content_doublesword->setPosition(0.48*winSize.width, 0.4*winSize.height);
        auto buy_doublesword = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,550));
        buy_doublesword->setScale(0.3f);buy_doublesword->setPosition(0.7*winSize.width, 0.4*winSize.height);
        
        //5.龙心
        Label* heartname = Label::createWithTTF("Heart Of Tarrasque", "fonts/aslangbary.otf", 35);
        heartname->setPosition(0.15 * winSize.width, 0.25* winSize.height);layer->addChild(heartname);
        auto heart = MenuItemImage::create("shop/heart.png", "shop/heart.png");
        heart->setScale(0.4f);heart->setPosition(0.3*winSize.width,0.25*winSize.height);heart->setTag(6);
        auto content_heart = MenuItemLabel::create(Label::createWithTTF("Max HP+20                  $600", "fonts/Quicksand-Bold.ttf", 20));content_heart->setPosition(0.5*winSize.width, 0.25*winSize.height);
        auto buy_heart = MenuItemImage::create("shop/buy.png","shop/buy.png",CC_CALLBACK_1(ShopLayer::buyCallback,this,600));
        buy_heart->setScale(0.3f);buy_heart->setPosition(0.7*winSize.width, 0.25*winSize.height);
        
        //6.阿哈利姆神杖
        Label* sceptername = Label::createWithTTF("Aghanim's Scepter", "fonts/aslangbary.otf", 35);
        sceptername->setPosition(0.15 * winSize.width, 0.1* winSize.height);layer->addChild(sceptername);
        auto scepter = MenuItemImage::create("shop/scepter.png", "shop/scepter.png");
        scepter->setScale(0.4f);scepter->setPosition(0.3*winSize.width,0.1*winSize.height);scepter->setTag(7);
        auto content_scepter = MenuItemLabel::create(Label::createWithTTF("All+10                           $1000", "fonts/Quicksand-Bold.ttf", 20));content_scepter->setPosition(0.5*winSize.width, 0.1*winSize.height);
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
        this->addChild(scrollView);
        mscrollView = scrollView;
        bRet = true;
    } while (0);
    
    return bRet;
}

void ShopLayer::buyCallback(Ref* psender,int price)
{
    removeChildByTag(100);//删除残留信息
    Size winSize = Director::getInstance()->getWinSize();
    if (money < price)//调用英雄金币来判断钱够不够
    {
        Label* label = Label::createWithSystemFont("You Need More Money!", "fonts/Quicksand-Bold.ttf", 24);
        label->setPosition(0.5*winSize.width, 0.9*winSize.height);
        label->setColor(Color3B(139,0,0));
        label->setTag(100);
        this->addChild(label);
        return;
    }
    else{
        Label* label = Label::createWithSystemFont("It Belongs To You Now!", "fonts/Quicksand-Bold.ttf", 24);
        label->setPosition(0.5*winSize.width, 0.9*winSize.height);
        label->setColor(Color3B(139,0,0));
        label->setTag(100);
        this->addChild(label);
    }
    money -= price;
    auto itemSelect = (MenuItem*)psender;
    int num = itemSelect->getTag();
    switch (num)
    {
        case 1:
            hero->speed += 2;
            break;
        case 2:
            hero->Attack += 20;
            hero->Weapon[2]++;
            break;
        case 3:
            hero->Attack_Speed += 10;
            hero->Weapon[3]++;
            break;
        case 4:
            hero->Critical_Rate += 10;
            hero->Weapon[4]++;
            break;
        case 5:
            hero->Attack += 80;
            hero->Weapon[5]++;
            break;
        case 6:
            hero->Attack_Speed += 25;
            hero->Weapon[6]++;
            break;
        case 7:
            hero->Critical_Rate += 25;
            hero->Weapon[7]++;
            break;
        case 8:
            hero->Attack += 160;
            hero->Weapon[8]++;
            break;
        case 9:
            hero->Attack_Speed += 40;
            hero->Weapon[9]++;
            break;
        case 10:
            hero->Critical_Rate += 50;
            hero->Weapon[10]++;
            break;
        case 11:
            hero->Skill_Enhance += 5;
            hero->Weapon[11]++;
            break;
        case 12:
            hero->MaxMP += 300;
            hero->Weapon[12]++;
            break;
        case 13:
            hero->MP_Recover += 5;
            hero->Weapon[13]++;
            break;
        case 14:
            hero->Skill_Enhance += 15;
            hero->Weapon[14]++;
            break;
        case 15:
            hero->MaxMP += 400;
            hero->Weapon[15]++;
            break;
        case 16:
            hero->MP_Recover += 15;
            hero->Weapon[16]++;
            break;
        case 17:
            hero->Skill_Enhance += 50;
            hero->Weapon[17]++;
            break;
        case 18:
            hero->MaxMP += 1000;
            hero->Weapon[18]++;
            break;
        case 19:
            hero->MP_Recover += 40;
            hero->Weapon[19]++;
            break;
        case 20:
            hero->MaxHP += 300;
            hero->Weapon[20]++;
            break;
        case 21:
            hero->Defense += 90;
            hero->Weapon[21]++;
            break;
        case 22:
            hero->Resistance += 90;
            hero->Weapon[22]++;
            break;
        case 23:
            hero->MaxHP += 1000;
            hero->Weapon[23]++;
            break;
        case 24:
            hero->Defense += 210;
            hero->Weapon[24]++;
            break;
        case 25:
            hero->Resistance += 150;
            hero->Weapon[25]++;
            break;
        case 26:
            hero->MaxHP += 2000;
            hero->Weapon[26]++;
            break;
        case 27:
            hero->Defense += 400;
            hero->Weapon[27]++;
            break;
        case 28:
            hero->Resistance += 320;
            hero->Weapon[28]++;
            break;
        case 29:
            
            break;
        case 30:
            
            break;
        case 31:
            
            break;
    }
    return;
}
