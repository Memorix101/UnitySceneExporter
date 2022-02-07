
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>

#include <IrrAssimp/IrrAssimp.h> // https://github.com/JLouis-B/IrrAssimp
#include <irrlicht.h>
#include "driverChoice.h"

#ifdef _IRR_WINDOWS_
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
To receive events like mouse and keyboard input, or GUI events like "the OK
button has been clicked", we need an object which is derived from the
irr::IEventReceiver object. There is only one method to override:
irr::IEventReceiver::OnEvent(). This method will be called by the engine once
when an event happens. What we really want to know is whether a key is being
held down, and so we will remember the current state of each key.
*/
class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

vector3df cam_pos;
vector3df cam_rot;
IAnimatedMesh* mesh[100];
IAnimatedMeshSceneNode* node[100];
IrrlichtDevice* device = nullptr;
ISceneManager* smgr = nullptr;
IVideoDriver* driver = nullptr;

void load_level(std::string file)
{
	std::fstream infile(file);
	std::string line;
	int lineCount = 0;
	bool isLVL = false;

	int mesh_index = 0;

	while (std::getline(infile, line)) {
		if (line == "lvl1") {
			isLVL = true;
		}

		if (line[0] == '#') {
			continue;
		}

		// i guess this would have been much simpler and shorter 
		// https://stackoverflow.com/a/14266139

		if (lineCount > 0 && isLVL) {
			//std::cout << line << std::endl;

			// parsePos
			std::string model, name, tag; //0, 1, 2
			float x, y, z; //3,4,5
			float rx, ry, rz, rw; //6,7,8
			float sx, sy, sz; //9,10,11
			int parsePos = 0;
			std::string tmp;
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ',') {
					parsePos++;
					tmp.clear();
					continue;
				}
				switch (parsePos) {
				case 0:
					model += line[i];
					break;
				case 1:
					name += line[i];
					break;
				case 2:
					tag += line[i];
					break;
				case 3:
					tmp += line[i];
					x = atof(tmp.c_str());
					break;
				case 4:
					tmp += line[i];
					y = atof(tmp.c_str());
					break;
				case 5:
					tmp += line[i];
					z = atof(tmp.c_str());
					break;
				case 6:
					tmp += line[i];
					rx = atof(tmp.c_str());
					break;
				case 7:
					tmp += line[i];
					ry = atof(tmp.c_str());
					break;
				case 8:
					tmp += line[i];
					rz = atof(tmp.c_str());
					break;
				case 9:
					tmp += line[i];
					rw = atof(tmp.c_str());
					break;
				case 10:
					tmp += line[i];
					sx = atof(tmp.c_str());
					break;
				case 11:
					tmp += line[i];
					sy = atof(tmp.c_str());
					break;
				case 12:
					tmp += line[i];
					sz = atof(tmp.c_str());
					break;
				}
			}

			if(tag == "MainCamera")
			{
				cam_pos = vector3df(x,y,z);
				cam_rot = vector3df(rx, ry, rz);
				continue;
			}

			if (model == "null")
				continue;

			std::string tmpx = "assets/" + model;
			IrrAssimp assimp(smgr); 
			//mesh[mesh_index] = assimp.getMesh(tmpx.c_str());
			mesh[mesh_index] = smgr->getMesh(tmpx.c_str());
			smgr->addExternalMeshLoader(new IrrAssimpImport(smgr));
			if (!mesh)
			{
				device->drop();
			}

			node[mesh_index] = smgr->addAnimatedMeshSceneNode(mesh[mesh_index]);
			if (node[mesh_index])
			{
				core::vector3df _rot;
				core::quaternion _qrot(rx, ry, rz, rw);
				_qrot.toEuler(_rot);
				node[mesh_index]->setPosition(core::vector3df(x, y, z));
				node[mesh_index]->setRotation(_rot/PI*180);
				node[mesh_index]->setScale(core::vector3df(sx, sy, sz));
				node[mesh_index]->setMaterialType(E_MATERIAL_TYPE::EMT_LIGHTMAP_LIGHTING);
				node[mesh_index]->setMaterialTexture(0, driver->getTexture("assets/texture_08.png"));
				node[mesh_index]->setMaterialFlag(video::EMF_LIGHTING, false);
			}

			mesh_index++;

			std::cout << "OUTPUT:\n";
			std::cout << " model: " << model << "\n name: " << name << "\n tag: " << tag << std::endl;
			std::cout << " pos: " << x << " " << y << " " << z << std::endl;
			std::cout << " rot: " << rx << " " << ry << " " << rz  << " " << rw << std::endl;
			std::cout << " scale: " << sx << " " << sy << " " << sz << std::endl;
		}
		lineCount++;
	}
}


/*
The event receiver for keeping the pressed keys is ready, the actual responses
will be made inside the render loop, right before drawing the scene. So lets
just create an irr::IrrlichtDevice and the scene node we want to move. We also
create some other additional scene nodes, to show that there are also some
different possibilities to move and animate scene nodes.
*/
int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType = video::E_DRIVER_TYPE::EDT_OPENGL; //driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device
	MyEventReceiver receiver;

	device = createDevice(driverType,
			core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	load_level("SampleScene.lvl");

	/*
	Create the node which will be moved with the WSAD keys. We create a
	sphere node, which is a built-in geometry primitive. We place the node
	at (0,0,30) and assign a texture to it to let it look a little bit more
	interesting. Because we have no dynamic lights in this scene we disable
	lighting for each model (otherwise the models would be black).
	*/

	/*IAnimatedMesh* mesh = smgr->getMesh("assets/cube.obj");
	if (!mesh)
	{
		device->drop();
		return 1;
	}

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
	{
		node->setPosition(core::vector3df(0,0,30));
		node->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}*/

	/*
	Now we create another node, movable using a scene node animator. Scene
	node animators modify scene nodes and can be attached to any scene node
	like mesh scene nodes, billboards, lights and even camera scene nodes.
	Scene node animators are not only able to modify the position of a
	scene node, they can also animate the textures of an object for
	example. We create a cube scene node and attach a 'fly circle' scene
	node animator to it, letting this node fly around our sphere scene node.
	*/
	/*scene::ISceneNode* n = smgr->addCubeSceneNode();
	if (n)
	{
		n->setMaterialTexture(0, driver->getTexture("../../media/t351sml.jpg"));
		n->setMaterialFlag(video::EMF_LIGHTING, false);
		scene::ISceneNodeAnimator* anim =
			smgr->createFlyCircleAnimator(core::vector3df(0,0,30), 20.0f);
		if (anim)
		{
			n->addAnimator(anim);
			anim->drop();
		}
	}*/

	/*
	The last scene node we add to show possibilities of scene node animators is
	a b3d model, which uses a 'fly straight' animator to run between to points.
	*/
	/*scene::IAnimatedMeshSceneNode* anms =
		smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/ninja.b3d"));

	if (anms)
	{
		scene::ISceneNodeAnimator* anim =
			smgr->createFlyStraightAnimator(core::vector3df(100,0,60),
			core::vector3df(-100,0,60), 3500, true);
		if (anim)
		{
			anms->addAnimator(anim);
			anim->drop();
		}

		/*
		To make the model look right we disable lighting, set the
		frames between which the animation should loop, rotate the
		model around 180 degrees, and adjust the animation speed and
		the texture. To set the right animation (frames and speed), we
		would also be able to just call
		"anms->setMD2Animation(scene::EMAT_RUN)" for the 'run'
		animation instead of "setFrameLoop" and "setAnimationSpeed",
		but this only works with MD2 animations, and so you know how to
		start other animations. But a good advice is to not use
		hardcoded frame-numbers...
		*/
		/*anms->setMaterialFlag(video::EMF_LIGHTING, false);

		anms->setFrameLoop(0, 13);
		anms->setAnimationSpeed(15);
//		anms->setMD2Animation(scene::EMAT_RUN);

		anms->setScale(core::vector3df(2.f,2.f,2.f));
		anms->setRotation(core::vector3df(0,-90,0));
//		anms->setMaterialTexture(0, driver->getTexture("../../media/sydney.bmp"));

	}*/

	/*
	To be able to look at and move around in this scene, we create a first
	person shooter style camera and make the mouse cursor invisible.
	*/
	smgr->addCameraSceneNodeFPS();
	smgr->addCameraSceneNode(0, cam_pos, cam_rot);
	device->getCursorControl()->setVisible(false);

	/*
	Add a colorful irrlicht logo
	*/
	/*device->getGUIEnvironment()->addImage(driver->getTexture("../../media/irrlichtlogoalpha2.tga"), core::position2d<s32>(10, 20));
	gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 400, 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));*/

	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the name of the driver to the caption of the
	window.
	*/
	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 5.f;

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		/* Check if keys W, S, A or D are being held down, and move the
		sphere node around respectively. */
		/*core::vector3df nodePosition = node->getPosition();

		if(receiver.IsKeyDown(irr::KEY_KEY_W))
			nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_S))
			nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

		if(receiver.IsKeyDown(irr::KEY_KEY_A))
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_D))
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

		node->setPosition(nodePosition);*/

		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Custom Level Loader - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	device->drop();
	
	return 0;
}