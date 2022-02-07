
#include <iostream>
#include <string>
#include <fstream>

#include "include/raylib.h"
#include "include/raymath.h"        // Required for: MatrixRotateXYZ()


#define RLIGHTS_IMPLEMENTATION
#include "rlights.h" // https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_basic_lighting.c

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

// x86
#pragma comment(lib, "lib/raylib.lib")
#pragma comment(lib, "winmm.lib")

Vector3 cam_pos;
Vector3 cam_rot;
Model model_mesh[2];
Vector3 model_pos[2];
Vector3 model_rot[2];
Vector3 model_scale[2];

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
			float rx, ry, rz; //6,7,8
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
					sx = atof(tmp.c_str());
					break;
				case 10:
					tmp += line[i];
					sy = atof(tmp.c_str());
					break;
				case 11:
					tmp += line[i];
					sz = atof(tmp.c_str());
					break;
				}
			}

			if (tag == "MainCamera")
			{
				cam_pos = Vector3{ x, y, z };
				cam_rot = Vector3{ rx, ry, rz };
				continue;
			}

			if (model == "null")
				continue;

			std::string tmpx = "assets/" + model;
			model_mesh[mesh_index] = LoadModel(tmpx.c_str());
			Texture2D texture = LoadTexture("assets/texture_08.png");

			model_mesh[mesh_index].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
			if (!model[mesh_index])
			{
				CloseWindow();
			}
			else
			{
				model_pos[mesh_index] = Vector3{ -x, y, z };
				model_rot[mesh_index] = Vector3{ rx, ry, rz };
				model_scale[mesh_index] = Vector3{ sx, sy, sz };
			}

			mesh_index++;

			std::cout << "OUTPUT:\n";
			std::cout << " model: " << model << "\n name: " << name << "\n tag: " << tag << std::endl;
			std::cout << " pos: " << x << " " << y << " " << z << std::endl;
			std::cout << " rot: " << rx << " " << ry << " " << rz << std::endl;
			std::cout << " scale: " << sx << " " << sy << " " << sz << std::endl;
		}
		lineCount++;
	}
}

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 640;
	const int screenHeight = 480;

	//SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(screenWidth, screenHeight, "raylib - Custom Level Loader");

	load_level("SampleScene.lvl");

	Camera3D  camera = { 0 };
	camera.position = Vector3{ cam_pos };// Camera position perspective
	camera.target = Vector3{ cam_rot };      // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 60.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;           // Camera type
	SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera);          // Update camera
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLUE);

		// Draw 3D model (recomended to draw 3D always before 2D)
		BeginMode3D(camera);

		for (int m = 0; m < 2; m++)
		{
			model_mesh[m].transform = MatrixRotateXYZ(Vector3{ DEG2RAD * model_rot[m].x, DEG2RAD * model_rot[m].y, DEG2RAD * model_rot[m].z });
			//model_mesh[m].transform = MatrixTranslate(model_pos[m].x, model_pos[m].y, model_pos[m].z);
			//model_mesh[m].transform = MatrixScale(model_scale[m].x, model_scale[m].y, model_scale[m].z);
			DrawModel(model_mesh[m], model_pos[m], 1.0f, WHITE);   // Draw 3d model with texture
			//DrawModelEx(model_mesh[m], model_pos[m], model_rot[m], 0.0f, model_scale[m], WHITE);
		}

		//DrawGrid(10, 10.0f);

		EndMode3D();

		DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 20, 10, RAYWHITE);
		DrawText("Custom Level Loader Example - Memorix101", screenWidth - 240, screenHeight - 20, 10, RAYWHITE);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	for (int m = 0; m < 10; m++)
	{
		UnloadModel(model_mesh[m]);     // Unload model data
	}

	CloseWindow();          // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}