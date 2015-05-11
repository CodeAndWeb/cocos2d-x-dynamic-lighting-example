#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class LightEffect;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

public:
    void handleTouches(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused);

protected:
    cocos2d::Vec3 _lightPos;
    LightEffect *_effect;
    cocos2d::Sprite *_lightSprite;
};

#endif // __HELLOWORLD_SCENE_H__
