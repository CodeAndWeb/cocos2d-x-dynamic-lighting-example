#include "HelloWorldScene.h"
#include "EffectSprite/LightEffect.h"
#include "EffectSprite/EffectSprite.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{
    if (!Layer::init())
        return false;
    
    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("spritesheet.plist");

    Vector<SpriteFrame*> animFrames;
    char str[100];
    for(int i = 1; i <= 16; i++)
    {
        sprintf(str, "capguy/%04d.png", i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    
    auto sprite = EffectSprite::createWithSpriteFrame(animFrames.front());
    
    Animation *animation = Animation::createWithSpriteFrames(animFrames, 1.0f/15);
    sprite->runAction(RepeatForever::create(Animate::create(animation)));
    sprite->setPosition(Director::getInstance()->getWinSize() / 2);
    
    addChild(sprite);
    
    _effect = LightEffect::create();
    _effect->retain();
    
    _lightPos = Vec3(200, 200, 100);
    _effect->setLightPos(_lightPos);
    _effect->setLightCutoffRadius(1000);

    sprite->setEffect(_effect, "spritesheet_n.png");

    _lightSprite = Sprite::create("lightbulb.png");
    _lightSprite->setPosition(_lightPos.x, _lightPos.y);
    this->addChild(_lightSprite);
    
    auto listerner = EventListenerTouchAllAtOnce::create();
    listerner->onTouchesBegan = CC_CALLBACK_2(HelloWorld::handleTouches, this);
    listerner->onTouchesMoved = CC_CALLBACK_2(HelloWorld::handleTouches, this);
    listerner->onTouchesEnded = CC_CALLBACK_2(HelloWorld::handleTouches, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listerner, this);
    
    return true;
}

void HelloWorld::handleTouches(const std::vector<Touch *> &touches, cocos2d::Event *)
{
    for (auto &touch: touches)
    {
        Point pos = touch->getLocation();
        _lightSprite->setPosition(pos);
        _lightPos.set(pos.x, pos.y, _lightPos.z);
        _effect->setLightPos(_lightPos);
    }
}