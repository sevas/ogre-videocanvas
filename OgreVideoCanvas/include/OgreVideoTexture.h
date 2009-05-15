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
    void resetVideo();
    void setDepthCheckEnabled(bool);

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

    void _logMessage(const Ogre::String&);

protected:
    Ogre::String mVideoFileName, mVideoBaseName;
    Ogre::String mTextureName, mMaterialName;
    Ogre::TexturePtr mVideoTexture;
    Ogre::MaterialPtr mVideoMaterial;
    Ogre::Timer mTimeSinceLastUpdate;

    CvCapture *mCvCapture;
    IplImage *mCurrentVideoFrame;

    int mFrameCount, mCurrentFrameIndex;
    Ogre::Timer mTimer;
    Ogre::Log *mLog;
    bool mIsLoggingEvents;
};
