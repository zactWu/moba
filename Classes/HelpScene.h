//
//  Help.hpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//

#include <stdio.h>
#include "cocos2d.h"

<<<<<<< HEAD
class Help : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
//    CREATE_FUNC(HelpScene);		//这一行有问题，以后再改
=======
class HelpScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuBackCallback(cocos2d::Ref* pSender);
    cocos2d::Menu* createText();
    cocos2d::Sprite* createBGImage();
    CREATE_FUNC(HelpScene);
>>>>>>> startscene
};
