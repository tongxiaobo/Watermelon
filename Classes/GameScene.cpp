//
//  GameScene.cpp
//  Watermelon-mobile
//
//  Created by Cyril.t on 2022/3/7.
//

#include "GameScene.hpp"
#include "SimpleAudioEngine.h"
#include <array>

using namespace std;
using namespace CocosDenshion;
using namespace std;

static std::array<float, 11> fruitSizes = {45, 55, 60, 70, 90, 120, 150, 170, 180, 190, 200};

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setGravity(Vec2(0.f, -98.f));
    scene->getPhysicsWorld()->setSpeed(6);

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backgroud = ImageView::create("youxibeij.png");
    backgroud->setPosition(Vec2(visibleSize.width/2.0, visibleSize.height/2.0));
    backgroud->setContentSize(visibleSize);
    backgroud->ignoreContentAdaptWithSize(false);
    this->addChild(backgroud);

    auto topLine = Sprite::create("toplineimg.png");
    topLine->setPosition(Vec2(visibleSize.width/2.0, visibleSize.height-140));
    topLine->setScale(0.8);
    this->addChild(topLine);

    ground = ImageView::create("dibuxian.png");
    ground->setAnchorPoint(Vec2(0, 0));
    ground->setContentSize(Size(visibleSize.width, 66));
    ground->ignoreContentAdaptWithSize(false);
    this->addChild(ground);

    scoreLabel = Label::createWithSystemFont("分数:0", "Helvetica-Bold", 30);
    scoreLabel->setColor(Color3B(250, 168, 85));
    scoreLabel->setPosition(Vec2(70, visibleSize.height-45));
    this->addChild(scoreLabel);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::touchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::touchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::touchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto physicsBody = PhysicsBody::createEdgeBox(visibleSize,PhysicsMaterial(0.1f, 0.f, 1.f));
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    auto node = Node::create();
    node->setPhysicsBody(physicsBody);
    node->setPosition(Vec2(visibleSize.width/2.0, visibleSize.height/2.0+66));
    this->addChild(node);

//    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setDebugDrawMask(true);
    showFruit();

    return true;
}

void GameScene::showFruit() {
    isReady = true;
    Size visibleSize = Director::getInstance()->getVisibleSize();

    srand((unsigned)time(NULL));
    int number = rand()%5;

    fruit = createFruit(number);
    fruit->setPosition(Vec2(visibleSize.width/2.0, visibleSize.height-80));
    this->addChild(fruit);
}

Sprite* GameScene::createFruit(int number) {

    std::string str = "imgball" + to_string(number+1) + ".png";

    int w = fruitSizes[number];

    auto sprite = Sprite::create(str);
    sprite->setContentSize(Size(w, w));
    sprite->setName("fruit");
    return sprite;
}

void GameScene::addPhysics(Sprite* fruit) {
    float r = fruit->getContentSize().width/2.0;
    auto physicsBody = PhysicsBody::createCircle(r, PhysicsMaterial(3.0f, 0.3f, 0.5));
    physicsBody->setDynamic(true);
    physicsBody->setName("physicsBody");
//    physicsBody->setMass(10);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);

    if (fruit->getComponent("physicsBody") == NULL) {
        fruit->addComponent(physicsBody);
    }
}

void GameScene::calculateScore(int number) {
    score = score + number + 1;
    scoreLabel->setString("分数:" + to_string(score));
}

bool GameScene::touchBegan(Touch* touch, Event* event) {
    return true;
}

void GameScene::touchMoved(Touch* touch, Event* event) {
    float d = touch->getLocation().x - touch->getPreviousLocation().x;
    float newX = fruit->getPosition().x + d;
    float fruitWith = fruit->getContentSize().width;
    float visibleWidth = Director::getInstance()->getVisibleSize().width;
    if (newX < fruitWith/2.0) {
        newX = fruitWith/2.0;
    } else if (newX > visibleWidth-fruitWith/2.0) {
        newX = visibleWidth-fruitWith/2.0;
    }
    fruit->setPosition(Vec2(newX, fruit->getPosition().y));
}

void GameScene::touchEnded(Touch* touch, Event* event) {

    addPhysics(fruit);
    isReady = false;
}

bool GameScene::onContactBegin(PhysicsContact& contact) {
    Sprite* nodeA = (Sprite *)contact.getShapeA()->getBody()->getNode();
    Sprite* nodeB = (Sprite *)contact.getShapeB()->getBody()->getNode();

    if (!isReady && nodeA == fruit) {
        showFruit();
    }

    if (nodeA && nodeB && nodeA->getName() == "fruit" && nodeB->getName() == "fruit") {
        if (nodeA->getContentSize().width == nodeB->getContentSize().width) {
            float w = nodeA->getContentSize().width;
            int i = 0;
            for (; i < fruitSizes.size(); i++) {
                if (fruitSizes[i] == w) {
                    break;
                }
            }
            if (i < fruitSizes.size()-1) {

                calculateScore(i);

                auto hitPosition = nodeA->getPosition();
                auto position = nodeB->getPosition();

                float d = (fruitSizes[i+1] - fruitSizes[i])/2.0;
                position.x = hitPosition.x < position.x ? position.x - d : position.x + d;
                position.y += d;

                nodeA->removeFromParent();
                nodeB->removeFromParent();

                Sprite* newFruit = createFruit(i+1);
                newFruit->setPosition(position);
                this->addChild(newFruit);
                newFruits.pushBack(newFruit);

                schedule([&](float dx){
                    auto fruit = newFruits.front();
                    newFruits.eraseObject(fruit);
                    addPhysics(fruit);

                }, 0, 0, 0.01, "add");
            }
        }
    }
    return true;
}
