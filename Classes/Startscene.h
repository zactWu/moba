//
//  Startscene.hpp
//  Glory
//
//  Created by 武信庭 on 2019/5/21.
//
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SettingScene.h"
#include "HelpScene.h"

class Startscene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    cocos2d::Menu* createText();
    cocos2d::Sprite* createBGImage();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);
    //void menuPlayCallback(cocos2d::Ref* pSender);
    void menuHelpCallback(cocos2d::Ref * pSender);
    void menuSettingCallback(cocos2d::Ref * pSender);
    CREATE_FUNC(Startscene);
};

#endif // __HELLOWORLD_SCENE_H__
