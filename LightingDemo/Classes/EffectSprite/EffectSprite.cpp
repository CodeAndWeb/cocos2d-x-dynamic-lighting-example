/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

/*
 * based on cocos2d-x-3.6/tests/cpp-tests/Classes/ShaderTest/ShaderTest2.cpp
 * extended by CodeAndWeb GmbH https://www.codeandweb.com
 */

#include "EffectSprite.h"
#include "LightEffect.h"


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


void EffectSprite::setEffect(LightEffect *effect, const std::string &normalMapFilename)
{
    _normalmap = cocos2d::Director::getInstance()->getTextureCache()->addImage(normalMapFilename);
    
    if(_effect != effect) {
        
        CC_SAFE_RELEASE(_effect);
        _effect = effect;
        CC_SAFE_RETAIN(_effect);
        
        setGLProgramState(_effect->getGLProgramState());
    }
}

void EffectSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    if (_effect != nullptr)
    {
        _effect->prepareForRender(this, _normalmap);
    }
    Sprite::draw(renderer, transform, flags);
    renderer->render();
    
}

EffectSprite::~EffectSprite()
{
    CC_SAFE_RELEASE(_effect);
}