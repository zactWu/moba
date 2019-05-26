//
//  SettingScene.hpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//


#include <stdio.h>
#include "cocos2d.h"

<<<<<<< HEAD
class SettingScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
=======
class SettingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuBackCallback(cocos2d::Ref* pSender);
    cocos2d::Menu* createText();
    cocos2d::Sprite* createBGImage();
>>>>>>> startscene
    CREATE_FUNC(SettingScene);
};
