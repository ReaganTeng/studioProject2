#include "mini_game_3.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"
#include <math.h>

#include <sstream>

mini_game_3::mini_game_3()
{
}

mini_game_3::~mini_game_3()
{
}

void mini_game_3::innitPos()
{
	//Maze boundaries (Dont need to touch already) (ends at index 11)
	{
		//Wall boundary 1:
		mazeWallPos.push_back(427);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(432);
		//Wall boundary 2:
		mazeWallPos.push_back(427);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-432);
		//Wall boundary 3:
		mazeWallPos.push_back(-427);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-432);
		//Wall boundary 4:
		mazeWallPos.push_back(-427);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(432);
	}
	//Actual maze (Size 3 walls) (ends at index 26)
	{
		//maze wall 1:
		mazeWallPos.push_back(301);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(427);
		//maze wall 2:
		mazeWallPos.push_back(301);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(86);
		//maze wall 3:
		mazeWallPos.push_back(90);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(330);
		//maze wall 4:
		mazeWallPos.push_back(-47);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(81);
		//maze wall 5:
		mazeWallPos.push_back(426);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-161);
	}
	//Actual maze (Size 2 walls)
	{
		//maze wall 6:
		mazeWallPos.push_back(-426);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-138);
		//maze wall 7:
		mazeWallPos.push_back(-269);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-132);
		//maze wall 8:
		mazeWallPos.push_back(-269);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-132);
		//maze wall 9:
		mazeWallPos.push_back(-269);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(36);
		//maze wall 10:
		mazeWallPos.push_back(68);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-166);
		//maze wall 11:
		mazeWallPos.push_back(68);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(-336);
		//maze wall 12:
		mazeWallPos.push_back(-47);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(432);
		//maze wall 13:
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(0);
		mazeWallPos.push_back(267);
	}

	//Digit positions:
	{
		//0:
		digitPos.push_back(408);
		digitPos.push_back(20);
		digitPos.push_back(-148);
		//1:
		digitPos.push_back(275);
		digitPos.push_back(20);
		digitPos.push_back(424);
		//2:
		digitPos.push_back(117);
		digitPos.push_back(20);
		digitPos.push_back(-291);
		//3:
		digitPos.push_back(75);
		digitPos.push_back(20);
		digitPos.push_back(93);
		//4:
		digitPos.push_back(-280);
		digitPos.push_back(20);
		digitPos.push_back(-149);
		//5:
		digitPos.push_back(-59);
		digitPos.push_back(20);
		digitPos.push_back(278);
		//6:
		digitPos.push_back(140);
		digitPos.push_back(20);
		digitPos.push_back(110);
		//7:
		digitPos.push_back(71);
		digitPos.push_back(20);
		digitPos.push_back(35);
		//8:
		digitPos.push_back(-315);
		digitPos.push_back(20);
		digitPos.push_back(-95);
		//9:
		digitPos.push_back(-257);
		digitPos.push_back(20);
		digitPos.push_back(-94);
	}
	//Portal pos
	{
		portalPos.push_back(-426);
		portalPos.push_back(30);
		portalPos.push_back(0);
	}
}
void mini_game_3::innitRotate()
{
	//Maze boundary (ends at index 11)
	{
		//Wall boundary 1:
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		//Wall boundary 2:
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
		//Wall boundary 3:
		mazeWallRotate.push_back(180);
		mazeWallRotate.push_back(180);
		mazeWallRotate.push_back(180);
		//Wall boundary 4:
		mazeWallRotate.push_back(270);
		mazeWallRotate.push_back(270);
		mazeWallRotate.push_back(270);
	}
	//Actual maze (Size 3 walls) (ends at index 26)
	{
		//maze wall 1:
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		//maze wall 2:
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
		//maze wall 3:
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		//maze wall 4:
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		//maze wall 5:
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
	}
	//Actual maze (Size 2 walls)
	{
		//Maze wall 6:
		mazeWallRotate.push_back(270);
		mazeWallRotate.push_back(270);
		mazeWallRotate.push_back(270);
		//Maze wall 7:
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		//Maze wall 8:
		mazeWallRotate.push_back(180);
		mazeWallRotate.push_back(180);
		mazeWallRotate.push_back(180);
		//Maze wall 9:
		mazeWallRotate.push_back(180);
		mazeWallRotate.push_back(180);
		mazeWallRotate.push_back(180);
		//Maze wall 10:
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		//Maze wall 11:
		mazeWallRotate.push_back(270);
		mazeWallRotate.push_back(270);
		mazeWallRotate.push_back(270);
		//Maze wall 12:
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		mazeWallRotate.push_back(0);
		//Maze wall 13:
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
		mazeWallRotate.push_back(90);
	}
}
void mini_game_3::innitSize()
{
	//Maze boundary size
	for (unsigned i = 0; i < 4; i++)
	{
		mazeWallSize.push_back(60);
		mazeWallSize.push_back(60);
		mazeWallSize.push_back(300);
	}
	//Maze walls (Size 3 walls)
	for (unsigned i = 0; i < 5; i++)
	{
		mazeWallSize.push_back(60);
		mazeWallSize.push_back(60);
		mazeWallSize.push_back(120);
	}
	//Maze walls (Size 2 walls)
	for (unsigned i = 0; i < 8; i++)
	{
		mazeWallSize.push_back(60);
		mazeWallSize.push_back(60);
		mazeWallSize.push_back(60);
	}

	//Digit Sizes:
	for (unsigned i = 0; i < 10; i++)
	{
		digitSize.push_back(15);
		digitSize.push_back(15);
		digitSize.push_back(15);
	}

	//Portal sizes
	portalSize.push_back(30);
	portalSize.push_back(60);
	portalSize.push_back(15);
}

void mini_game_3::Init()
{
	innitPos();
	innitRotate();
	innitSize();

	//EVERYTHING NEEDED, DON'T TOUCH
	{

		// Init VBO here
		Mtx44 projection;

		projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2500.f);
		projectionStack.LoadMatrix(projection);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		camera.Init(Vector3(436, 25, 430), Vector3(0, 25, 0), Vector3(0, 1, 0));
		cameraPos = Vector3(camera.position.x, camera.position.y, camera.position.z);

		start == false;

		tutorialStart = true; tutorialFinish = false;
		near1 = near2 = near3 = near4 = near5 = near6 = near7 = near8 = near9 = near0 = false;
		picked1 = picked2 = picked3 = picked4 = picked5 = picked6 = picked7 = picked8 = picked9 = picked0 = false;
		passwordTimer = 10;
		tutorialTimer = 0;
		collectingTimer = 0;
		collected = false; wrong = false;

		warnings = 0;
		warningTimer = 0;

		canRenderDigitInteractions = false;

		win = false;
		lose = false;
		
		canSpawnPortal = false;
		canSpawnResult = false;

		test = 0;

		srand(time(0));
		for (int i = 0; i < 6; i++)
		{
			int num = rand() % 9 + 0;
			password.push_back(num);
		}

		light[0].type = Light::LIGHT_DIRECTIONAL;
		light[0].position.Set(0, 250, 0);
		light[0].color.Set(1, 1, 1);
		light[0].power = 4.5;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 3.f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);
		//light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		//light[0].type = Light::LIGHT_POINT;
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		//light[0].type = Light::LIGHT_SPOT;
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		//set background color
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

		//generate default VAO for now
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//load vertex and fragment shaders
		m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");


		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

		// Get a handle for our "colorTexture" uniform
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");


		m_parameters[U_TEXT_ENABLED] =
			glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
			"textColor");


		//Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		//use our shader
		glUseProgram(m_programID);
		// Make sure you pass uniform parameters after glUseProgram()
		glUniform1i(m_parameters[U_NUMLIGHTS], 1);


		//Enable depth test
		glEnable(GL_DEPTH_TEST);

	}



//LOAD THE MOPDELS
	{
		meshList[GEO_MAZEWALL] = MeshBuilder::GenerateOBJMTL("maze wall", "OBJ//mazeWall.obj", "OBJ//mazeWall.mtl");
		//Digits
		meshList[GEO_DIGIT0] = MeshBuilder::GenerateQuad("0", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT0]->textureID = LoadTGA("Images//digit0.tga");
		meshList[GEO_DIGIT1] = MeshBuilder::GenerateQuad("1", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT1]->textureID = LoadTGA("Images//digit1.tga");
		meshList[GEO_DIGIT2] = MeshBuilder::GenerateQuad("2", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT2]->textureID = LoadTGA("Images//digit2.tga");
		meshList[GEO_DIGIT3] = MeshBuilder::GenerateQuad("3", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT3]->textureID = LoadTGA("Images//digit3.tga");
		meshList[GEO_DIGIT4] = MeshBuilder::GenerateQuad("4", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT4]->textureID = LoadTGA("Images//digit4.tga");
		meshList[GEO_DIGIT5] = MeshBuilder::GenerateQuad("5", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT5]->textureID = LoadTGA("Images//digit5.tga");
		meshList[GEO_DIGIT6] = MeshBuilder::GenerateQuad("6", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT6]->textureID = LoadTGA("Images//digit6.tga");
		meshList[GEO_DIGIT7] = MeshBuilder::GenerateQuad("7", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT7]->textureID = LoadTGA("Images//digit7.tga");
		meshList[GEO_DIGIT8] = MeshBuilder::GenerateQuad("8", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT8]->textureID = LoadTGA("Images//digit8.tga");
		meshList[GEO_DIGIT9] = MeshBuilder::GenerateQuad("9", Color(1, 1, 1), 1);
		meshList[GEO_DIGIT9]->textureID = LoadTGA("Images//digit9.tga");
		//Portal
		meshList[GEO_PORTAL] = MeshBuilder::GenerateQuad("portal", Color(1, 1, 1), 1);
		meshList[GEO_PORTAL]->textureID = LoadTGA("Images//portal.tga");
	}
//Axes
	
// Texture for sky box
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//skybox_game3_front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//skybox_game3_back.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//skybox_game3_top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//skybox_game3_bottom.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//skybox_game3_left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//skybox_game3_right.tga");
//Lightball
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("light", Color(1, 1, 1), 10, 20, 1);
//Floor
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(1, 1, 1), 100);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Images//game3_floor.tga");
//UI
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UI", Color(1, 1, 1), 100);
	meshList[GEO_UI]->textureID = LoadTGA("Images//game1_UI.tga");
//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//MS_gothic.tga");


	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}

void mini_game_3::updateTutorial()
{
	if (Application::IsKeyPressed(VK_RETURN) && tutorialStart == true)
	{
		tutorialStart = false;
		tutorialTimer = 100;
	}
	if (Application::IsKeyPressed(VK_RETURN) && tutorialTimer == 0)
	{
		tutorialFinish = true;
		start = true;
		tutorialTimer = 100;
	}
}
void mini_game_3::updateIfNearDigit()
{
	float digitRange = 15;
	float digitRangez = 30;
	//Update
	{
		if (digitPos[0] + digitRange > camera.position.x && camera.position.x > digitPos[0] - digitRange
			&& camera.position.z < digitPos[2] + digitRangez && camera.position.z > digitPos[2]) //Checking for 0
		{
			canRenderDigitInteractions = true;
			near0 = true;
		}
		else if (digitPos[3] + digitRange > camera.position.x && camera.position.x > digitPos[3] - digitRange
			&& camera.position.z < digitPos[5] && camera.position.z > digitPos[5] - digitRangez) //Checking for 1 
		{
			canRenderDigitInteractions = true;
			near1 = true;
		}
		else if (digitPos[6] + digitRange > camera.position.x && camera.position.x > digitPos[6] - digitRange
			&& camera.position.z < digitPos[8] + digitRangez && camera.position.z > digitPos[8]) //Checking for 2
		{
			canRenderDigitInteractions = true;
			near2 = true;
		}
		else if (digitPos[9] + digitRange > camera.position.x && camera.position.x > digitPos[9] - digitRange
			&& camera.position.z < digitPos[11] + digitRangez && camera.position.z > digitPos[11]) //Checking for 3
		{
			canRenderDigitInteractions = true;
			near3 = true;
		}
		else if (digitPos[12] + digitRange > camera.position.x && camera.position.x > digitPos[12] - digitRange
			&& camera.position.z < digitPos[14] && camera.position.z > digitPos[2] - digitRangez) //Checking for 4
		{
			canRenderDigitInteractions = true;
			near4 = true;
		}
		else if (digitPos[15] + digitRange > camera.position.x && camera.position.x > digitPos[15] - digitRange
			&& camera.position.z < digitPos[17] + digitRangez && camera.position.z > digitPos[17]) //Checking for 5
		{
			canRenderDigitInteractions = true;
			near5 = true;
		}
		else if (digitPos[18] + digitRange > camera.position.x && camera.position.x > digitPos[18] - digitRange
			&& camera.position.z < digitPos[20] + digitRangez && camera.position.z > digitPos[20]) //Checking for 6
		{
			canRenderDigitInteractions = true;
			near6 = true;
		}
		else if (digitPos[21] + digitRange > camera.position.x && camera.position.x > digitPos[21] - digitRange
			&& camera.position.z < digitPos[23] && camera.position.z > digitPos[23] - digitRangez) //Checking for 7
		{
			canRenderDigitInteractions = true;
			near7 = true;
		}
		else if (digitPos[24] + digitRange > camera.position.x && camera.position.x > digitPos[24] - digitRange
			&& camera.position.z < digitPos[26] + digitRangez && camera.position.z > digitPos[26]) //Checking for 8
		{
			canRenderDigitInteractions = true;
			near8 = true;
		}
		else if (digitPos[27] + digitRange > camera.position.x && camera.position.x > digitPos[27] - digitRange
			&& camera.position.z < digitPos[29] + digitRangez && camera.position.z > digitPos[29]) //Checking for 9
		{
			canRenderDigitInteractions = true;
			near9 = true;
		}
		else
		{
			canRenderDigitInteractions = false;
			near1 = near2 = near3 = near4 = near5 = near6 = near7 = near8 = near9 = near0 = false;
		}
	}
}
void mini_game_3::updateDigitCollection()
{
	//In the middle of collecting
	if (Application::IsKeyPressed('F') && collectingTimer == 0)
	{
		if (near0 == true)
		{
			picked0 = true;
			collectingTimer = 100;
			digitsCollected.push_back(0);
		}
		if (near1 == true)
		{
			picked1 = true;
			collectingTimer = 100;
			digitsCollected.push_back(1);
		}
		if (near2 == true)
		{
			picked2 = true;
			collectingTimer = 100;
			digitsCollected.push_back(2);
		}
		if (near3 == true)
		{
			picked3 = true;
			collectingTimer = 100;
			digitsCollected.push_back(3);
		}
		if (near4 == true)
		{
			picked4 = true;
			collectingTimer = 100;
			digitsCollected.push_back(4);
		}
		if (near5 == true)
		{
			picked5 = true;
			collectingTimer = 100;
			digitsCollected.push_back(5);
		}
		if (near6 == true)
		{
			picked6 = true;
			collectingTimer = 100;
			digitsCollected.push_back(6);
		}
		if (near7 == true)
		{
			picked7 = true;
			collectingTimer = 100;
			digitsCollected.push_back(7);
		}
		if (near8 == true)
		{
			picked8 = true;
			collectingTimer = 100;
			digitsCollected.push_back(8);
		}
		if (near9 == true)
		{
			picked9 = true;
			collectingTimer = 100;
			digitsCollected.push_back(9);
		}

		collected = true;

		for (unsigned i = 0; i < digitsCollected.size(); i++)
		{
			if (password[i] != digitsCollected[i])
			{
				digitsCollected.pop_back();
				wrong = true;
			}
		}
	}

	//After collecting
	if (Application::IsKeyPressed(VK_RETURN) && collected == true && collectingTimer == 0)
	{
		collected = false;
		collectingTimer = 100;
	}
	if (Application::IsKeyPressed(VK_RETURN) && wrong == true)
	{
		wrong = false;
		collected = false;
		if (warningTimer == 0)
		{
			warnings++;
			warningTimer = 100;
		}
	}
}

void mini_game_3::updateEndGame()
{
	float portalWidth = 20;
	float range = -30;
	//Entering portal
	if (win == true)
	{
		if (camera.position.x > portalPos[0] - portalWidth && camera.position.x < portalPos[0] + portalWidth &&
			camera.position.z > portalPos[2] + range)
		{
			restart();
			while (!digitsCollected.empty())
			{
				digitsCollected.pop_back();
			}
			Application::setScene(4);
		}
	}

	if (lose == true && Application::IsKeyPressed(VK_RETURN) && collectingTimer == 0)
	{
		restart();
		while(!digitsCollected.empty())
		{
			digitsCollected.pop_back();
		}
		collectingTimer = 100;
		Application::setScene(0);
	}
}

void mini_game_3::restart()
{
	innitPos();
	innitRotate();
	innitSize();

	camera.Init(Vector3(436, 25, 430), Vector3(0, 25, 0), Vector3(0, 1, 0));
	cameraPos = Vector3(camera.position.x, camera.position.y, camera.position.z);

	start == false;

	tutorialStart = true; tutorialFinish = false;
	near1 = near2 = near3 = near4 = near5 = near6 = near7 = near8 = near9 = near0 = false;
	picked1 = picked2 = picked3 = picked4 = picked5 = picked6 = picked7 = picked8 = picked9 = picked0 = false;
	passwordTimer = 10;
	tutorialTimer = 0;
	collectingTimer = 0;
	collected = false; wrong = false;

	warnings = 0;
	warningTimer = 0;

	canRenderDigitInteractions = false;

	win = false;
	lose = false;

	canSpawnPortal = false;
	canSpawnResult = false;

	test = 0;

	srand(time(0));
	for (int i = 0; i < 6; i++)
	{
		int num = rand() % 9 + 0;
		password.push_back(num);
	}
}

void  mini_game_3::Update(double dt)
{
	fps = 1.f / dt;

	

	//Timer checks
	if (tutorialTimer > 0)
	{
		tutorialTimer -= (222 * dt);
		if (tutorialTimer < 0)
		{
			tutorialTimer = 0;
		}
	}
	if (collectingTimer > 0)
	{
		collectingTimer -= (222 * dt);
		if (collectingTimer < 0)
		{
			collectingTimer = 0;
		}
	}
	if (warningTimer > 0)
	{
		warningTimer -= (222 * dt);
		if (warningTimer < 0)
		{
			warningTimer = 0;
		}
	}
	updateTutorial();
	updateIfNearDigit();
	updateDigitCollection();
	updateEndGame();
	if (tutorialFinish == true && start == true)
	{
		camera.Update(dt, mazeWallPos, mazeWallSize, mazeWallRotate, digitPos, digitSize);
		passwordTimer -= (1 * dt);
		if (passwordTimer <= 0)
		{
			passwordTimer = 0;
		}
	}

	//Checking win/lose condition:
	if (warnings >= 3)
	{
		lose = true;
		win = false;
		canSpawnResult = true;
	}
	else if (digitsCollected.size() == 6)
	{
		win = true;
		lose = false;
		canSpawnPortal = true;
		canSpawnResult = true;
	}
	if (Application::IsKeyPressed(VK_RETURN) && canSpawnResult == true && collectingTimer == 0)
	{
		collectingTimer == 100;
		canSpawnResult = false;
		test = 1;
	}

	//Debugging (Comment out if not in use)
	if (Application::IsKeyPressed('P'))
	{
		std::cout << camera.position.x << ':' << camera.position.z << "   ";
	}


	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;


		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w; //convert (0,800) to (0,80)
		float posY = 60 - (y / h); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<
			std::endl;
		if (posX > 0.44 && posX < 0.56 && posY >
			59.42 && posY < 59.59)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		else
		{
			std::cout << "Miss!" << std::endl;

		}
	}

	{

		if (Application::IsKeyPressed('1')) //enable back face culling
			glEnable(GL_CULL_FACE);
		if (Application::IsKeyPressed('2')) //disable back face culling
			glDisable(GL_CULL_FACE);
		if (Application::IsKeyPressed('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
		if (Application::IsKeyPressed('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode




		float LSPEED = 30.0f;
		//MOVE LIGHT TO NEGATIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('I'))
			light[0].position.z -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('K'))
			light[0].position.z += (float)(LSPEED * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('J'))
			light[0].position.x -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('L'))
			light[0].position.x += (float)(LSPEED * dt);
		//GO DOWN
		if (Application::IsKeyPressed('O'))
			light[0].position.y -= (float)(LSPEED * dt);
		//GO UP
		if (Application::IsKeyPressed('P'))
			light[0].position.y += (float)(LSPEED * dt);


		if (Application::IsKeyPressed('5'))
		{
			//to do: switch light type to POINT and pass the information to shader
			light[0].type = Light::LIGHT_POINT;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		}
		else if (Application::IsKeyPressed('6'))
		{
			//to do: switch light type to DIRECTIONAL and pass the information to shader
			light[0].type = Light::LIGHT_DIRECTIONAL;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		}
		else if (Application::IsKeyPressed('7'))
		{
			//to do: switch light type to SPOT and pass the information to shader
			light[0].type = Light::LIGHT_SPOT;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		}


		//MOVE LIGHT TO NEGATIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('I'))
			light[0].position.z -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('K'))
			light[0].position.z += (float)(LSPEED * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('J'))
			light[0].position.x -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('L'))
			light[0].position.x += (float)(LSPEED * dt);
		//GO DOWN
		if (Application::IsKeyPressed('O'))
			light[0].position.y -= (float)(LSPEED * dt);
		//GO UP
		if (Application::IsKeyPressed('P'))
			light[0].position.y += (float)(LSPEED * dt);

		if (Application::IsKeyPressed('A'))
			translate_x -= (float)(50 * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('S'))
			translate_z += (float)(50 * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('D'))
			translate_x += (float)(50 * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('W'))
			translate_z -= (float)(50 * dt);

		if (translate_x >= 50)
		{
			translate_x = 50;
		}
		else if (translate_x <= -50)
		{
			translate_x = -50;
		}
		else if (translate_z <= -50)
		{
			translate_z = -50;
		}
		else if (translate_z >= 50)
		{
			translate_z = 50;
		}
		if (tutorialStart == false)
		{
			camera.Update(dt, mazeWallPos, mazeWallSize, mazeWallRotate, digitPos, digitSize);
		}
	}
}


void  mini_game_3::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}


	mesh->Render(); //this line should only be called once

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void mini_game_3::RenderSkybox()
{
	const float OFFSET = 999;

	modelStack.PushMatrix(); //Front
	modelStack.Translate(0, 0, -OFFSET);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Back
	modelStack.Translate(0, 0, OFFSET);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Bottom
	modelStack.Translate(0, OFFSET, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Top
	modelStack.Translate(0, -OFFSET, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Left
	modelStack.Translate(-OFFSET, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Right
	modelStack.Translate(OFFSET, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(2000, 2000, 2000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
}

void mini_game_3::RenderFloor()
{
	modelStack.PushMatrix();
	{
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_FLOOR], true);
	}
	modelStack.PopMatrix();
}

void mini_game_3::RenderMaze()
{
	//Maze
	for (unsigned i = 0; i < mazeWallPos.size(); i += 3)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(mazeWallPos[i], mazeWallPos[i + 1], mazeWallPos[i + 2]);
			modelStack.Rotate(mazeWallRotate[i], 0, 1, 0);
			modelStack.Scale(mazeWallSize[i], mazeWallSize[i + 1], mazeWallSize[i + 2]);
			RenderMesh(meshList[GEO_MAZEWALL], true);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_3::RenderDigits()
{
	//0
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[0], digitPos[1], digitPos[2]);
		modelStack.Scale(digitSize[0], digitSize[1], digitSize[2]);
		RenderMesh(meshList[GEO_DIGIT0], false);
	}
	modelStack.PopMatrix();
	//1
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[3], digitPos[4], digitPos[5]);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(digitSize[3], digitSize[4], digitSize[5]);
		RenderMesh(meshList[GEO_DIGIT1], false);
	}
	modelStack.PopMatrix();
	//2
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[6], digitPos[7], digitPos[8]);
		modelStack.Rotate(45, 0, 1, 0);
		modelStack.Scale(digitSize[6], digitSize[7], digitSize[8]);
		RenderMesh(meshList[GEO_DIGIT2], false);
	}
	modelStack.PopMatrix();
	//3
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[9], digitPos[10], digitPos[11]);
		modelStack.Scale(digitSize[9], digitSize[10], digitSize[11]);
		RenderMesh(meshList[GEO_DIGIT3], false);
	}
	modelStack.PopMatrix();
	//4
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[12], digitPos[13], digitPos[14]);
		modelStack.Rotate(225, 0, 1, 0);
		modelStack.Scale(digitSize[12], digitSize[13], digitSize[14]);
		RenderMesh(meshList[GEO_DIGIT4], false);
	}
	modelStack.PopMatrix();
	//5
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[15], digitPos[16], digitPos[17]);
		modelStack.Rotate(-45, 0, 1, 0);
		modelStack.Scale(digitSize[15], digitSize[16], digitSize[17]);
		RenderMesh(meshList[GEO_DIGIT5], false);
	}
	modelStack.PopMatrix();
	//6
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[18], digitPos[19], digitPos[20]);
		modelStack.Rotate(45, 0, 1, 0);
		modelStack.Scale(digitSize[18], digitSize[19], digitSize[20]);
		RenderMesh(meshList[GEO_DIGIT6], false);
	}
	modelStack.PopMatrix();
	//7
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[21], digitPos[22], digitPos[23]);
		modelStack.Rotate(225, 0, 1, 0);
		modelStack.Scale(digitSize[21], digitSize[22], digitSize[23]);
		RenderMesh(meshList[GEO_DIGIT7], false);
	}
	modelStack.PopMatrix();
	//8
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[24], digitPos[25], digitPos[26]);
		modelStack.Rotate(-45, 0, 1, 0);
		modelStack.Scale(digitSize[24], digitSize[25], digitSize[26]);
		RenderMesh(meshList[GEO_DIGIT8], false);
	}
	modelStack.PopMatrix();
	//9
	modelStack.PushMatrix();
	{
		modelStack.Translate(digitPos[27], digitPos[28], digitPos[29]);
		modelStack.Rotate(45, 0, 1, 0);
		modelStack.Scale(digitSize[27], digitSize[28], digitSize[29]);
		RenderMesh(meshList[GEO_DIGIT9], false);
	}
	modelStack.PopMatrix();
}

void mini_game_3::RenderPassword()
{
	if (passwordTimer != 0)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Password:", Color(0, 1, 0), 2.8, 2, 55);
		int gap = 3;
		for (unsigned i = 0; i < 6; i++)
		{
			std::ostringstream digit;
			digit.str("");
			digit << password[i];
			RenderTextOnScreen(meshList[GEO_TEXT], digit.str(), Color(0, 1, 0), 2.8, 25 + gap, 55);
			gap += 3;
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Disappears in:", Color(0, 1, 0), 2.8, 2, 50);
		std::ostringstream pwt;
		pwt.str("");
		pwt << round(passwordTimer);
		RenderTextOnScreen(meshList[GEO_TEXT], pwt.str(), Color(0, 1, 0), 2.8, 42, 50);
	}
	//Digits collected
	RenderTextOnScreen(meshList[GEO_TEXT], "Digits collected:", Color(0, 1, 0), 2.8, 2, 45);
	int gap = 3;
	for (unsigned i = 0; i < digitsCollected.size(); i++)
	{
		std::ostringstream digit;
		digit.str("");
		digit << digitsCollected[i];
		RenderTextOnScreen(meshList[GEO_TEXT], digit.str(), Color(0, 1, 0), 2.8, 48 + gap, 45);
		gap += 3;
	}
}

void mini_game_3::RenderWarnings()
{
	//Warnings
	RenderTextOnScreen(meshList[GEO_TEXT], "Warnings:", Color(0, 1, 0), 2.8, 2, 40);
	std::ostringstream digit;
	digit.str("");
	digit << warnings;
	RenderTextOnScreen(meshList[GEO_TEXT], digit.str(), Color(0, 1, 0), 2.8, 30, 40);
	RenderTextOnScreen(meshList[GEO_TEXT], "/3", Color(0, 1, 0), 2.8, 35, 40);
}

void mini_game_3::RenderGameResult()
{
	if (collected == false && canSpawnResult == true && test == 0)
	{
		if (win == true && lose == false)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Congratulations. You have collected all", Color(0, 1, 0), 2, 2, 34);
				RenderTextOnScreen(meshList[GEO_TEXT], "the digits.Find the portal tha---that", Color(0, 1, 0), 2, 2, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "spawned?????>??>?>/sgesgvsevg?>?>?./?-", Color(0, 1, 0), 2, 2, 26);
				RenderTextOnScreen(meshList[GEO_TEXT], "spawned to escc..esca--escape??", Color(0, 1, 0), 2, 2, 22);
				RenderTextOnScreen(meshList[GEO_TEXT], "                  [enter] to continue", Color(0, 1, 0), 2, 6, 16);
			}
			modelStack.PopMatrix();
		}
		if (win == false && lose == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Unfortunately, you have failed.", Color(0, 1, 0), 2, 2, 26);
				RenderTextOnScreen(meshList[GEO_TEXT], "                  [enter] to continue", Color(0, 1, 0), 2, 6, 16);
			}
			modelStack.PopMatrix();
		}
	}
}

void mini_game_3::RenderTutorialText()
{
	if (tutorialStart == true)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "A victim has given you their password,", Color(0, 1, 0), 2, 2, 38);
			RenderTextOnScreen(meshList[GEO_TEXT], "you must try to find its digits in", Color(0, 1, 0), 2, 2, 34);
			RenderTextOnScreen(meshList[GEO_TEXT], "this maze.Find all 6 digits to escape.", Color(0, 1, 0), 2, 2, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "Digits must be in order,you are given", Color(0, 1, 0), 2, 2, 26);
			RenderTextOnScreen(meshList[GEO_TEXT], "3 chances to make mistakes,or you lose.", Color(0, 1, 0), 2, 2, 22);
			RenderTextOnScreen(meshList[GEO_TEXT], "                  [enter] to continue", Color(0, 1, 0), 2, 6, 16);
		}
		modelStack.PopMatrix();
		
	}
	if (tutorialStart == false && tutorialFinish == false)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "TAKE NOTE:", Color(0, 1, 0), 2, 2, 38);
			RenderTextOnScreen(meshList[GEO_TEXT], "-Digits can be collected more than once", Color(0, 1, 0), 2, 2, 34);
			RenderTextOnScreen(meshList[GEO_TEXT], "-Password will disappear in 10 seconds", Color(0, 1, 0), 2, 2, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "-Up to 3 Warnings will be given when", Color(0, 1, 0), 2, 2, 26);
			RenderTextOnScreen(meshList[GEO_TEXT], "digit is collected in the wrong order", Color(0, 1, 0), 2, 2, 22);
			RenderTextOnScreen(meshList[GEO_TEXT], "                    [enter] to Begin", Color(0, 1, 0), 2, 6, 16);
		}
		modelStack.PopMatrix();

	}
}

void mini_game_3::RenderDigitInteractions()
{
	//When near a digit to collect
	if (collected == false)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "Press 'F' to collect this digit once", Color(0, 1, 0), 2, 2, 26);
		}
		modelStack.PopMatrix();
	}
	//After collecting a digit
	else if (collected == true && wrong == false)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "You have collected the digit", Color(0, 1, 0), 2, 2, 26);
			RenderTextOnScreen(meshList[GEO_TEXT], "                  [enter] to continue", Color(0, 1, 0), 2, 6, 16);
		}
		modelStack.PopMatrix();
	}
	else if (collected == true && wrong == true)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "WARNING: digit not in order.", Color(0, 1, 0), 2, 2, 26);
			RenderTextOnScreen(meshList[GEO_TEXT], "                  [enter] to continue", Color(0, 1, 0), 2, 6, 16);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_3::RenderPortal()
{
	modelStack.Translate(portalPos[0], portalPos[1], portalPos[2]);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(portalSize[0], portalSize[1], portalSize[2]);
	RenderMesh(meshList[GEO_PORTAL], false);
}

void mini_game_3::RenderDay()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "DAY 3", Color(0, 1, 0), 3, 55, 55);
}

void mini_game_3::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f
		//is the spacing of each character, you may change this value
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void mini_game_3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	//glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen


	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);


	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0);		//is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void mini_game_3::RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey, float rotate)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 0);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void  mini_game_3::Render()
{

	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();


	{
		if (light[0].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[0].position.x,
				light[0].position.y, light[0].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() *
				lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightDirection_cameraspace.x);
		}
		else if (light[0].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() *
				light[0].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1,
				&spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightPosition_cameraspace.x);
		}
	}

	//Render stuff here:
	//Skybox, light, floor
	modelStack.PushMatrix();
	{
		modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
	}
	modelStack.PopMatrix();
	RenderSkybox();
	RenderFloor();
	//Rendering models
	RenderMaze();
	RenderDay();
	RenderTutorialText();
	RenderDigits();
	if (lose == false)
	{
		if (tutorialFinish == true)
		{
			RenderPassword();
		}
		if (canRenderDigitInteractions == true && tutorialFinish == true)
		{
			RenderDigitInteractions();
		}
		if (warnings > 0)
		{
			RenderWarnings();
		}
	}
	RenderGameResult();
	if (canSpawnPortal == true)
	{
		RenderPortal();
	}
}

void  mini_game_3::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}