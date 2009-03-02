#include "ExampleApplication.h"
//#include <boost/interprocess/ipc/message_queue.hpp>

class TutorialFrameListener : public ExampleFrameListener
{
public:
	TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr)
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
		mMove = 250;

		
		mq = new boost::interprocess::message_queue(boost::interprocess::open_only, "message_queue");
	}

	// Overriding the default processUnbufferedKeyInput so the key updates we define
	// later on work as intended.
	bool processUnbufferedKeyInput(const FrameEvent& evt)
	{
		return true;
	}

	// Overriding the default processUnbufferedMouseInput so the Mouse updates we define
	// later on work as intended. 
	bool processUnbufferedMouseInput(const FrameEvent& evt)
	{
		return true;
	}

	bool frameStarted(const FrameEvent &evt)
	{
		//mCamNode->getParentSceneNode()->yaw(Degree(1.0), SceneNode::TS_PARENT);
		processMQEvents();

		mMouse->capture();
		mKeyboard->capture();

		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
			return false;

		return true;
	}


	//void processMQEvents()
	//{
	//	unsigned int priority;
	//	std::size_t recvd_size;

	//	int num = mq->get_num_msg();
	//	
	//	int number;
	//	if(num > 0)
	//	{
	//		//for(int i = 0; i < 10; ++i)
	//		//{
	//		//	int number;
	//		//	//number = buffer[i]
	//		//	//mq->receive(&number, sizeof(number), recvd_size, priority);
	//		//}
	//		mq->receive(&number, sizeof(number), recvd_size, priority);
	//		mCamNode->getParentSceneNode()->yaw(Degree(float(number)), SceneNode::TS_PARENT);
	//	}
	//	else
	//	{
	//		; //mCamNode->translate(Vector3(0, 10, 0));
	//	}
	}

protected:
	bool mMouseDown;       // Whether or not the left mouse button was down last frame
	Real mToggle;          // The time left until next toggle
	Real mRotate;          // The rotate constant
	Real mMove;            // The movement constant
	SceneManager *mSceneMgr;   // The current SceneManager
	SceneNode *mCamNode;   // The SceneNode the camera is currently attached to

	boost::interprocess::message_queue *mq;
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

		Entity *ent = mSceneMgr->createEntity("Ninja", "cube.mesh");
		SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode", Vector3(0, 100, 0));
		node->attachObject(ent);

		Light *light = mSceneMgr->createLight("Light1");
		light->setType(Light::LT_POINT);
		light->setPosition(Vector3(250, 150, 250));
		light->setDiffuseColour(ColourValue::White);
		light->setSpecularColour(ColourValue::White);

		// Create the scene node
		SceneNode *yawnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("YawCamNode1");
		node = yawnode->createChildSceneNode("CamNode1", Vector3(-400, 200, 400));
		node->yaw(Degree(-45));
		node->attachObject(mCamera);

		// create the second camera node
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Vector3(0, 200, 400));
	}

	void createFrameListener(void)
	{
		// Create the FrameListener
		mFrameListener = new TutorialFrameListener(mWindow, mCamera, mSceneMgr);
		mRoot->addFrameListener(mFrameListener);
	}
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