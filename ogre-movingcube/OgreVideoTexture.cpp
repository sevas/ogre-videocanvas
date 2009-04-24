#include "OgreVideoTexture.h"

#include <boost/format.hpp>

//------------------------------------------------------------------------------
OgreVideoTexture::OgreVideoTexture(const Ogre::String _filename)
    :mVideoFileName(_filename)
    ,mVideoStream(NULL)
    ,mCurrentVideoFrame(NULL)
{
    mLog = Ogre::LogManager::getSingletonPtr()->createLog("OgreVideoTexture.log");
    _init();
}
//------------------------------------------------------------------------------
OgreVideoTexture::~OgreVideoTexture(void)
{
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_init()
{
    mLog->logMessage("init");
    
    // OpenCv crap
    Ogre::String fullName = Ogre::String("../media/videos/") + mVideoFileName;
    mVideoStream = cvCreateFileCapture(fullName.c_str());

    mLog->logMessage("openned " + fullName);

    _createTextureFromCapture(mVideoStream);

    // set first frame
    cvGrabFrame(mVideoStream);
    cvGrabFrame(mVideoStream);
    mCurrentVideoFrame = cvRetrieveFrame(mVideoStream);

    _updateTextureFromImage(mCurrentVideoFrame);

    mMaterialName = "Video Texture "+ mVideoFileName;
    mTimeSinceLastUpdate.reset();

    mFrameCount = cvGetCaptureProperty(mVideoStream, CV_CAP_PROP_FRAME_COUNT);
    mCurrentFrameIndex = 2;

    mLog->logMessage("init done");
}
//------------------------------------------------------------------------------
void OgreVideoTexture::nextFrame()
{
    if (mTimeSinceLastUpdate.getMilliseconds() > 37 &&
        mCurrentFrameIndex < mFrameCount) 
    {
        cvGrabFrame(mVideoStream);
        mCurrentVideoFrame = cvRetrieveFrame(mVideoStream);
        _updateTextureFromImage(mCurrentVideoFrame);
        mTimeSinceLastUpdate.reset();
        mCurrentFrameIndex++;
    }
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_createTextureFromCapture(CvCapture *_capture)
{

    int w, h;
    w = cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH);
    h = cvGetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT);


    // Create the texture
    mVideoTexture = Ogre::TextureManager::getSingleton().createManual(
        "DynamicTexture", // name
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Ogre::TEX_TYPE_2D,      // type
        1024, 1024,         // width & height
        0,                // number of mipmaps
        Ogre::PF_BYTE_RGBA,
        Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);     

    _initTexture(mVideoTexture);

    // Create a material using the texture
    mVideoMaterial = Ogre::MaterialManager::getSingleton().create(
        mMaterialName, // name
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    mVideoMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("DynamicTexture");
    mVideoMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

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

    // Fill in some pixel data. This will give a semi-transparent blue,
    // but this is of course dependent on the chosen pixel format.
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

        // Unlock the pixel buffer
        pixelBuffer->unlock();
}
//------------------------------------------------------------------------------
void OgreVideoTexture::_updateTextureFromImage(const IplImage *_image)
{
    mTimer.reset();
    // Get the pixel buffer
    Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mVideoTexture->getBuffer();

    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
    const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
    
    Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

    // Fill in some pixel data. This will give a semi-transparent blue,
    // but this is of course dependent on the chosen pixel format.
    for (size_t j = 0; j < _image->height; j++)
        for(size_t i = 0; i < _image->width ; i++)
        {
            char pixelR = CV_IMAGE_ELEM(_image, char, j, i*3+2 );
            char pixelG = CV_IMAGE_ELEM(_image, char, j, i*3+1);
            char pixelB = CV_IMAGE_ELEM(_image, char, j, i*3 );

            int w = mVideoTexture->getWidth();

            pDest[j*w*4 + i*4]   = pixelB;
            pDest[j*w*4 + i*4+1] = pixelG;
            pDest[j*w*4 + i*4+2] = pixelR;
            pDest[j*w*4 + i*4+3] = 255;
        }

    // Unlock the pixel buffer
    pixelBuffer->unlock();

    boost::format fmt("%1% : %2% µs");
    
    fmt % "write to texture" % mTimer.getMicroseconds();
    
    mLog->logMessage(fmt.str());
}