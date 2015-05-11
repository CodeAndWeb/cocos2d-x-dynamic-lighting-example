#ifndef _LIGHTEFFECT_H_
#define _LIGHTEFFECT_H_

#include "Effect.h"


class LightEffect : public Effect
{
public:
    static LightEffect* create(const std::string&normalMapFileName);
    
    void setLightPos(const cocos2d::Vec3& pos);
    void setLightColor(const cocos2d::Color3B& color);
    void setAmbientLightColor(const cocos2d::Color3B& color);
    void setLightCutoffRadius(float value);
    void setLightHalfRadius(float value);
    
    virtual void setSprite(cocos2d::Sprite *sprite) override;
    virtual void prepareForRender() override;

protected:
    bool init();
    bool initNormalMap(const std::string&normalMapFileName);

    cocos2d::Sprite *_sprite = nullptr;
    cocos2d::Vec3 _lightPos;
    cocos2d::Color3B _lightColor;
    cocos2d::Color3B _ambientLightColor;
    float _lightCutoffRadius;
    float _lightHalfRadius;
};

#endif
