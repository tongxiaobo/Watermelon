//
//  GameScene.hpp
//  Watermelon-mobile
//
//  Created by Cyril.t on 2022/3/7.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class GameScene : public cocos2d::Scene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void onMenuCloseCallback(cocos2d::Ref* sender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    Label* scoreLabel;
    ui::ImageView* ground;
    Sprite* fruit;
    Vector<Sprite*> newFruits;
    bool isReady;
    int score;

    Sprite* createFruit(int number);
    void addPhysics(Sprite* fruit);
    void showFruit();
    void calculateScore(int number);
    bool touchBegan(Touch* touch, Event* event);
    void touchMoved(Touch* touch, Event* event);
    void touchEnded(Touch* touch, Event* event);
    bool onContactBegin(PhysicsContact& contact);
    bool onContactPostSolve(PhysicsContact& contact);
};

#endif /* GameScene_hpp */
