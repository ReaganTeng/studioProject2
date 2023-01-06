#include "mini_game_1.h"
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

mini_game_1::mini_game_1()
{
}

mini_game_1::~mini_game_1()
{
}

void mini_game_1::innitPos()
{
	//Table (1):
	tablePos.push_back(-85);
	tablePos.push_back(0);
	tablePos.push_back(-85);
	//Table (2):
	tablePos.push_back(-65);
	tablePos.push_back(0);
	tablePos.push_back(-85);
	//Table (3):
	tablePos.push_back(-45);
	tablePos.push_back(0);
	tablePos.push_back(-85);
	//Table (4):
	tablePos.push_back(-25);
	tablePos.push_back(0);
	tablePos.push_back(-85);
	//Table (5):
	tablePos.push_back(-5);
	tablePos.push_back(0);
	tablePos.push_back(-85);
	//Table (6):
	tablePos.push_back(15);
	tablePos.push_back(0);
	tablePos.push_back(-85);
	//Table (7):
	tablePos.push_back(35);
	tablePos.push_back(0);
	tablePos.push_back(-85);
	//Table (8):
	tablePos.push_back(55);
	tablePos.push_back(0);
	tablePos.push_back(-85);

	//NPC 1:
	NPCPos.push_back(-75);
	NPCPos.push_back(0);
	NPCPos.push_back(90);
	//NPC 2:
	NPCPos.push_back(-40);
	NPCPos.push_back(0);
	NPCPos.push_back(90);
	//NPC 3:
	NPCPos.push_back(-5);
	NPCPos.push_back(0);
	NPCPos.push_back(90);
	//NPC 4:
	NPCPos.push_back(30);
	NPCPos.push_back(0);
	NPCPos.push_back(90);
	//NPC 5:
	NPCPos.push_back(65);
	NPCPos.push_back(0);
	NPCPos.push_back(90);

	//Scam 1:
	scamPos.push_back(-85);
	scamPos.push_back(9);
	scamPos.push_back(-85);
	//Scam 2:
	scamPos.push_back(-50);
	scamPos.push_back(9);
	scamPos.push_back(-85);
	//Scam 3:
	scamPos.push_back(-15);
	scamPos.push_back(9);
	scamPos.push_back(-85);
	//Scam 4:
	scamPos.push_back(20);
	scamPos.push_back(9);
	scamPos.push_back(-85);
	//Scam 5:
	scamPos.push_back(55);
	scamPos.push_back(9);
	scamPos.push_back(-85);
}
void mini_game_1::innitSize()
{
	//Table sizes:
	for (int i = 0; i < 8; i++)
	{
		tableSize.push_back(20);
		tableSize.push_back(20);
		tableSize.push_back(20);
	}
	//NPC sizes:
	for (int i = 0; i < 5; i++)
	{
		NPCSize.push_back(40);
		NPCSize.push_back(40);
		NPCSize.push_back(40);
	}
	//Scam sizes:
	for (int i = 0; i < 5; i++)
	{
		scamSize.push_back(0.1);
		scamSize.push_back(0.1);
		scamSize.push_back(0.1);
	}
}

void mini_game_1::Init()
{
	innitPos();
	innitSize();

	//EVERYTHING NEEDED, DON'T TOUCH
	{

		// Init VBO here
		Mtx44 projection;

		projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
		projectionStack.LoadMatrix(projection);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		camera.Init(Vector3(4, 25, 3), Vector3(0, 25, 0), Vector3(0, 1, 0));
		cameraPos = Vector3(camera.position.x, camera.position.y, camera.position.z);

		successes = 0;
		failures = 0;
		attempts = 0;

		tutorialStart = true;
		tutorial2 = false;
		tutorial3 = false;

		nearScam1 = false; pickedScam1 = false;
		nearScam2 = false; pickedScam2 = false;
		nearScam3 = false; pickedScam3 = false;
		nearScam4 = false; pickedScam4 = false;
		nearScam5 = false; pickedScam5 = false;
		holdingAScam = false;
		enterInteraction = false;
		nearNPC1 = false; spawn1 = true;
		nearNPC2 = false; spawn2 = true;
		nearNPC3 = false; spawn3 = true;
		nearNPC4 = false; spawn4 = true;
		nearNPC5 = false; spawn5 = true;
		talking = false;
		talking1 = false;
		talking2 = false;
		talking3 = false;
		talking4 = false;
		talking5 = false;
		canGiveScam = false;
		renderResult = false;
		addToScore = false;
		whatScam = 0;
		whatNPC = 0;
		textTimer = 0;
		endTimer = 0;
		won = false; lost = false;
		end = false;

		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(0, 40, 0);
		light[0].color.Set(1, 1, 1);
		light[0].power = 5;
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
		meshList[GEO_DESK] = MeshBuilder::GenerateOBJMTL("desk", "OBJ//stall.obj", "OBJ//stall.mtl");
		meshList[GEO_VICTIM] = MeshBuilder::GenerateOBJMTL("victim", "OBJ//digger.obj", "OBJ//digger.mtl");

		//scam ads:
		meshList[GEO_PHONESCAM] = MeshBuilder::GenerateQuad("phone scam", Color(1, 1, 1), 100);
		meshList[GEO_PHONESCAM]->textureID = LoadTGA("Images//testingScam01.tga");
		meshList[GEO_JOBSCAM] = MeshBuilder::GenerateQuad("job scam", Color(1, 1, 1), 100);
		meshList[GEO_JOBSCAM]->textureID = LoadTGA("Images//scam02.tga");
		meshList[GEO_GIRLFRIENDSCAM] = MeshBuilder::GenerateQuad("gf scam", Color(1, 1, 1), 100);
		meshList[GEO_GIRLFRIENDSCAM]->textureID = LoadTGA("Images//scam03.tga");
		meshList[GEO_LOTTERYSCAM] = MeshBuilder::GenerateQuad("job scam", Color(1, 1, 1), 100);
		meshList[GEO_LOTTERYSCAM]->textureID = LoadTGA("Images//scam04.tga");
		meshList[GEO_MONEYSCAM] = MeshBuilder::GenerateQuad("job scam", Color(1, 1, 1), 100);
		meshList[GEO_MONEYSCAM]->textureID = LoadTGA("Images//scam05.tga");
	}
// Texture for sky box
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 100);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//exosystem_ft.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 100);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//exosystem_bk.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 100);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//exosystem_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 100);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//exosystem_dn.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 100);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//exosystem_lf.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 100);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//exosystem_rt.tga");
//Lightball
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("light", Color(1, 1, 1), 10, 20, 1);
//Floor
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(1, 1, 1), 100);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Images//game1_floor.tga");
//Room wall
	meshList[GEO_ROOMWALL] = MeshBuilder::GenerateQuad("roomWall", Color(1, 1, 1), 100);
	meshList[GEO_ROOMWALL]->textureID = LoadTGA("Images//game1_room_wall.tga");
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

void mini_game_1::checkCamCollision(Vector3 cameraPos)
{
	float halfScamLength = 4;
	float halfNPCLength = 2;
	//Check if near scams (minimised):
	{
		//Scam 1:
		if ((scamPos[0] + halfScamLength > camera.position.x - 5 && camera.position.x + 5 > scamPos[0] - halfScamLength ||
			scamPos[0] - halfScamLength < camera.position.x + 5 && camera.position.x + 5 < scamPos[0] + halfScamLength)
			&& camera.position.z < scamPos[2] + 30 && pickedScam1 == false)
		{
			nearScam1 = true;
		}
		else
		{
			nearScam1 = false;
		}
		//Scam 2:
		if ((scamPos[3] + halfScamLength > camera.position.x - 5 && camera.position.x + 5 > scamPos[3] - halfScamLength ||
			scamPos[3] - halfScamLength < camera.position.x + 5 && camera.position.x + 5 < scamPos[3] + halfScamLength)
			&& camera.position.z < scamPos[5] + 30 && pickedScam2 == false)
		{
			nearScam2 = true;
		}
		else
		{
			nearScam2 = false;
		}
		//Scam 3:
		if ((scamPos[6] + halfScamLength > camera.position.x - 5 && camera.position.x + 5 > scamPos[6] - halfScamLength ||
			scamPos[6] - halfScamLength < camera.position.x + 5 && camera.position.x + 5 < scamPos[6] + halfScamLength)
			&& camera.position.z < scamPos[8] + 30 && pickedScam3 == false)
		{
			nearScam3 = true;
		}
		else
		{
			nearScam3 = false;
		}
		//Scam 4:
		if ((scamPos[9] + halfScamLength > camera.position.x - 5 && camera.position.x + 5 > scamPos[9] - halfScamLength ||
			scamPos[9] - halfScamLength < camera.position.x + 5 && camera.position.x + 5 < scamPos[9] + halfScamLength)
			&& camera.position.z < scamPos[11] + 30 && pickedScam4 == false)
		{
			nearScam4 = true;
		}
		else
		{
			nearScam4 = false;
		}
		//Scam 5:
		if ((scamPos[12] + halfScamLength > camera.position.x - 5 && camera.position.x + 5 > scamPos[12] - halfScamLength ||
			scamPos[12] - halfScamLength < camera.position.x + 5 && camera.position.x + 5 < scamPos[12] + halfScamLength)
			&& camera.position.z < scamPos[14] + 30 && pickedScam5 == false)
		{
			nearScam5 = true;
		}
		else
		{
			nearScam5 = false;
		}
	}
	//Check if near NPC (minimised):
	{
		//NPC 1:
		if ((NPCPos[0] + halfNPCLength > camera.position.x - 5 && camera.position.x + 5 > NPCPos[0] - halfNPCLength ||
			NPCPos[0] - halfNPCLength < camera.position.x + 5 && camera.position.x + 5 < NPCPos[0] + halfNPCLength)
			&& camera.position.z > NPCPos[2] - 30)
		{
			nearNPC1 = true;
		}
		else
		{
			nearNPC1 = false;
		}
		//NPC 2:
		if ((NPCPos[3] + halfNPCLength > camera.position.x - 5 && camera.position.x + 5 > NPCPos[3] - halfNPCLength ||
			NPCPos[3] - halfNPCLength < camera.position.x + 5 && camera.position.x + 5 < NPCPos[3] + halfNPCLength)
			&& camera.position.z > NPCPos[5] - 30)
		{
			nearNPC2 = true;
		}
		else
		{
			nearNPC2 = false;
		}
		//NPC 3:
		if ((NPCPos[6] + halfNPCLength > camera.position.x - 5 && camera.position.x + 5 > NPCPos[6] - halfNPCLength ||
			NPCPos[6] - halfNPCLength < camera.position.x + 5 && camera.position.x + 5 < NPCPos[6] + halfNPCLength)
			&& camera.position.z > NPCPos[8] - 30)
		{
			nearNPC3 = true;
		}
		else
		{
			nearNPC3 = false;
		}
		//NPC 4:
		if ((NPCPos[9] + halfNPCLength > camera.position.x - 5 && camera.position.x + 5 > NPCPos[9] - halfNPCLength ||
			NPCPos[9] - halfNPCLength < camera.position.x + 5 && camera.position.x + 5 < NPCPos[9] + halfNPCLength)
			&& camera.position.z > NPCPos[11] - 30)
		{
			nearNPC4 = true;
		}
		else
		{
			nearNPC4 = false;
		}
		//NPC 5:
		if ((NPCPos[12] + halfNPCLength > camera.position.x - 5 && camera.position.x + 5 > NPCPos[12] - halfNPCLength ||
			NPCPos[12] - halfNPCLength < camera.position.x + 5 && camera.position.x + 5 < NPCPos[12] + halfNPCLength)
			&& camera.position.z > NPCPos[14] - 30)
		{
			nearNPC5 = true;
		}
		else
		{
			nearNPC5 = false;
		}
	}
}

void mini_game_1::checkforInteraction()
{
	//Checking for scam interaction
	if (holdingAScam == false)
	{
		if (nearScam1 == true && Application::IsKeyPressed('E'))
		{
			pickedScam1 = true;
			nearScam1 = false;
			holdingAScam = true;
			whatScam = 1;
		}
		if (nearScam2 == true && Application::IsKeyPressed('E'))
		{
			pickedScam2 = true;
			nearScam2 = false;
			holdingAScam = true;
			whatScam = 2;
		}
		if (nearScam3 == true && Application::IsKeyPressed('E'))
		{
			pickedScam3 = true;
			nearScam3 = false;
			holdingAScam = true;
			whatScam = 3;
		}
		if (nearScam4 == true && Application::IsKeyPressed('E'))
		{
			pickedScam4 = true;
			nearScam4 = false;
			holdingAScam = true;
			whatScam = 4;
		}
		if (nearScam5 == true && Application::IsKeyPressed('E'))
		{
			pickedScam5 = true;
			nearScam5 = false;
			holdingAScam = true;
			whatScam = 5;
		}
	}
	//Checking for NPC interaction (before talk)
	if (talking == false && renderResult == false)
	{
		if (nearNPC1 == true && Application::IsKeyPressed('F'))
		{
			nearNPC1 = false;
			talking1 = true;
			talking = true;
		}
		if (nearNPC2 == true && Application::IsKeyPressed('F'))
		{
			nearNPC2 = false;
			talking2 = true;
			talking = true;
		}
		if (nearNPC3 == true && Application::IsKeyPressed('F'))
		{
			nearNPC3 = false;
			talking3 = true;
			talking = true;
		}
		if (nearNPC4 == true && Application::IsKeyPressed('F'))
		{
			nearNPC4 = false;
			talking4 = true;
			talking = true;
		}
		if (nearNPC5 == true && Application::IsKeyPressed('F'))
		{
			nearNPC5 = false;
			talking5 = true;
			talking = true;
		}
	}
	//Checking for NPC interaction (after talk)
	if (talking == true)
	{
		if (talking1 == true && Application::IsKeyPressed(VK_RETURN))
		{
			nearNPC1 = true;
			talking1 = false;
			talking = false;
		}
		if (talking2 == true && Application::IsKeyPressed(VK_RETURN))
		{
			nearNPC2 = true;
			talking2 = false;
			talking = false;
		}
		if (talking3 == true && Application::IsKeyPressed(VK_RETURN))
		{
			nearNPC3 = true;
			talking3 = false;
			talking = false;
		}
		if (talking4 == true && Application::IsKeyPressed(VK_RETURN))
		{
			nearNPC4 = true;
			talking4 = false;
			talking = false;
		}
		if (talking5 == true && Application::IsKeyPressed(VK_RETURN))
		{
			nearNPC5 = true;
			talking5 = false;
			talking = false;
		}
	}
	//Checking for NPC interaction (Giving scam)
	if (holdingAScam == true)
	{
		if (nearNPC1 == true && holdingAScam == true)
		{
			canGiveScam = true;
			if (Application::IsKeyPressed('F'))
			{
				pickedScam1 = false;
				CheckScamGiven();
				canGiveScam = true;
				holdingAScam = false;
				whatNPC = 1;
				renderResult = true;
				nearNPC1 = false;
				addToScore = true;
			}
		}
		if (nearNPC2 == true && holdingAScam == true)
		{
			canGiveScam = true;
			if (Application::IsKeyPressed('F'))
			{
				pickedScam2 = false;
				CheckScamGiven();
				canGiveScam = true;
				holdingAScam = false;
				whatNPC = 2;
				renderResult = true;
				nearNPC2 = false;
				addToScore = true;
			}
		}
		if (nearNPC3 == true && holdingAScam == true)
		{
			canGiveScam = true;
			if (Application::IsKeyPressed('F'))
			{
				pickedScam3 = false;
				CheckScamGiven();
				canGiveScam = true;
				holdingAScam = false;
				whatNPC = 3;
				renderResult = true;
				nearNPC3 = false;
				addToScore = true;
			}
		}
		if (nearNPC4 == true && holdingAScam == true)
		{
			canGiveScam = true;
			if (Application::IsKeyPressed('F'))
			{
				pickedScam4 = false;
				CheckScamGiven();
				canGiveScam = true;
				holdingAScam = false;
				whatNPC = 4;
				renderResult = true;
				nearNPC4 = false;
				addToScore = true;
			}
		}
		if (nearNPC5 == true && holdingAScam == true)
		{
			canGiveScam = true;
			if (Application::IsKeyPressed('F'))
			{
				pickedScam5 = false;
				CheckScamGiven();
				canGiveScam = true;
				holdingAScam = false;
				whatNPC = 5;
				renderResult = true;
				nearNPC5 = false;
				addToScore = true;
			}
		}
	}
	//Checking for MPC interaction (After giving scam)
	if (renderResult == true && Application::IsKeyPressed(VK_RETURN))
	{
		renderResult = false;
		pickedScam1 = false; pickedScam2 = false; pickedScam3 = false; pickedScam4 = false; pickedScam5 = false;
		if (whatNPC == 1)
			spawn1 = false;
		if (whatNPC == 2)
			spawn2 = false;
		if (whatNPC == 3)
			spawn3 = false;
		if (whatNPC == 4)
			spawn4 = false;
		if (whatNPC == 5)
			spawn5 = false;
		whatNPC = 0;
		whatScam = 0;
	}
}

void mini_game_1::CheckScamGiven()
{
	success = false;
	attempts++;
	if (whatScam == 3 && whatNPC == 1 && renderResult == true)
	{
		success = true;
		successes++;
		attempts--;
	}
	else if (whatScam == 4 && whatNPC == 2 && renderResult == true)
	{
		success = true;
		successes++;
		attempts--;
	}
	else if (whatScam == 1 && whatNPC == 3 && renderResult == true)
	{
		success = true;
		successes++;
		attempts--;
	}
	else if (whatScam == 2 && whatNPC == 4 && renderResult == true)
	{
		success = true;
		successes++;
		attempts--;
	}
	else if (whatScam == 5 && whatNPC == 5 && renderResult == true)
	{
		success = true;
		successes++;
		attempts--;
	}
	else if (success == false && renderResult == true)
	{
		attempts--;
	}
}

void mini_game_1::UpdateEndgame()
{
	if (Application::IsKeyPressed(VK_RETURN) && (won == true || lost == true))
	{
		end = true;
	}
}

void mini_game_1::restart()
{
	innitPos();
	innitSize();

	camera.Init(Vector3(4, 25, 3), Vector3(0, 25, 0), Vector3(0, 1, 0));
	cameraPos = Vector3(camera.position.x, camera.position.y, camera.position.z);

	successes = 0;
	failures = 0;
	attempts = 0;

	tutorialStart = true;
	tutorial2 = false;
	tutorial3 = false;

	nearScam1 = false; pickedScam1 = false;
	nearScam2 = false; pickedScam2 = false;
	nearScam3 = false; pickedScam3 = false;
	nearScam4 = false; pickedScam4 = false;
	nearScam5 = false; pickedScam5 = false;
	holdingAScam = false;
	enterInteraction = false;
	nearNPC1 = false; spawn1 = true;
	nearNPC2 = false; spawn2 = true;
	nearNPC3 = false; spawn3 = true;
	nearNPC4 = false; spawn4 = true;
	nearNPC5 = false; spawn5 = true;
	talking = false;
	talking1 = false;
	talking2 = false;
	talking3 = false;
	talking4 = false;
	talking5 = false;
	canGiveScam = false;
	renderResult = false;
	addToScore = false;
	whatScam = 0;
	whatNPC = 0;
	textTimer = 0;
	endTimer = 0;
	won = false; lost = false;
	end = false;
}

bool mini_game_1::lose()
{
	return true;
}
int mini_game_1::win()
{
	return successes;
}

void  mini_game_1::Update(double dt)
{
	fps = 1.f / dt;

	//text timer tick down
	if (textTimer > 0)
	{
		textTimer -= (222 * dt);
		if (textTimer <= 0)
		{
			textTimer = 0;
		}
	}
	if (endTimer > 0)
	{
		endTimer -= (222 * dt);
		if (endTimer < 0)
		{
			endTimer = 0;
		}
	}
	//Camera collision:
	cameraPos.x = camera.position.x;
	cameraPos.z = camera.position.z;
	camera.Update(dt, NPCPos, NPCSize, tablePos, tableSize);
	checkCamCollision(cameraPos);
	checkforInteraction();
	if (addToScore == true)
	{
		CheckScamGiven();
		addToScore = false;
	}
	//Checking for loss/win'
	if (attempts == 5)
	{
		if (successes >= 3)
		{
			lost = false;
			won = true;
		}
		else
		{
			won = false;
			lost = true;
		}
	}
	UpdateEndgame();
	if (Application::IsKeyPressed('P'))
	{
		std::cout << Application::IsKeyPressed(VK_RETURN) << won << end << ", ";
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
		float posX = x / w * 80; //convert (0,800) to (0,80)
		float posY = 60 - (y / h * 60); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY <<std::endl;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
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



	
	

	if (Application::IsKeyPressed(VK_RETURN))
	{
		if (won == true && lost == false)
		{
			restart();
			Application::setScene(2);
		}
		else if (won == false && lost == true)
		{
			restart();
			Application::setScene(0);
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

		camera.Update(dt, NPCPos, NPCSize, tablePos, tableSize);
	}
}


void  mini_game_1::RenderMesh(Mesh* mesh, bool enableLight)
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

void mini_game_1::RenderInteractionText()
{
	if (tutorialStart == false)
	{
		//Scam interaction que
		if (holdingAScam == false)
		{
			if (nearScam1 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' to pick up scam 1", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearScam2 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' to pick up scam 2", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearScam3 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' to pick up scam 3", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearScam4 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' to pick up scam 4", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearScam5 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'E' to pick up scam 5", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
		}
		//NPC interaction que
		if (holdingAScam == false && talking == false)
		{
			if (nearNPC1 == true && spawn1 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'F' to talk to NPC1", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearNPC2 == true && spawn2 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'F' to talk to NPC2", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearNPC3 == true && spawn3 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'F' to talk to NPC3", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearNPC4 == true && spawn4 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'F' to talk to NPC4", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
			if (nearNPC5 == true && spawn5 == true)
			{
				modelStack.PushMatrix();
				{
					RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
					RenderTextOnScreen(meshList[GEO_TEXT], "Press 'F' to talk to NPC5", Color(0, 1, 0), 3, 0, 30);
				}
				modelStack.PopMatrix();
			}
		}
	}
	//Tutorial text
	if (tutorialStart == true)
	{
		if (tutorial2 == false && tutorial3 == false)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Talk to NPCs and find out", Color(0, 1, 0), 2.6, 2, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "their needs.([enter] to cont.)", Color(0, 1, 0), 2.6, 2, 25);
			}
			modelStack.PopMatrix();
			if (Application::IsKeyPressed(VK_RETURN))
			{
				tutorial2 = true;
				textTimer = 100;
			}
		}
		if (tutorial2 == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Grab ads that match needs", Color(0, 1, 0), 2.6, 6, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "and give it to the NPC", Color(0, 1, 0), 2.6, 6, 25);
				RenderTextOnScreen(meshList[GEO_TEXT], "([enter] to Continue.)", Color(0, 1, 0), 2.6, 6, 20);
			}
			modelStack.PopMatrix();
			if (Application::IsKeyPressed(VK_RETURN) && textTimer == 0)
			{
				tutorial3 = true;
				tutorial2 = false;
				textTimer = 100;
			}
		}
		else if (tutorial3 == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Get at least 3/5 right", Color(0, 1, 0), 2.6, 2, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "to move to the next day", Color(0, 1, 0), 2.6, 2, 25);
				RenderTextOnScreen(meshList[GEO_TEXT], "([enter] to continue.)", Color(0, 1, 0), 2.6, 2, 20);
			}
			modelStack.PopMatrix();
			if (Application::IsKeyPressed(VK_RETURN) && textTimer == 0)
			{
				tutorialStart = false;
				tutorial3 = false;
				textTimer = 100;
			}
		}
	}
	//"Giving scam" text
	if (canGiveScam == true && holdingAScam == true && 
		((nearNPC1 == true && spawn1 == true) || (nearNPC2 == true && spawn2 == true) ||
		(nearNPC3 == true && spawn3 == true) || (nearNPC4 == true && spawn4 == true) || (nearNPC5 == true && spawn5 == true)))
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "Press [F] to give the", Color(0, 1, 0), 2.6, 6, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "scam to the dumb victim", Color(0, 1, 0), 2.6, 6, 25);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_1::RenderNPCSpeech()
{
	if (renderResult == false)
	{
		if (talking1 == true && spawn1 == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Man... I'm so single", Color(0, 1, 0), 3, 4, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "and lonely...", Color(0, 1, 0), 3, 4, 25);
				RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
			}
			modelStack.PopMatrix();
		}
		if (talking2 == true && spawn2 == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Just bought a 4D number,", Color(0, 1, 0), 3, 4, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "hope I win!", Color(0, 1, 0), 3, 4, 25);
				RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
			}
			modelStack.PopMatrix();
		}
		if (talking3 == true && spawn3 == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Why are I-Phones these", Color(0, 1, 0), 3, 4, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "days so expensive?", Color(0, 1, 0), 3, 4, 25);
				RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
			}
			modelStack.PopMatrix();
		}
		if (talking4 == true && spawn4 == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "It's the holidays,", Color(0, 1, 0), 3, 4, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "I want a job!", Color(0, 1, 0), 3, 4, 25);
				RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
			}
			modelStack.PopMatrix();
		}
		if (talking5 == true && spawn5 == true)
		{
			modelStack.PushMatrix();
			{
				RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "My mom is sick, how can I", Color(0, 1, 0), 3, 4, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], "pay for her medication?", Color(0, 1, 0), 3, 4, 25);
				RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
			}
			modelStack.PopMatrix();
		}
	}
}

void mini_game_1::RenderScore()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "Current score:", Color(0, 1, 0), 2.8, 2, 55);
	std::ostringstream ss;
	ss.str("");
	ss << successes;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.8, 2, 50);
	RenderTextOnScreen(meshList[GEO_TEXT], "/5", Color(0, 1, 0), 2.8, 6, 50);
}

void mini_game_1::RenderScamResult(bool success)
{
	if (success == true)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "Thank you for the Ad,", Color(0, 1, 0), 2.6, 6, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "I really needed it!", Color(0, 1, 0), 2.6, 6, 25);
			RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
		}
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "I don't need this, are", Color(0, 1, 0), 2.6, 6, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "you a scammer? L+Ratio.", Color(0, 1, 0), 2.6, 6, 25);
			RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_1::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 499);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();                 //front

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -499);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();                //back

	modelStack.PushMatrix();
	modelStack.Translate(-499, 0, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();                //RIGHT

	modelStack.PushMatrix();
	modelStack.Translate(499, 0, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();               //left

	modelStack.PushMatrix();
	modelStack.Translate(0, 499, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();              //top

	modelStack.PushMatrix();
	modelStack.Translate(0, -499, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();             //bottom
}

void mini_game_1::RenderRoom()
{
	//Right wall
	modelStack.PushMatrix();
	{
		modelStack.Translate(100, 15, -100);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
	}
	modelStack.PopMatrix();
	//Left wall
	modelStack.PushMatrix();
	{
		modelStack.Translate(100, 15, 100);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
	}
	modelStack.PopMatrix();
	//Front wall
	modelStack.PushMatrix();
	{
		modelStack.Translate(-100, 15, 100);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
	}
	modelStack.PopMatrix();
	//Back wall
	modelStack.PushMatrix();
	{
		modelStack.Translate(100, 15, -100);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
		modelStack.Translate(50, 0, 0);
		RenderMesh(meshList[GEO_ROOMWALL], true);
	}
	modelStack.PopMatrix();
	//Ceiling
	modelStack.PushMatrix();
	{
		modelStack.Translate(0, 50, 0);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_ROOMWALL], true);
	}
	modelStack.PopMatrix();
}

void mini_game_1::RenderScams()
{
	//Rendering scam advertisements:
	if (pickedScam1 == false)
	{
		modelStack.PushMatrix();//scam 1
		{
			modelStack.Translate(scamPos[0], scamPos[1], scamPos[2]);
			modelStack.Rotate(-60, 1, 0, 0);
			modelStack.Scale(scamSize[0], scamSize[1], scamSize[2]);
			RenderMesh(meshList[GEO_PHONESCAM], false);
		}
		modelStack.PopMatrix();
	}
	if (pickedScam2 == false)
	{
		modelStack.PushMatrix();//scam 2
		{
			modelStack.Translate(scamPos[3], scamPos[4], scamPos[5]);
			modelStack.Rotate(-60, 1, 0, 0);
			modelStack.Scale(scamSize[3], scamSize[4], scamSize[5]);
			RenderMesh(meshList[GEO_JOBSCAM], false);
		}
		modelStack.PopMatrix();
	}
	if (pickedScam3 == false)
	{
		modelStack.PushMatrix();//scam 3
		{
			modelStack.Translate(scamPos[6], scamPos[7], scamPos[8]);
			modelStack.Rotate(-60, 1, 0, 0);
			modelStack.Scale(scamSize[6], scamSize[7], scamSize[8]);
			RenderMesh(meshList[GEO_GIRLFRIENDSCAM], false);
		}
		modelStack.PopMatrix();
	}
	if (pickedScam4 == false)
	{
		modelStack.PushMatrix();//scam 4
		{
			modelStack.Translate(scamPos[9], scamPos[10], scamPos[11]);
			modelStack.Rotate(-60, 1, 0, 0);
			modelStack.Scale(scamSize[9], scamSize[10], scamSize[11]);
			RenderMesh(meshList[GEO_LOTTERYSCAM], false);
		}
		modelStack.PopMatrix();
	}
	if (pickedScam5 == false)
	{
		modelStack.PushMatrix();//scam 5
		{
			modelStack.Translate(scamPos[12], scamPos[13], scamPos[14]);
			modelStack.Rotate(-60, 1, 0, 0);
			modelStack.Scale(scamSize[12], scamSize[13], scamSize[14]);
			RenderMesh(meshList[GEO_MONEYSCAM], false);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_1::RenderScamInHand()
{
	if (pickedScam1 == true && holdingAScam == true)
	{
		RenderMeshOnScreen(meshList[GEO_PHONESCAM], 40, 5, 0.3f, 0.3f, -60);
	}
	if (pickedScam2 == true && holdingAScam == true)
	{
		RenderMeshOnScreen(meshList[GEO_JOBSCAM], 40, 5, 0.3f, 0.3f, -60);
	}
	if (pickedScam3 == true && holdingAScam == true)
	{
		RenderMeshOnScreen(meshList[GEO_GIRLFRIENDSCAM], 40, 5, 0.3f, 0.3f, -60);
	}
	if (pickedScam4 == true && holdingAScam == true)
	{
		RenderMeshOnScreen(meshList[GEO_LOTTERYSCAM], 40, 5, 0.3f, 0.3f, -60);
	}
	if (pickedScam5 == true && holdingAScam == true)
	{
		RenderMeshOnScreen(meshList[GEO_MONEYSCAM], 40, 5, 0.3f, 0.3f, -60);
	}
}

void mini_game_1::RenderEndGame()
{
	if (won == true)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "Congratulations,", Color(0, 1, 0), 3, 4, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "you have succeeded.", Color(0, 1, 0), 3, 4, 25);
			RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
		}
		modelStack.PopMatrix();
	}
	if (lost == true)
	{
		modelStack.PushMatrix();
		{
			RenderMeshOnScreen(meshList[GEO_UI], 40, 30, 0.85, 0.3, 0);
			RenderTextOnScreen(meshList[GEO_TEXT], "Unfortunately,", Color(0, 1, 0), 3, 4, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "you have failed.", Color(0, 1, 0), 3, 4, 25);
			RenderTextOnScreen(meshList[GEO_TEXT], "[enter] to continue.", Color(0, 1, 0), 3, 4, 20);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_1::RenderDesk()
{
	for (int i = 0; i < 24; i += 3)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(tablePos[i], tablePos[i + 1], tablePos[i + 2]);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(tableSize[i], tableSize[i + 1], tableSize[i + 2]);
			RenderMesh(meshList[GEO_DESK], true);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_1::RenderVictim()
{
	if (spawn1 == true)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(NPCPos[0], NPCPos[1], NPCPos[2]);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(NPCSize[0], NPCSize[1], NPCSize[2]);
			RenderMesh(meshList[GEO_VICTIM], true);
		}
		modelStack.PopMatrix();
	}
	if (spawn2 == true)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(NPCPos[3], NPCPos[4], NPCPos[5]);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(NPCSize[3], NPCSize[4], NPCSize[5]);
			RenderMesh(meshList[GEO_VICTIM], true);
		}
		modelStack.PopMatrix();
	}
	if (spawn3 == true)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(NPCPos[6], NPCPos[7], NPCPos[8]);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(NPCSize[6], NPCSize[7], NPCSize[8]);
			RenderMesh(meshList[GEO_VICTIM], true);
		}
		modelStack.PopMatrix();
	}
	if (spawn4 == true)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(NPCPos[9], NPCPos[10], NPCPos[11]);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(NPCSize[9], NPCSize[10], NPCSize[11]);
			RenderMesh(meshList[GEO_VICTIM], true);
		}
		modelStack.PopMatrix();
	}
	if (spawn5 == true)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(NPCPos[12], NPCPos[13], NPCPos[14]);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(NPCSize[12], NPCSize[13], NPCSize[14]);
			RenderMesh(meshList[GEO_VICTIM], true);
		}
		modelStack.PopMatrix();
	}
}

void mini_game_1::RenderDay()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "DAY 1", Color(0, 1, 0), 3, 55, 55);
}

void mini_game_1::RenderText(Mesh* mesh, std::string text, Color color)
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

void mini_game_1::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void mini_game_1::RenderMeshOnScreen(Mesh* mesh, int x, int y, float sizex, float sizey, float rotate)
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

void  mini_game_1::Render()
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
	RenderSkybox();
	modelStack.PushMatrix();
	{
		modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
	}
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	{
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_FLOOR], true);
	}
	modelStack.PopMatrix();
	//Rendering Room
	RenderRoom();
	if (won == false && lost == false)
	{
		RenderDesk();
		RenderVictim();
		RenderScams();
		RenderScamInHand();
		RenderInteractionText();
		RenderNPCSpeech();
		RenderDay();
		if (renderResult == true)
		{
			RenderScamResult(success);
		}
		if (tutorialStart == false)
		{
			RenderScore();
		}
	}
	else
	{
		RenderEndGame();
	}
}

void  mini_game_1::Exit()
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