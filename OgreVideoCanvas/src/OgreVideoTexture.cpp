#include "OgreVideoTexture.h"

#include <boost/format.hpp>
#include <cstring>

//------------------------------------------------------------------------------
OgreVideoTexture::OgreVideoTexture(const Ogre::String _filename)
    :mVideoFileName(_filename)
    ,mCvCapture(NULL)
    ,mCurrentVideoFrame(NULL)
    ,mCurrentFrameIndex(0)
    ,mLog(NULL)
    ,mIsLoggingEvents(false)
{
    Ogre::String out;
    Ogre::StringUtil::splitFilename(mVideoFileName, mVideoBaseName, out);

    mMaterialName = "Video Material "+ mVideoBaseName;
    mTextureName = "Video Texture " + mVideoBaseName;
    if (mIsLoggingEvents)
        mLog = Ogre::LogManager::getSingletonPtr()->createLog("OgreVideoTexture_"+mVideoBaseName+".log");
    
    _init();
}
//------------------------------------------------------------------------------
OgreVideoTexture::~OgreVideoTexture(void)
{
    _endCapture();
    _destroyTexture();
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_init()
{
    _logMessage("init");
    

    _initCapture();
    _createTextureFromCapture(mCvCapture);

    // set first frame
    mCurrentVideoFrame = cvQueryFrame(mCvCapture);
    mCurrentFrameIndex++;

    _updateTextureFromImage(mCurrentVideoFrame);


    mTimeSinceLastUpdate.reset();

     
    _logMessage("init done");
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_initCapture()
{
    
    mCvCapture = cvCreateFileCapture(mVideoFileName.c_str());
    mFrameCount = cvGetCaptureProperty(mCvCapture, CV_CAP_PROP_FRAME_COUNT);

    // skip first frame
    cvGrabFrame(mCvCapture);
    mCurrentFrameIndex++;

    _logMessage("openned " + mVideoFileName);
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_endCapture()
{
    cvReleaseCapture(&mCvCapture);
    mCurrentFrameIndex = 0;
    mCurrentVideoFrame = NULL;
    mCvCapture = NULL;
    
    _logMessage("video file" + mVideoFileName + "ended");
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_reinitCapture()
{
    cvSetCaptureProperty(mCvCapture, CV_CAP_PROP_POS_MSEC, 0.0);
    
    cvGrabFrame(mCvCapture);
    mCurrentFrameIndex = 1;
}
//------------------------------------------------------------------------------
void OgreVideoTexture::nextFrame()
{
    if (mCurrentFrameIndex < mFrameCount)
    {
        if (mTimeSinceLastUpdate.getMilliseconds() > 20)
            
        {
            mCurrentVideoFrame = cvQueryFrame(mCvCapture);
            _updateTextureFromImage(mCurrentVideoFrame);
            mTimeSinceLastUpdate.reset();
            mCurrentFrameIndex++;
        }
    }
    else
    {
        _reinitCapture();
    }
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_createTextureFromCapture(CvCapture *_capture)
{

    int w, h;
    w = cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH);
    h = cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT);

    Ogre::TextureManager *TM = Ogre::TextureManager::getSingletonPtr();

    // Create the texture
    mVideoTexture = Ogre::TextureManager::getSingleton().createManual(
        mTextureName, // name
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Ogre::TEX_TYPE_2D,      // type
        1024, 1024,         // width & height
        0,                // number of mipmaps
        Ogre::PF_BYTE_BGR,
        Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

    _initTexture(mVideoTexture);

    // Create a material using the texture
    mVideoMaterial = Ogre::MaterialManager::getSingleton().create(
        mMaterialName, // name
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    mVideoMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(mTextureName);
    //mVideoMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

}
//------------------------------------------------------------------------------
void OgreVideoTexture::_destroyTexture()
{
    //TODO
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_initTexture(Ogre::TexturePtr _texture)
{
    // Get the pixel buffer
    Ogre::HardwarePixelBufferSharedPtr pixelBuffer = _texture->getBuffer();

    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
    const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

    Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

    for (size_t j = 0; j < _texture->getHeight(); j++)
        for(size_t i = 0; i < _texture->getWidth() ; i++)
        {

            if (j<480-5 && i<640-5)
            {
                *pDest++ = 255; // B
                *pDest++ = 0; // G
                *pDest++ = 255; // R
            }
            else
            {
                *pDest++ = 255; // B
                *pDest++ = 0;   // G
                *pDest++ = 0; // R
            }
        }
 
        pixelBuffer->unlock();
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_updateTextureFromImage(const IplImage *_image)
{
    if (mIsLoggingEvents)
        mTimer.reset();
    
    // Get the pixel buffer
    Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mVideoTexture->getBuffer();

    //_copyImagePerChannel(_image, pixelBuffer);
    //_copyImagePerLine(_image, pixelBuffer);
    _copyImagePerPixel(_image, pixelBuffer);

    
    if (mIsLoggingEvents)
    {        
        boost::format fmt("%1% (%2%) : %3% µs");
        
        fmt % "write to texture" 
            % mCurrentFrameIndex
            % mTimer.getMicroseconds();
        
        _logMessage(fmt.str());
    }
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_copyImagePerLine(const IplImage *_image
                                        ,Ogre::HardwarePixelBufferSharedPtr _pixelBuffer)
{

    // Lock the pixel buffer and get a pixel box
    _pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD); // for best performance use HBL_DISCARD!
    const Ogre::PixelBox& pixelBox = _pixelBuffer->getCurrentLock();

    //Ogre::uint32* pDest = static_cast<Ogre::uint32*>(pixelBox.data);
    Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

    for (size_t i = 0 ; i < _image->height ; i++)
    {
    memcpy(pDest + i*1024*4
          , (_image->imageData) +   i*_image->width * 3
          , _image->width * 3);
    }

    _pixelBuffer->unlock();
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_copyImagePerPixel(const IplImage *_image
                                         ,Ogre::HardwarePixelBufferSharedPtr _pixelBuffer)
{
    // Lock the pixel buffer and get a pixel box
    _pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD); // for best performance use HBL_DISCARD!
    const Ogre::PixelBox& pixelBox = _pixelBuffer->getCurrentLock();

    Ogre::uint32* pDest = static_cast<Ogre::uint32*>(pixelBox.data);
    
    size_t w, h, widthStep;
    w = _image->width;
    h = _image->height;
    widthStep = _image->widthStep;

    Ogre::uint32 pixelBGRA;
    for(size_t i=0 ; i < h ; i++)
    {
        size_t offset = i*widthStep;
        for (size_t j=0 ; j < w ; j++)
        {
            memcpy(&pixelBGRA, _image->imageData + offset +j*3, sizeof(Ogre::uint32));            
            pDest[i *1024 + j] = pixelBGRA;            
        }
    }
   
    _pixelBuffer->unlock();
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_copyImagePerChannel(const IplImage *_image
                                         ,Ogre::HardwarePixelBufferSharedPtr _pixelBuffer)
{

    // Lock the pixel buffer and get a pixel box
    _pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD); // for best performance use HBL_DISCARD!
    const Ogre::PixelBox& pixelBox = _pixelBuffer->getCurrentLock();

    
    Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);


    for (size_t j = 0; j < _image->height; j++)
        for(size_t i = 0; i < _image->width ; i++)
        {
            char pixelR = CV_IMAGE_ELEM(_image, char, j, i*3+2 );
            char pixelG = CV_IMAGE_ELEM(_image, char, j, i*3+1);
            char pixelB = CV_IMAGE_ELEM(_image, char, j, i*3 );

            int w = mVideoTexture->getWidth();

            pDest[j*1024*4 + i*4]   = pixelB;
            pDest[j*1024*4 + i*4+1] = pixelG;
            pDest[j*1024*4 + i*4+2] = pixelR;
            //pDest[j*w*4 + i*4+3] = 255;
        }

    _pixelBuffer->unlock();
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_logMessage(const Ogre::String &_msg)
{
    if (mIsLoggingEvents)
        mLog->logMessage(_msg);
}