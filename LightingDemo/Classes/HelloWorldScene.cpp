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
    
    _screenW = Director::getInstance()->getWinSize().width;
    _screenH = Director::getInstance()->getWinSize().height;
    _scale = _screenW / 1920;
    
    _effect = LightEffect::create();
    _effect->retain();

    _lightPos = Vec3(_screenW-100, _screenH-100, 100);
    _effect->setLightPos(_lightPos);
    _effect->setLightCutoffRadius(1000);
    _effect->setBrightness(2.0);

    initBackground();
    
    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("spritesheet.plist");

    Vector<SpriteFrame*> animFrames;
    char str[100];
    for(int i = 1; i <= 8; i++)
    {
        sprintf(str, "character/%02d.png", i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    
    auto sprite = EffectSprite::createWithSpriteFrame(animFrames.front());
    
    Animation *animation = Animation::createWithSpriteFrames(animFrames, 1.0f/8);
    sprite->runAction(RepeatForever::create(Animate::create(animation)));
    sprite->setPosition(_screenW / 2.0, _screenH / 2.0 - 75.0 * _scale);
    sprite->setEffect(_effect, "spritesheet_n.png");
    sprite->setScale(_scale);
    
    addChild(sprite);

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


void HelloWorld::initBackground()
{
    addBackgroundTile("res/background_01.png", 0, 100);
    addBackgroundTile("res/background_01.png", 1920, 100);
    addBackgroundTile("res/foreground_01.png", 0, 200, "res/foreground_01_n.png");
    addBackgroundTile("res/foreground_02.png", 1920, 200, "res/foreground_02_n.png");
}


EffectSprite *HelloWorld::addBackgroundTile(const std::string &spriteFile, float offsetX, float speed, const std::string &normalsFile)
{
    auto background = EffectSprite::create(spriteFile);
    if (!normalsFile.empty())
    {
        background->setEffect(_effect, normalsFile);
    }
    else
    {
        background->setColor(_effect->getAmbientLightColor());
    }

    offsetX *= _scale;
    float offsetY = (_screenH - background->getContentSize().height * _scale) / 2.0f;

    background->setAnchorPoint(Vec2(0,0));
    background->setScale(_scale);
    addChild(background);
    
    auto a1 = MoveTo::create(0, Vec2(offsetX, offsetY));
    auto a2 = MoveTo::create((_screenW + offsetX) / speed, Vec2(-_screenW, offsetY));
    auto a3 = MoveTo::create(0, Vec2(_screenW, offsetY));
    auto a4 = MoveTo::create((_screenW - offsetX) / speed, Vec2(offsetX, offsetY));
    background->runAction(RepeatForever::create(Sequence::create(a1, a2, a3, a4, nullptr)));
    
    return background;
}