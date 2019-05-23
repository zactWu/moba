//
//  Help.hpp
//  
//
//  Created by 武信庭 on 2019/5/23.
//

#include <stdio.h>
#include "cocos2d.h"

class Help : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelpScene);
};
