//
//  ShopLayer.h
//  Glory
//
//  Created by 武信庭 on 2019/6/7.
//
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Hero.h"

using namespace cocos2d;
using namespace cocos2d::extension;
class ShopLayer :public Layer
{
public:
    bool init();
    CREATE_FUNC(ShopLayer);
    void shopMenuCallback(Ref* psender);
    void buyCallback(Ref* psender,int price);
    static ShopLayer* createLayer(Hero* hero1);
    ScrollView* mscrollView;
    Point touchPoint;
    Point offsetPoint;
    int m_nCurPage;
    
};
