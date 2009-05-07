#include "OgreVideoCanvas.h"

//------------------------------------------------------------------------------
OgreVideoCanvas::OgreVideoCanvas(const String &_filename, SceneManager *_sceneMgr)
    :mSceneMgr(_sceneMgr)
    ,mVideoFileName(_filename)
    ,mCanvas(NULL)
    ,mVideoTexture(NULL)
    ,mCanvasNode(NULL)
    ,mWidth(640)
    ,mHeight(480)
    ,mTextureSize(1024)
{
    Ogre::String out;
    Ogre::StringUtil::splitFilename(mVideoFileName, mVideoBaseName, out);
}
//------------------------------------------------------------------------------
OgreVideoCanvas::~OgreVideoCanvas(void)
{
}
//------------------------------------------------------------------------------
void OgreVideoCanvas::buildCanvas(SceneNode *_parentNode)
{
    
    float uMin = 0, vMin = 0;
    float uMax = float(mWidth)  / mTextureSize;
    float vMax = float(mHeight) / mTextureSize;

    mCanvas = mSceneMgr->createManualObject("Video Canvas " + mVideoBaseName);
    mCanvas->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_STRIP);

    mCanvas->position(-mWidth/2,  mHeight/2, 0);   mCanvas->textureCoord(uMin, vMax);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);
    mCanvas->position( mWidth/2,  mHeight/2, 0);   mCanvas->textureCoord(uMax, vMax);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);
    mCanvas->position(-mWidth/2, -mHeight/2, 0);   mCanvas->textureCoord(uMin, vMin);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);
    mCanvas->position( mWidth/2, -mHeight/2, 0);   mCanvas->textureCoord(uMax, vMin);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);

    mCanvas->end();

    mCanvasNode = _parentNode->createChildSceneNode("Canvas Node " + mVideoBaseName);
    mCanvasNode ->attachObject(mCanvas);
    mCanvasNode->yaw(Degree(180.0));
    mCanvasNode->roll(Degree(180.0));

     mCanvas->setMaterialName(0, "VideoCanvas/NoVideo");
}
//------------------------------------------------------------------------------
void OgreVideoCanvas::nextFrame()
{
    if (mVideoTexture)
        mVideoTexture->nextFrame();
}
//------------------------------------------------------------------------------
void OgreVideoCanvas::setVideoTexture(OgreVideoTexture *_videoTexture)
{
    if (_videoTexture && _videoTexture != mVideoTexture)
    {
        mVideoTexture = _videoTexture;
        mCanvas->setMaterialName(0, mVideoTexture->getMaterialName());
    }
}
//------------------------------------------------------------------------------
void OgreVideoCanvas::unsetVideoTexture()
{
    mVideoTexture = NULL;
    mCanvas->setMaterialName(0, "VideoCanvas/NoVideo");
}
//------------------------------------------------------------------------------