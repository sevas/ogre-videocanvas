

#include <map>
#include <boost/foreach.hpp>

#include "OgreVideoCanvas.h"
//#include "OgreVideoTexture.h"
#include "ExampleApplication.h"




class TutorialFrameListener : public ExampleFrameListener
{
public:
    TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr
                          , SceneNode *canvasNode
                          , OgreVideoCanvas *_videoCanvas
                          , OgreVideoCanvas *_videoCanvas2)
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
        mVideoCanvas = _videoCanvas;
        mVideoCanvas2 = _videoCanvas2;
	}


	bool frameStarted(const FrameEvent &evt)
	{
        //mCanvasNode->roll(Degree(1.0) * evt.timeSinceLastFrame * 100);
        mVideoCanvas->nextFrame();
        mVideoCanvas2->nextFrame();

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
    OgreVideoCanvas *mVideoCanvas, *mVideoCanvas2;
	
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

		Light *light = mSceneMgr->createLight("Light1");
		light->setType(Light::LT_POINT);
		light->setPosition(Vector3(0, 300, 600));
		light->setDiffuseColour(ColourValue::White);
		light->setSpecularColour(ColourValue::White);

		// Create the scene node
		SceneNode *yawnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("YawCamNode1");
        Ogre::SceneNode *node = yawnode->createChildSceneNode("CamNode1", Vector3(0, 100, 1000));
		node->attachObject(mCamera);


        mCanvasNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Canvas Node", Vector3(0, 100, 0));       
        mVideoCanvas = new OgreVideoCanvas("../media/videos/liege.avi", mSceneMgr);
        mVideoCanvas->buildCanvas(mCanvasNode);


        mCanvasNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("Canvas Node2", Vector3(700, 100, 0));       
        mVideoCanvas2 = new OgreVideoCanvas("../media/videos/indochine.avi", mSceneMgr);
        mVideoCanvas2->buildCanvas(mCanvasNode2);
	}



	void createFrameListener(void)
	{
		// Create the FrameListener
		mFrameListener = 
            new TutorialFrameListener(mWindow, mCamera, mSceneMgr
                                    , mCanvasNode, mVideoCanvas, mVideoCanvas2);
		mRoot->addFrameListener(mFrameListener);
	}

protected:
    SceneNode *mCanvasNode, *mCanvasNode2;
    //typedef VideoCanvasItem std::pair<String, OgreVideoCanvas*>;
    //std::map<String, OgreVideoCanvas*> mVideoCanvases;
    OgreVideoCanvas *mVideoCanvas;
    OgreVideoCanvas *mVideoCanvas2;
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
