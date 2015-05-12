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

#include "LightEffect.h"
#include "EffectSprite.h"

USING_NS_CC;


LightEffect* LightEffect::create(const std::string&normalMapFileName)
{
    LightEffect *normalMappedSprite = new (std::nothrow) LightEffect();
    if (normalMappedSprite && normalMappedSprite->init() && normalMappedSprite->initNormalMap(normalMapFileName))
    {
        normalMappedSprite->autorelease();
        return normalMappedSprite;
    }
    CC_SAFE_DELETE(normalMappedSprite);
    return nullptr;
}

bool LightEffect::init()
{
    if (initGLProgramState("pointlight.frag"))
    {
        setLightColor(cocos2d::Color3B::WHITE);
        setAmbientLightColor(cocos2d::Color3B(127,127,127));
        setLightCutoffRadius(500.0f);
        setLightHalfRadius(0.5f);
        return true;
    }
    return false;
}

bool LightEffect::initNormalMap(const std::string& normalMapFileName)
{
    auto normalMapTexture = Director::getInstance()->getTextureCache()->addImage(normalMapFileName);
    getGLProgramState()->setUniformTexture("u_normals", normalMapTexture);
    return normalMapTexture != nullptr;
}

void LightEffect::setSprite(Sprite *sprite)
{
    _sprite = sprite;
    Point posRelToSprite = PointApplyAffineTransform(Point(_lightPos.x, _lightPos.y), _sprite->getWorldToNodeAffineTransform());
    getGLProgramState()->setUniformVec3("u_lightPos", Vec3(posRelToSprite.x, posRelToSprite.y, _lightPos.z));
    prepareForRender();
}

void LightEffect::setLightPos(const Vec3& pos)
{
    _lightPos = pos;
    if (_sprite != nullptr)
    {
        Point posRelToSprite = PointApplyAffineTransform(Point(_lightPos.x, _lightPos.y), _sprite->getWorldToNodeAffineTransform());
        getGLProgramState()->setUniformVec3("u_lightPos", Vec3(posRelToSprite.x, posRelToSprite.y, _lightPos.z));
    }
}

void LightEffect::setLightColor(const Color3B &color)
{
    _lightColor = color;
    getGLProgramState()->setUniformVec3("u_lightColor", Vec3(_lightColor.r,_lightColor.g,_lightColor.b)/255.0f);
}

void LightEffect::setAmbientLightColor(const Color3B &color)
{
    _ambientLightColor = color;
    getGLProgramState()->setUniformVec3("u_ambientColor", Vec3(_ambientLightColor.r,_ambientLightColor.g,_ambientLightColor.b)/255.0f);
}

void LightEffect::setLightCutoffRadius(float value)
{
    _lightCutoffRadius = std::max(1.0f, value);
    getGLProgramState()->setUniformFloat("u_cutoffRadius", _lightCutoffRadius);
}

void LightEffect::setLightHalfRadius(float value)
{
    _lightHalfRadius = std::max(0.01f, std::min(0.99f, value));
    getGLProgramState()->setUniformFloat("u_halfRadius", _lightHalfRadius);
}

void LightEffect::prepareForRender()
{
    if (_sprite)
    {
        auto gl = getGLProgramState();
        gl->setUniformVec2("u_contentSize", _sprite->getContentSize());
        
        SpriteFrame *frame = _sprite->getSpriteFrame();
        Size untrimmedSize = frame->getOriginalSize();
        Size trimmedSize = frame->getRect().size;
        Vec2 framePos = frame->getRect().origin;
        Size texSize = frame->getTexture()->getContentSize();

        // set sprite position in sheet
        gl->setUniformVec2("u_spritePosInSheet", Vec2(framePos.x / texSize.width, framePos.y / texSize.height));
        gl->setUniformVec2("u_spriteSizeRelToSheet", Vec2(untrimmedSize.width / texSize.width, untrimmedSize.height / texSize.height));
        gl->setUniformInt("u_spriteRotated", frame->isRotated());
       
        // set offset of trimmed sprite
        Vec2 bottomLeft = frame->getOffset() + (untrimmedSize - trimmedSize) / 2;
        Vec2 cornerOffset = frame->isRotated() ? Vec2(bottomLeft.y, bottomLeft.x)
                                               : Vec2(bottomLeft.x, untrimmedSize.height - trimmedSize.height - bottomLeft.y);
        
        gl->setUniformVec2("u_spriteOffset", cornerOffset);
    }
}
