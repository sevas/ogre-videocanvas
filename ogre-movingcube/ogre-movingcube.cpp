

#include "OgreVideoTexture.h"
#include "ExampleApplication.h"




class TutorialFrameListener : public ExampleFrameListener
{
public:
    TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr, SceneNode *canvasNode, OgreVideoTexture *_videoTexture)
		: ExampleFrameListener(win, cam, false, false)
	{
		// key and mouse state tracking
		mMouseDown = false;
		mToggle = 0.0;

		// Populate the camera and scene manager containers
		mCamNode = cam->getParentSceneNode();
		mSceneMgr = sceneMgr;

		// set the rotation and move speed
		mRotate = 0.13;
		mMoveSpeed = 500;

		mCanvasNode = canvasNode;
        mVideoTexture = _videoTexture;
	}

	//// Overriding the default processUnbufferedKeyInput so the key updates we define
	//// later on work as intended.
	//bool processUnbufferedKeyInput(const FrameEvent& evt)
	//{
	//	return true;
	//}

	//// Overriding the default processUnbufferedMouseInput so the Mouse updates we define
	//// later on work as intended. 
	//bool processUnbufferedMouseInput(const FrameEvent& evt)
	//{
	//	return true;
	//}

	bool frameStarted(const FrameEvent &evt)
	{
        mCanvasNode->roll(Degree(1.0) * evt.timeSinceLastFrame * 100);

        mVideoTexture->nextFrame();


		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
			return false;

		return true;

	}

protected:
	bool mMouseDown;       // Whether or not the left mouse button was down last frame
	Real mToggle;          // The time left until next toggle
	Real mRotate;          // The rotate constant
	Real mMove;            // The movement constant
	SceneManager *mSceneMgr;   // The current SceneManager
	SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
    SceneNode *mCanvasNode;
    OgreVideoTexture *mVideoTexture;
	char *mBuffer;
};

class TutorialApplication : public ExampleApplication
{
public:
	TutorialApplication()
	{
	}

	~TutorialApplication() 
	{
	}
protected:
	void createCamera(void)
	{
		// create camera, but leave at default position
		mCamera = mSceneMgr->createCamera("PlayerCam"); 
		mCamera->setNearClipDistance(5);
	}

	void createScene(void)
	{
        
		mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

        float uMin = 0, vMin = 0;
        float uMax = 640.0/1024, vMax = 480.0/1024;

        mCanvas = mSceneMgr->createManualObject("video canvas");
        mCanvas->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_STRIP);
   
        /*mCanvas->position(-0.65,  0.5, 0);   mCanvas->textureCoord(uMin, vMax);     mCanvas->normal(Ogre::Vector3::UNIT_Y);
        mCanvas->position( 0.65,  0.5, 0);   mCanvas->textureCoord(uMax, vMax);     mCanvas->normal(Ogre::Vector3::UNIT_Y);
        mCanvas->position(-0.65, -0.5, 0);   mCanvas->textureCoord(uMin, vMin);     mCanvas->normal(Ogre::Vector3::UNIT_Y);
        mCanvas->position( 0.65, -0.5, 0);   mCanvas->textureCoord(uMax, vMin);     mCanvas->normal(Ogre::Vector3::UNIT_Y);*/
   
        mCanvas->position(-320,  240, 0);   mCanvas->textureCoord(uMin, vMax);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);
        mCanvas->position( 320,  240, 0);   mCanvas->textureCoord(uMax, vMax);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);
        mCanvas->position(-320, -240, 0);   mCanvas->textureCoord(uMin, vMin);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);
        mCanvas->position( 320, -240, 0);   mCanvas->textureCoord(uMax, vMin);     mCanvas->normal(Ogre::Vector3::NEGATIVE_UNIT_Z);

        mCanvas->end();



        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Canvas Node", Vector3(0, 100, 0));
        node->attachObject(mCanvas);
        //node->scale(100, 100, 100);
        node->yaw(Degree(180.0));
        node->roll(Degree(180.0));
        mCanvasNode = node;



		Light *light = mSceneMgr->createLight("Light1");
		light->setType(Light::LT_POINT);
		light->setPosition(Vector3(0, 300, 600));
		light->setDiffuseColour(ColourValue::White);
		light->setSpecularColour(ColourValue::White);

		// Create the scene node
		SceneNode *yawnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("YawCamNode1");
		node = yawnode->createChildSceneNode("CamNode1", Vector3(0, 100, 1000));
		node->attachObject(mCamera);


        mVideoTexture = new OgreVideoTexture("liege.avi");
        mCanvas->setMaterialName(0, mVideoTexture->getMaterialName());
        
	}






	void createFrameListener(void)
	{
		// Create the FrameListener
		mFrameListener = new TutorialFrameListener(mWindow, mCamera, mSceneMgr, mCanvasNode, mVideoTexture);
		mRoot->addFrameListener(mFrameListener);
	}

protected:
    SceneNode *mCanvasNode;
    ManualObject *mCanvas;
    OgreVideoTexture *mVideoTexture;
};

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
	// Create application object
	TutorialApplication app;

	try {
		app.go();
	} catch(Exception& e) {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		fprintf(stderr, "An exception has occurred: %s\n",
			e.getFullDescription().c_str());
#endif
	}

	return 0;
}