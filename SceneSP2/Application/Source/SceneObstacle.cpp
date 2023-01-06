#include "SceneObstacle.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"

#include <sstream>
#include <math.h>

enum obstacleType
{
	BUILDING_1,
	BUILDING_2,
	POLICE,
	MEDKIT,
};

SceneObstacle::SceneObstacle()
{
}

SceneObstacle::~SceneObstacle()
{
}

void SceneObstacle::InitAttributes()
{
	// Set for obstacles

	const int obstacleCount = 170;
	const int startPos = 280;
	int generateObstacleType;
	srand(time(NULL));
	for (int i = 0; i < obstacleCount; ++i)
	{
		obstaclePos.push_back(startPos + (i * 33));
		obstaclePos.push_back(0);
		obstaclePos.push_back((rand() % 180) - 90);

		generateObstacleType = rand() % 100; // Giving rates that a particular obstacle will spawn
		if (generateObstacleType <= 15) // Building 1
		{
			obstacleType.push_back(BUILDING_1);
			obstacleSize.push_back(20);
			obstacleSize.push_back(20);
			obstacleSize.push_back(20);
		}
		else if (generateObstacleType <= 30) // Building 2
		{
			obstacleType.push_back(BUILDING_2);
			obstacleSize.push_back(40);
			obstacleSize.push_back(40);
			obstacleSize.push_back(40);
		}
		else if (generateObstacleType <= 92) // Police
		{
			obstacleType.push_back(POLICE);
			obstacleSize.push_back(1);
			obstacleSize.push_back(1);
			obstacleSize.push_back(1);
		}
		else
		{
			obstacleType.push_back(MEDKIT);
			obstacleSize.push_back(7);
			obstacleSize.push_back(7);
			obstacleSize.push_back(7);
		}
	}

	int treeCount = 100;
	int treeRandomPos = 0;
	int treeSide;
	for (unsigned i = 0; i < treeCount; ++i)
	{
		treeRandomPos += (rand() % 100) + 30;
		treePos.push_back(treeRandomPos);
		treePos.push_back(0);
		treeSide = rand() % 2;
		if (treeSide == 1)
		{
			treePos.push_back(-150);
		}
		else
		{
			treePos.push_back(150);
		}
	}
}

void SceneObstacle::RenderPolice()
{
	//HEAD
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, 0);
	modelStack.Scale(0.2, 0.1, 0.2);

	// Helmet
	modelStack.PushMatrix();
	{
		modelStack.Translate(0, 5, 0);
		modelStack.Scale(2, 2, 2);
		modelStack.Rotate(-90, 1, 0, 0);
		RenderMesh(meshList[GEO_POLICEHELMET], true);
	}
	modelStack.PopMatrix();

	//torso
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -40, 0);
		modelStack.Scale(1, 5, 1);
		modelStack.Rotate(0, 0, 0, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();
	}

	//LEFT ARM
	{
		modelStack.PushMatrix();
		modelStack.Translate(15, -20, 15);
		modelStack.Rotate(0, 0, 0, 1);


		//left for arm
		modelStack.PushMatrix();
		modelStack.Translate(5, 0, 25);
		modelStack.Rotate(90, 90, 0, 1);
		modelStack.Scale(1, 3, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();

		//left back arm
		modelStack.PushMatrix();
		modelStack.Rotate(90, -1.5, 0, 1);
		modelStack.Scale(1, 2, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();


		RenderMesh(meshList[GEO_PALM], true);
		modelStack.PopMatrix();
	}

	//RIGHT ARM
	{
		modelStack.PushMatrix();
		modelStack.Translate(-15, -20, 15);

		//right for arm
		modelStack.PushMatrix();
		modelStack.Translate(-5, 0, 25);
		modelStack.Rotate(90, 90, 0, 1);
		modelStack.Scale(1, 3, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();

		//right back arm
		modelStack.PushMatrix();
		modelStack.Rotate(90, 1.5, 0, 1);
		modelStack.Scale(1, 2, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();


		RenderMesh(meshList[GEO_PALM], true);
		modelStack.PopMatrix();
	}


	//ENTIRE RIGHT LEG
	{


		//right thigh joint
		modelStack.PushMatrix();
		modelStack.Translate(-4, -60, 0);
		modelStack.Rotate(0, 0, 0, 1);


		//right thigh
		modelStack.PushMatrix();
		modelStack.Translate(0, -15, 0);
		modelStack.Scale(1, 2, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();


		//right lower leg joint
		modelStack.PushMatrix();
		modelStack.Translate(0, -5, 0);
		modelStack.Rotate(30, 30, 0, 1);

		//right lower leg
		modelStack.PushMatrix();
		modelStack.Translate(0, -30, 10);
		modelStack.Scale(1, 3, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();

		//right feet
		modelStack.PushMatrix();
		modelStack.Translate(0, -45, 15);
		modelStack.Scale(10, 5, 20);
		RenderMesh(meshList[GEO_FEET], true);
		modelStack.PopMatrix();


		RenderMesh(meshList[GEO_PALM], true);
		modelStack.PopMatrix();




		RenderMesh(meshList[GEO_PALM], true);
		modelStack.PopMatrix();
	}


	//ENTIRE LEFT LEG
	{
		//left thigh joint
		modelStack.PushMatrix();
		modelStack.Translate(4, -60, 0);
		modelStack.Rotate(0, 0, 0, 1);

		//left thigh
		modelStack.PushMatrix();
		modelStack.Translate(0, -15, 0);
		modelStack.Scale(1, 2, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();


		//left lower leg joint
		modelStack.PushMatrix();
		modelStack.Translate(0, -5, 0);
		modelStack.Rotate(30, 30, 0, 1);

		//left lower leg
		modelStack.PushMatrix();
		modelStack.Translate(0, -30, 10);
		modelStack.Scale(1, 3, 1);
		//modelStack.Rotate(0, 30, 0, 1);
		RenderMesh(meshList[GEO_LIMBS], true);
		modelStack.PopMatrix();

		//left feet
		modelStack.PushMatrix();
		modelStack.Translate(0, -45, 15);
		modelStack.Scale(10, 5, 20);
		RenderMesh(meshList[GEO_FEET], true);
		modelStack.PopMatrix();


		RenderMesh(meshList[GEO_PALM], true);
		modelStack.PopMatrix();




		RenderMesh(meshList[GEO_PALM], true);
		modelStack.PopMatrix();
	}



	modelStack.Rotate(-30 + 3, 0, 30 * 3, 1);
	//modelStack.Rotate( 3, 3, 0, 1);

	//ENTIRE EYES
	{
		modelStack.PushMatrix();
		modelStack.Translate(-5, 5, 15);
		modelStack.Scale(2, 2, 2);
		modelStack.Rotate(90, 0, 0, 1);
		RenderMesh(meshList[GEO_EYES], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(5, 5, 15);
		modelStack.Scale(2, 2, 2);
		modelStack.Rotate(90, 0, 0, 1);
		RenderMesh(meshList[GEO_EYES], true);
		modelStack.PopMatrix();
	}


	//ENTIRE MOUTH
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -10, 13);
		modelStack.Rotate(-90, -180, 360, 1);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_MOUTH], true);
		modelStack.PopMatrix();
	}

	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_HEAD], true);
	modelStack.PopMatrix();
}



void SceneObstacle::Init()
{
	InitAttributes();
	jumpHeight = 0;
	medKitRotate = 0;
	health = 5; // amount of 'hits' that player can take from police
	isHit = false;
	hitTimer = 0;

	gameStarted = false;
	gameEnd = false;
	restart = false;

	tutorial1 = false;
	textTimer = 0;

	surviveTimer = 60; // How long in seconds to win the game

	//EVERYTHING NEEDED, DON'T TOUCH
	{

		// Init VBO here
		Mtx44 projection;

		projection.SetToPerspective(90.f, 4.f / 3.f, 0.1f, 1000.f);
		projectionStack.LoadMatrix(projection);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		camera.Init(Vector3(4, 3, 3), Vector3(0, 3, 3.02), Vector3(0, 1, 0));

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

		//use our shader
		glUseProgram(m_programID);

		light[0].type = Light::LIGHT_DIRECTIONAL;
		light[0].position.Set(-900, 20, -900);
		light[0].color.Set(1, 1, 1);
		light[0].power = 1;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 3.5f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);


		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);

		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);

		//light[0].type = Light::LIGHT_SPOT;
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		//set background color
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

		// Make sure you pass uniform parameters after glUseProgram()
		glUniform1i(m_parameters[U_NUMLIGHTS], 1);

		//Enable depth test
		glEnable(GL_DEPTH_TEST);

		//Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	}



	// LOAD THE MODELS

	//Skybox
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//exosystem_ft.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//exosystem_bk.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//exosystem_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//exosystem_dn.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//exosystem_rt.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//exosystem_lf.tga");




	// Buildings
	meshList[GEO_BUILDING1] = MeshBuilder::GenerateOBJMTL("building 1", "OBJ//large_buildingA.obj", "OBJ//large_buildingA.mtl");
	meshList[GEO_BUILDING2] = MeshBuilder::GenerateOBJMTL("building 2", "OBJ//house_type12.obj", "OBJ//house_type12.mtl");
	meshList[GEO_POLICEHELMET] = MeshBuilder::GenerateOBJMTL("police helmet", "OBJ//blue_helmet.obj", "OBJ//blue_helmet.mtl");


	meshList[GEO_MEDKIT] = MeshBuilder::GenerateOBJMTL("med kit", "OBJ//FirstAid.obj", "OBJ//FirstAid.mtl");
	meshList[GEO_MEDKIT]->textureID = LoadTGA("Images//FirstAid.tga");


	// Health UI
	meshList[GEO_REDHEART] = MeshBuilder::GenerateQuad("red heart", Color(1, 1, 1), 1.f);
	meshList[GEO_REDHEART]->textureID = LoadTGA("ImageS//redhealth.tga");

	// Ground Mesh
	meshList[GEO_GROUNDMESH] = MeshBuilder::GenerateQuad("ground mesh", Color(1, 1, 1), 1.f);
	meshList[GEO_GROUNDMESH]->textureID = LoadTGA("Images//game1_floor.tga");

	// Enviromental objects
	meshList[GEO_GRASS] = MeshBuilder::GenerateQuad("grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS]->textureID = LoadTGA("Images//grass.tga");
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("police helmet", "OBJ//tree_blocks.obj", "OBJ//tree_blocks.mtl");


	// Text rendering
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//MS_gothic.tga");

	// For tutorial screen
	meshList[GEO_BLACKQUAD] = MeshBuilder::GenerateQuad("black quad", Color(0, 0, 0), 1.f);
	meshList[GEO_TEXTBG] = MeshBuilder::GenerateQuad("text bg", Color(1, 1, 1), 100);
	meshList[GEO_TEXTBG]->textureID = LoadTGA("Images//game1_UI.tga");

	// Police model
	meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 30, 30, 2);
	meshList[GEO_HEAD]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_HEAD]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_HEAD]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_HEAD]->material.kShininess = 1.f;


	meshList[GEO_LIMBS] = MeshBuilder::GenerateCylinder("STAR", Color(0, 0, 1), 30, 10, 5);
	meshList[GEO_LIMBS]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_LIMBS]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_LIMBS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_LIMBS]->material.kShininess = 1.f;

	meshList[GEO_PALM] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 30, 10, 0);
	meshList[GEO_PALM]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PALM]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_PALM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PALM]->material.kShininess = 1.f;


	meshList[GEO_FEET] = MeshBuilder::GenerateCube("STAR", Color(0.545098039215686, 0.270588235294117, 0.074509803921568), 1);
	meshList[GEO_FEET]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FEET]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FEET]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FEET]->material.kShininess = 1.f;

	meshList[GEO_EYES] = MeshBuilder::GenerateTorus("STAR", Color(0, 0, 0), 20, 20, 3, 2);
	meshList[GEO_EYES]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_EYES]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_EYES]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_EYES]->material.kShininess = 1.f;

	meshList[GEO_MOUTH] = MeshBuilder::GenerateHalfTorus("STAR", Color(0, 0, 0), 20, 20, 3, 2);
	meshList[GEO_MOUTH]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MOUTH]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_MOUTH]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MOUTH]->material.kShininess = 1.f;


	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}



void  SceneObstacle::Update(double dt)
{
	fps = 1.f / dt;




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
		if (Application::IsKeyPressed('L'))
			light[0].position.z -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('J'))
			light[0].position.z += (float)(LSPEED * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('I'))
			light[0].position.x -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('K'))
			light[0].position.x += (float)(LSPEED * dt);
		//GO DOWN
		if (Application::IsKeyPressed('O'))
			light[0].position.y -= (float)(LSPEED * dt);
		//GO UP
		if (Application::IsKeyPressed('P'))
			light[0].position.y += (float)(LSPEED * dt);

		if (Application::IsKeyPressed('A'))
			translate_x -= (float)(80 * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('S'))
			translate_z += (float)(80 * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('D'))
			translate_x += (float)(80 * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('W'))
			translate_z -= (float)(80 * dt);


		// Jumping
		if (Application::IsKeyPressed(VK_SPACE) && jumpHeight == 0 && !isJump && !gameEnd && gameStarted)
		{
			isJump = true;
		}
		if (isJump && jumpHeight < 30)
		{
			jumpHeight += (float)(45.f * dt);
		}
		else if (jumpHeight > 0)
		{
			isJump = false;
			jumpHeight -= (float)(45.f * dt);
			if (jumpHeight < 0)
			{
				jumpHeight = 0;
			}
		}

		// Police tracking
		for (int i = 0; i < obstaclePos.size(); i += 3)
		{
			if (obstacleType[i / 3] == POLICE && !gameEnd && gameStarted)
			{
				if (obstaclePos[i + 2] > camera.position.z + 0.5 && obstaclePos[i] + camera.position.x <= 120)
				{
					obstaclePos[i + 2] -= (float)(35 * dt);
				}
				else if (obstaclePos[i + 2] < camera.position.z - 0.5 && obstaclePos[i] + camera.position.x <= 120)
				{
					obstaclePos[i + 2] += (float)(35 * dt);
				}
			}
		}

		// Check collision for obstacles
		if (!isHit && !gameEnd)
		{
			for (int i = 0; i < obstaclePos.size(); i += 3)
			{
				switch (obstacleType[i / 3]) // Check which type of obstacle
				{
				case BUILDING_1:
					if (camera.position.x >= -19.5 - obstaclePos[i] && camera.position.x <= 19.5 - obstaclePos[i] &&
						camera.position.z >= -19.5 + obstaclePos[i + 2] && camera.position.z <= 19.5 + obstaclePos[i + 2])
					{
						isHit = true;
						hitTimer = 100;
						health = 0;
					}
					break;
				case BUILDING_2:
					if (camera.position.x >= -28 - obstaclePos[i] && camera.position.x <= 24 - obstaclePos[i] &&
						camera.position.z >= -23.5 + obstaclePos[i + 2] && camera.position.z <= 23.5 + obstaclePos[i + 2])
					{
						isHit = true;
						hitTimer = 100;
						health = 0;
					}
					break;
				case POLICE:
					if (camera.position.x >= -10 - obstaclePos[i] && camera.position.x <= 10 - obstaclePos[i] &&
						camera.position.y <= 20 &&
						camera.position.z >= -10 + obstaclePos[i + 2] && camera.position.z <= 10 + obstaclePos[i + 2])
					{
						isHit = true;
						hitTimer = 100;
						health--;
					}
					break;
				case MEDKIT:
					if (camera.position.x >= -7 - obstaclePos[i] && camera.position.x <= 7 - obstaclePos[i] &&
						camera.position.y <= 15 &&
						camera.position.z >= -7 + obstaclePos[i + 2] && camera.position.z <= 7 + obstaclePos[i + 2])
					{
						isHit = true;
						hitTimer = 100;
						if (health < 5)
						{
							health++;
						}
					}
				}
			}
		}
		else
		{
			hitTimer -= (float)(300 * dt);
			if (hitTimer <= 0)
			{
				hitTimer = 0;
				isHit = false;
			}
		}
	


		medKitRotate += (80 * dt);
		// FOR DEBUGGING
		//std::cout << "x: " << camera.position.x << std::endl;
		//std::cout << "y: " << camera.position.y << std::endl;
		//std::cout << "z: " << camera.position.z << std::endl;

		//for (int i = 3; i < 4; ++i)
		//{
		//	std::cout << "x: " << obstaclePos[i] << std::endl;
		//	std::cout << "z: " << obstaclePos[i + 2] << std::endl;
		//}

		if (textTimer > 0)
		{
			textTimer -= (222 * dt);
			if (textTimer < 0)
			{
				textTimer = 0;
			}
		}

		if (surviveTimer > 0 && gameStarted && !gameEnd)
		{
			surviveTimer -= (float)(1 * dt);
			if (surviveTimer < 0)
			{
				surviveTimer = 0;
			}
		}

		if (restart)
		{

			jumpHeight = 0;
			medKitRotate = 0;
			health = 5; // amount of 'hits' that player can take from police
			isHit = false;
			hitTimer = 0;

			gameStarted = false;
			gameEnd = false;
			restart = false;

			tutorial1 = false;
			textTimer = 0;

			surviveTimer = 60; // How long in seconds to win the game

			obstaclePos.clear();
			obstacleSize.clear();
			obstacleType.clear();

			InitAttributes(); // Rerandomise objects
			Application::setScene(0);
		}

		if (!gameEnd && gameStarted)
		{
			camera.Update(dt, jumpHeight, restart);
		}
	}
}


void  SceneObstacle::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneObstacle::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0 - camera.position.x, 0, -499 - camera.position.z);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();       

	modelStack.PushMatrix();
	modelStack.Translate(0 + camera.position.x, 0, -499 + camera.position.z);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();             

	modelStack.PushMatrix();
	modelStack.Translate(-499 + camera.position.x, 0, 0 + camera.position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();          

	modelStack.PushMatrix();
	modelStack.Translate(499 + camera.position.x, 0, 0 + camera.position.z);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();   

	modelStack.PushMatrix();
	modelStack.Translate(0 + camera.position.x, 499, 0 + camera.position.z);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();          

	modelStack.PushMatrix();
	modelStack.Translate(0 + camera.position.x, -499, 0 + camera.position.z);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();          
}

void SceneObstacle::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneObstacle::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float sizeX, float sizeY, float x, float y)
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
	modelStack.Scale(sizeX, sizeY, 1);


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

void SceneObstacle::RenderMeshOnScreen(Mesh* mesh, float x, float y, float
	sizex, float sizey)
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

void SceneObstacle::Render()
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
	
		


	// Skybox
	RenderSkybox();

	// Tutorial screen
	if (!gameStarted)
	{

		modelStack.PushMatrix();
		{
			modelStack.Translate(2, 0, 0);
			modelStack.Scale(20, 20, 20);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_BLACKQUAD], false);
		}
		modelStack.PopMatrix();

		if (!tutorial1)
		{
			RenderMeshOnScreen(meshList[GEO_TEXTBG], 40, 30, 0.9, 0.5);
			RenderTextOnScreen(meshList[GEO_TEXT], "Escape from the city!", Color(0, 1, 0), 3, 3, 4, 40);
			RenderTextOnScreen(meshList[GEO_TEXT], "Avoid the buildings", Color(0, 1, 0), 3, 3, 4, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "and police to survive.", Color(0, 1, 0), 3, 3, 4, 20);
			RenderTextOnScreen(meshList[GEO_TEXT], "(Press [Enter] to continue)", Color(1, 1, 1), 2, 2, 4, 10);
			if (Application::IsKeyPressed(VK_RETURN))
			{
				tutorial1 = true;
				textTimer = 100;
			}
		}
		else
		{
			RenderMeshOnScreen(meshList[GEO_TEXTBG], 40, 30, 0.9, 0.5);
			RenderTextOnScreen(meshList[GEO_TEXT], "[CONTROLS]", Color(0, 1, 0), 4, 4, 15, 45);
			RenderTextOnScreen(meshList[GEO_TEXT], "A & D - Move left and right", Color(0, 1, 0), 2, 2, 4, 40);
			RenderTextOnScreen(meshList[GEO_TEXT], "Spacebar - Jump", Color(0, 1, 0), 2.2, 2.2,  4, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "You are able to jump over the police", Color(0, 1, 0), 2, 2, 4, 20);
			RenderTextOnScreen(meshList[GEO_TEXT], "(Press [Enter] to begin)", Color(1, 1, 1), 2, 2, 4, 10);
			if (Application::IsKeyPressed(VK_RETURN) && textTimer == 0)
			{
				gameStarted = true;
			}
		}


	}

	// Ground mesh
	modelStack.PushMatrix();
	{

		modelStack.PushMatrix();
		{
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(12000, 200, 200);
			RenderMesh(meshList[GEO_GROUNDMESH], true);
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			modelStack.Translate(0, 0, -150);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(12000, 100, 100);
			RenderMesh(meshList[GEO_GRASS], true);
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			modelStack.Translate(0, 0, 150);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(12000, 100, 100);
			RenderMesh(meshList[GEO_GRASS], true);
		}
		modelStack.PopMatrix();

	}
	modelStack.PopMatrix();

	// Render obstacle
	for (int i = 0; i < obstaclePos.size(); i += 3)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(-obstaclePos[i], obstaclePos[i + 1], obstaclePos[i + 2]);
			modelStack.Scale(obstacleSize[i], obstacleSize[i + 1], obstacleSize[i + 2]);
			modelStack.Rotate(90, 0, 1, 0);
			if (obstacleType[i / 3] == BUILDING_1)
			{
				RenderMesh(meshList[GEO_BUILDING1], true);
			}
			else if (obstacleType[i / 3] == BUILDING_2)
			{
				RenderMesh(meshList[GEO_BUILDING2], true);
			}
			else if (obstacleType[i / 3] == POLICE)
			{
				RenderPolice();
			}
			else if (obstacleType[i / 3] == MEDKIT)
			{
				modelStack.Rotate(medKitRotate, 0, 1, 0);
				RenderMesh(meshList[GEO_MEDKIT], true);
			}
		}
		modelStack.PopMatrix();
	}

	// Render trees
	for (int i = 0; i < treePos.size(); i += 3)
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(-treePos[i], treePos[i + 1], treePos[i + 2]);
			modelStack.Scale(70, 50, 70);
			RenderMesh(meshList[GEO_TREE], true);
		}
		modelStack.PopMatrix();
	}

	// Print UI for health
	if (gameStarted)
	{
		for (int i = 0; i < health; ++i)
		{
			RenderMeshOnScreen(meshList[GEO_REDHEART], 6 + (5 * i), 55, 5, 5);
		}
	}
	
	// Win condition
	if (surviveTimer <= 0)
	{
		if (!gameEnd)
		{
			textTimer = 100;
			gameEnd = true;
		}

		RenderMeshOnScreen(meshList[GEO_TEXTBG], 40, 30, 0.9, 0.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "You escaped from", Color(0, 1, 0), 4, 4, 8, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "the police", Color(0, 1, 0), 4, 4, 8, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "(Press [Enter] to continue)", Color(1, 1, 1), 2.5, 2.5, 4, 15);
		if (Application::IsKeyPressed(VK_RETURN) && textTimer <= 0)
		{
			jumpHeight = 0;
			medKitRotate = 0;
			health = 5; // amount of 'hits' that player can take from police
			isHit = false;
			hitTimer = 0;

			gameStarted = false;
			gameEnd = false;
			restart = false;

			tutorial1 = false;
			textTimer = 0;

			surviveTimer = 60; // How long in seconds to win the game

			obstaclePos.clear();
			obstacleSize.clear();
			obstacleType.clear();

			InitAttributes(); // Rerandomise objects
			Application::setScene(5);
		}
	}

	// Lose condition
	if (health <= 0)
	{
		gameEnd = true;

		RenderMeshOnScreen(meshList[GEO_TEXTBG], 40, 30, 0.9, 0.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "You have been arrested", Color(1, 0, 0), 3.5, 3.5, 4, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "by the police", Color(1, 0, 0), 4, 4, 4, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "(Press [R] to retry)", Color(1, 1, 1), 2.5, 2.5, 4, 20);


		if (Application::IsKeyPressed('R'))
		{
			restart = true;
			camera.Update(1, jumpHeight, restart);
		}
	}

	// Print timer
	if (gameStarted)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "DAY 4", Color(0, 1, 0), 3, 3,  55, 55);
		RenderMeshOnScreen(meshList[GEO_TEXTBG], 60, 0, 0.4, 0.1);
		std::ostringstream pwt;
		pwt.str("");
		pwt << round(surviveTimer);
		RenderTextOnScreen(meshList[GEO_TEXT], "Time left:" + pwt.str(), Color(0, 1, 0), 3, 3, 43, 0);
	}

}

void  SceneObstacle::Exit()
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