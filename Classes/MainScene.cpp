//
//  MainScene.cpp
//  MyGame-mobile
//
//  Created by Cyril.t on 2022/3/3.
//

#include "MainScene.hpp"
#include "ui/CocosGUI.h"
#include "GameScene.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    log("visibleSize: %lf %lf", visibleSize.width, visibleSize.height);
    log("origin: %lf %lf", origin.x, origin.y);

    auto backgroud = ImageView::create("shouyebg.png");
    backgroud->setAnchorPoint(Vec2(0, 0));
    backgroud->setContentSize(visibleSize);
    backgroud->ignoreContentAdaptWithSize(false);
    this->addChild(backgroud);

    auto rule = ImageView::create("gamesuomin.png");
    rule->setPosition(Vec2(visibleSize.width/2.0, visibleSize.height/2.0+50));
    rule->setContentSize(Size(300, 230));
    rule->ignoreContentAdaptWithSize(false);
    this->addChild(rule);

    int score = UserDefault::getInstance()->getIntegerForKey("score");
    if (score > 0) {
        auto lable = Label::createWithSystemFont("历史最高分:"+to_string(score), "Helvetica", 28);
        lable->setColor(Color3B(255, 69, 0));
        lable->setPosition(Vec2(visibleSize.width/2.0, visibleSize.height-200));
        this->addChild(lable);
    }

    auto button = Button::create("yxoneann.png");
    button->setAnchorPoint(Vec2(0.5, 1));
    button->setPosition(Vec2(visibleSize.width/2.0, rule->getPositionY()-rule->getContentSize().height/2.0-60));
    button->setContentSize(Size(160, 64));
    button->ignoreContentAdaptWithSize(false);
    button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
//                Director::getInstance()->replaceScene(TransitionFlipX::create(0.5, GameScene::createScene()));
                Director::getInstance()->replaceScene(GameScene::createScene());
                break;
            default:
                break;
        }
    });
    this->addChild(button);

    return true;
}
