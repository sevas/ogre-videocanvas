#pragma once

#include <Ogre.h>
#include "OgreVideoTexture.h"

using namespace Ogre;

class OgreVideoCanvas
{
public:
    OgreVideoCanvas(const String&, SceneManager*);
    ~OgreVideoCanvas(void);

    void buildCanvas(SceneNode*);
    void nextFrame();

protected:
    void _initVideoTexture(){};

protected:
    SceneManager *mSceneMgr;
    String mVideoFileName;
    SceneNode *mCanvasNode;
    ManualObject *mCanvas;
    OgreVideoTexture *mVideoTexture;
    int mWidth, mHeight, mTextureSize;

};
