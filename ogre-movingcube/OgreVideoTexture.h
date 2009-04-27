#pragma once


#include <cv.h>
#include <highgui.h>

#include <Ogre.h>


class OgreVideoTexture
{
public:
    OgreVideoTexture(const Ogre::String);
    ~OgreVideoTexture(void);
    Ogre::String getMaterialName() const { return mVideoMaterial->getName();};
    void nextFrame();

protected:
    void _init();
    void _initCapture();
    void _reinitCapture();
    void _endCapture();
    void _createTextureFromCapture(CvCapture*);
    void _initTexture(Ogre::TexturePtr);
    void _updateTextureFromImage(const IplImage*);
    void _destroyTexture();

   
    inline void _copyImagePerLine(const IplImage*, Ogre::HardwarePixelBufferSharedPtr);
    inline void _copyImagePerPixel(const IplImage*, Ogre::HardwarePixelBufferSharedPtr);
    inline void _copyImagePerChannel(const IplImage*, Ogre::HardwarePixelBufferSharedPtr);

protected:
    Ogre::String mVideoFileName;
    CvCapture *mCvCapture;
    IplImage *mCurrentVideoFrame;
    Ogre::TexturePtr mVideoTexture;
    Ogre::MaterialPtr mVideoMaterial;
    Ogre::String mMaterialName;
    Ogre::Timer mTimeSinceLastUpdate;
    int mFrameCount, mCurrentFrameIndex;
    Ogre::Log *mLog;
    Ogre::Timer mTimer;
};
