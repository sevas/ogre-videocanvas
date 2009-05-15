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
    void resetVideo();
    void setVideoTexture(OgreVideoTexture*);
    void unsetVideoTexture();
    void setRenderQueueGroup(Ogre::uint8);


protected:
    SceneManager *mSceneMgr;
    String mName;
    String mVideoFileName, mVideoBaseName;
    SceneNode *mCanvasNode;
    ManualObject *mCanvas;
    OgreVideoTexture *mVideoTexture;
    int mWidth, mHeight, mTextureSize;

};
