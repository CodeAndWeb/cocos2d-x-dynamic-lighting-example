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

#include "Effect.h"

USING_NS_CC;


bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _fragSource = fragSource;
#endif
    
    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _glprogramstate->retain();
    
    return _glprogramstate != nullptr;
}

Effect::Effect()
: _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = _glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

Effect::~Effect()
{
    CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

