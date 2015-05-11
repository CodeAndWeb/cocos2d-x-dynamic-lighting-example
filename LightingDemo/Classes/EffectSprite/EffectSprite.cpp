//
#include "EffectSprite.h"
#include "Effect.h"


EffectSprite *EffectSprite::create(const std::string& filename)
{
    auto ret = new (std::nothrow) EffectSprite;
    if(ret && ret->initWithFile(filename)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}


EffectSprite *EffectSprite::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    auto ret = new (std::nothrow) EffectSprite;
    if(ret && ret->initWithSpriteFrame(spriteFrame)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
    
}


void EffectSprite::setEffect(Effect *effect)
{
    if(_effect != effect) {
        
        CC_SAFE_RELEASE(_effect);
        _effect = effect;
        CC_SAFE_RETAIN(_effect);
        
        _effect->setSprite(this);
        setGLProgramState(_effect->getGLProgramState());
    }
}

void EffectSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    if (_effect != nullptr)
    {
        _effect->prepareForRender();
    }
    Sprite::draw(renderer, transform, flags);
    
}

EffectSprite::~EffectSprite()
{
    CC_SAFE_RELEASE(_effect);
}