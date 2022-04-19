//
//  MainScene.hpp
//  MyGame-mobile
//
//  Created by Cyril.t on 2022/3/3.
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"

class MainScene : public cocos2d::Scene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif /* MainScene_hpp */
