//
//  SettingScene.hpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//


#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class SettingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuBackCallback(cocos2d::Ref* pSender);
    CheckBox* checkbox[3] = {nullptr};
    void addCheckBox();
    void CheckButtontouchEvent(CheckBox* sender, CheckBox::EventType type);
    cocos2d::Menu* createText();
    cocos2d::Sprite* createBGImage();
    CREATE_FUNC(SettingScene);
};
