#include "mini_game_2.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"
#include <sstream>
#include <ctime>
#include "QnA.h"
//TAKE NOTE OF VARIETY, DON'T JUST MAKE IT GUESS THE PASSWORD GAME

//void merge(vector <QnA>& data, int first, int middle, int last, int sort_what)

void generateSetOfNumbers(int arr[], int n)
{
	int p[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

	//shuffle p
	for (int i = 11; i > 0; --i)
	{
		//get swap index
		int j = rand() % i;
		//swap p[i] with p[j]
		int temp = p[i];
		p[i] = p[j];
		p[j] = temp;
	}

	//copy first n elements from p to arr
	for (int i = 0; i < n; ++i)
	{
		arr[i] = p[i];
	}
}

mini_game_2::mini_game_2()
{
	
}



void mini_game_2::Init()
{
	restart();

	// Init VBO here
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//EVERYTHING NEEDED, DON'T TOUCH
	{
		

		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(0, 20, 0);
		light[0].color.Set(1, 1, 1);
		light[0].power = 10.f;
		light[0].kC = 0.5f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.4f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 1.f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);

		//set background color
		glClearColor(0.0f, 1.0f, 1.0f, 0.0f);

		//generate default VAO for now
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//load vertex and fragment shaders
		m_programID = LoadShaders("Shader//Texture.vertexshader",
			"Shader//Text.fragmentshader");

		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
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

		// Get a handle for our "colorTexture" uniform
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		m_parameters[U_TEXT_ENABLED] =
			glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
			"textColor");

		//use our shader
		glUseProgram(m_programID);

		// Make sure you pass uniform parameters after glUseProgram()
		glUniform1i(m_parameters[U_NUMLIGHTS], 1);

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

		//Enable depth test
		glEnable(GL_DEPTH_TEST);

		//Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	}

	//LOAD THE MOPDELS
	{
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("lightball", Color(0, 0, 0), 1.0f);

		meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 50, 50, 1.0f);

		//GenerateQuad(const std::string& meshName, Color color, float length = 1.f);
		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_TOP]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_up.tga");
		meshList[GEO_TOP]->position.Set(0, scale_translate_skybox / 2, 0);

		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_dn.tga");
		meshList[GEO_BOTTOM]->position.Set(0, -scale_translate_skybox / 2, 0);

		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_LEFT]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_lf.tga");
		meshList[GEO_LEFT]->position.Set(scale_translate_skybox / 2, 0, 0);

		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_RIGHT]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_rt.tga");
		meshList[GEO_RIGHT]->position.Set(-scale_translate_skybox / 2, 0, 0);

		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_BACK]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_bk.tga");
		meshList[GEO_BACK]->position.Set(0, 0, scale_translate_skybox / 2);

		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_FRONT]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//zeus_ft.tga");
		meshList[GEO_FRONT]->position.Set(0, 0, -scale_translate_skybox / 2);

		meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("front", (1.f, 1.f, 1.f), 1.f);
		meshList[GEO_GROUND]->textureID = LoadTGA("Image//SceneMain//indoors-skyboxes//Carpet.tga");

		meshList[GEO_PLAYERSPHERE] = MeshBuilder::GenerateSphere("playersphere", Color(1, 1, 1), 50, 50, 1.0f);

		meshList[GEO_DESK] = MeshBuilder::GenerateOBJMTL("desk",
			"OBJ//stall.obj", "OBJ//stall.mtl");
		meshList[GEO_DESK] -> position.Set(-50, 1, 0);
		

		meshList[GEO_COMPUTER_CASE] = MeshBuilder::GenerateOBJMTL("computer_case",
			"OBJ//SceneMain//com_case.obj", "OBJ//SceneMain//com_case.mtl");
		meshList[GEO_COMPUTER_CASE]->textureID = LoadTGA("OBJ//SceneMain//com_case.tga");

		meshList[GEO_MONITOR] = MeshBuilder::GenerateOBJMTL("monitor",
			"OBJ//SceneMain//monitor.obj", "OBJ//SceneMain//monitor.mtl");
		meshList[GEO_MONITOR]->textureID = LoadTGA("OBJ//SceneMain//monitor.tga");

		meshList[GEO_HOUSE] = MeshBuilder::GenerateOBJMTL("house",
			"OBJ//SceneMain//house.obj", "OBJ//SceneMain//house.mtl");
		meshList[GEO_HOUSE]->textureID = LoadTGA("OBJ//SceneMain//Stone_tiled_homemade_simple.tga");

		meshList[GEO_COMPUTER] = MeshBuilder::GenerateQuad("computer", Color(1, 1, 1), 1.f);
		meshList[GEO_COMPUTER]->textureID = LoadTGA("Image//ComputerScreen.tga");
	}

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,
		16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//MS_gothic.tga");

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}

void mini_game_2::Update(double dt)
{
	static bool bLButton = false;
	if (!bLButton && Application::IsMousePressed(0))
	{
		bLButton = true;
		tutorial_page_1 = !tutorial_page_1;
	}

	static bool access_computer = false;
	if (tutorial_page_1 == false)
	{
		if (camera.position.x - meshList[GEO_DESK]->position.x <= 28 && camera.position.z < 18.8 && camera.position.z > -18.8)
		{
			if (game_over == false && win_mode == false)
			{
				
				if (!access_computer && Application::IsKeyPressed('X'))				{
					access_computer = true;
					password_game = !password_game;
				}
				if (access_computer && !Application::IsKeyPressed('X'))
				{
					access_computer = false;
				}
				

				//RenderMeshOnScreen(Mesh * mesh, int x, int y, int sizex, int sizey)
				// RenderMeshOnScreen(meshList[GEO_PAPER], 15, 30, 50, 55);
				float specific_x = 15;
				float specific_y = 30;

				float difference = 2;
				float x_axis = 50;

				//Converting Viewport space to UI space
				double x, y;
				Application::GetCursorPos(&x, &y);
				unsigned w = Application::GetWindowWidth();
				unsigned h = Application::GetWindowHeight();
				float posX = x / w * 80; //convert (0,800) to (0,80)
				float posY = 60 - (y / h * 60); //convert (600,0) to (0,60)

				if (question_number == 1)
				{
					card_1.Update(warnings, question_number);
				}
				else if (question_number == 2)
				{
					card_2.Update(warnings, question_number);
				}
				else if (question_number == 3)
				{
					card_3.Update(warnings, question_number);
				}
				else if (question_number == 4)
				{
					win_mode = true;
				}
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

			

			if (Application::IsKeyPressed('5'))
			{
				//to do: switch light type to POINT and pass the information to shader
				light[0].type = Light::LIGHT_POINT;
			}
			else if (Application::IsKeyPressed('6'))
			{
				//to do: switch light type to DIRECTIONAL and pass the information to shader
				light[0].type = Light::LIGHT_DIRECTIONAL;
			}
			else if (Application::IsKeyPressed('7'))
			{
				//to do: switch light type to SPOT and pass the information to shader
				light[0].type = Light::LIGHT_SPOT;
			}

		

			
		}
	}
	

	if (warnings == 3)
	{
		game_over = true;
	}

	//LOSE CONDITION
	if (game_over == true )
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			Application::setScene(0);
			bLButton = false;
			restart();
		}
	}
	//WIN CONDITION
	else if (win_mode == true)
	{
		//money_amount = 20000;
		Application::setScene(3);
		bLButton = false;
		restart();
		
	}

	if (password_game == false && tutorial_page_1 == false)
	{
		camera.Update(dt);
	}

	
}

void mini_game_2::RenderMesh(Mesh* mesh, bool enableLight)
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

void mini_game_2::restart()
{
	srand((unsigned)time(NULL));

	game_over = false;

	tutorial_page_1 = true;

	win_mode = false;


	//Andy like to use birtdnj (rough length)
	//, and does some investment
	card1.setQuestion(
		"Name:Andy Ting Chiang\nBirthday:\n9th October 1978\n\nPASSWORDS USED\nInstagram:78Insta09gram10\nFacebook:TingFacebook78\nSnapchat:910AndySnapchat\n\nWhat is likely his DBS\npassword?",
		"*70DBS80&AT", "7&dB5781oq", "AndyTDBS091078", "AndyTDBS091078");
	card2.setQuestion(
		"John is an amateur investor,and does some investment.\nHowever,all of them failed due\nto his lack of\nunderstanding on how\ninvestment works.\nHe now has little money and\nneeds some to continue his\ninvestment.\n\nYou should pretend as what\nto scam John?"
		, "Investment Firm", "Loan Agency", "Businessman", "Loan Agency");
	card3.setQuestion(
		"Jenny wants the newest smartphone, but\nshe only has $100. Regardless,\nshe still surf the net and try to\nfind the cheapest and newest smartphone she can buy.\n\nWhat product and price should you\nscam the person with?",
		"Iphone(2010 model),$98", "Oppo(2020 model),$99", "HTC(2019 model),$120", "Oppo(2020 model),$99");
	card4.setQuestion(
		"You plan to make a fake competition\nto take other people's money.\nHowever,you do not want to make the details\ntoo personal as to not make it\nsuspicious.\n\nWhat details should you ask\nfrom them?",
		"Bank PIN", "Bank account number", "Mobile Number", "Bank account number");
	card5.setQuestion(
		"Name:Tony Fong Yu Xiang\nBirthday:\n26th July 1998\nName and type of Pet:\nPaddy(Dog)\n\nPASSWORDS USED\nSingpass:PaddyTF98\nFacebook:TonyPaddy2607TF\nOutlook:267PaddyTF98\n\nWhat is likely his POSB password?"
		, "TonyPOSBFong1998", "DogLoverTF", "Paddy26TonyFong07", "Paddy26TonyFong07");
	card6.setQuestion(
		"Recently, there has been an influx of trash in the\nbeaches of Singapore. Some national\ngovernmental organisations stepped up\nand do their part in mitigating\nthis issue via donations.You decide to take\nadvantage of the crisis.\n\nWhich environmental\norganisation will you\ndisguise as to make\nhigh profit?"
		, "NEA(Governmental)", "Nature Society(NGO)", "WWF(NGO)", "NEA(Governmental)");
	card7.setQuestion(
		"You decide to scam people\nby pretending to be\na local bank to tell\nthem about something.\n\nWhat starting number you\nshould use to mask\nas a local number?"
		, "+95", "+69", "+65", "+65");
	card8.setQuestion(
		"Sarah is a person who owed some money\nto a loan company.\nHowever,she realised that she cannot\npay back in time and needed to settle the\nloans without borrowing\nany more money.\n\nYou should pretend\nas what\nto scam Sarah?"
		, "Debt settler", "Lawyer", "Loan Agency", "Debt settler");
	card9.setQuestion(
		"You decide to scam some people of their money by\npersuading them to give you their credit\ncard details.\n\nWhat you can disguise as to make\nit more likely to trick\nthem?"
		, "Charity", "Bank company", "NGO", "Bank company");
	card10.setQuestion(
		"Zach wants to buy new $300 retail price Nike shoes\nwith maximum $200.He wants the cheapest option as\nlong as is the brand he wants.\n\nWhat product and price\nshould you scam\nthe person with?",
		"Nike($250)", "Nike($199)", "Adidas($150)", "Nike($199)");
	card11.setQuestion(
		"Recently,graduates are having trouble finding their\ndesired jobs. The jobs they want are either too much\nfor them or not up to their salary standards.\nYou decide to use this opportunity to scam them with\njob opportunities they\nfeel suit them\n\nWhat kind of job you think you can promote?",
		"Easy,high-paying", "Hard,high-paying", "Easy,low-paying", "Easy,high-paying");
	card12.setQuestion(
		"Mdm Huang is a lonely elderly woman who have no\nrelatives who are in Singapore.\nAll of the relatives are either the ones she\nnever met or have successful jobs overseas\n\nWho you can pretend as to make Madam Huang more\nlikely to be scammed?",
		"Job hirer", "Unknown relative", "Known relative", "Unknown relative");
	card13.setQuestion(
		"Name:Manny Teong Yu Xin\nBirthday:\n9th August 1999\n Past education:\nPunggol Secondary School\n\nPASSWORDS USED\nBigo:BigoPSSManny\nFacebook:FacebookPSSManny\nInstagram:InstaPSSManny\n\nWhat is likely his DBS password?"
		, "MannyTYXDBS", "MannyPSSDBS", "DBSPSSManny", "DBSPSSManny");


	text_x_coordinate = 0.5;
	word_size = 1.5;

	scale_translate_skybox = 1000;
	scale_ground = 300;
	password_game = false;



	int numbers[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };


	generateSetOfNumbers(numbers, 13);

	QnA random_cards[13] = { card1, card2, card3, card4,
		card5, card6, card7, card8, card9, card10, card11, card12, card13 };

	random_generator_1 = numbers[0];
	random_generator_2 = numbers[1];
	random_generator_3 = numbers[2];


	card_1 = random_cards[random_generator_1];
	card_2 = random_cards[random_generator_2];
	card_3 = random_cards[random_generator_3];

	question_number = 1;
	warnings = 0;

	

	//Vector3(0, 60, 88), Vector3(0, 60, 0), Vector3(0, 1, 0)
	camera.Init(Vector3(50, 25, 0), Vector3(0, 15, 0), Vector3(0, 1, 0));
}

void mini_game_2::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_TOP]->position.x, meshList[GEO_TOP]->position.y,
		meshList[GEO_TOP]->position.z);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_BOTTOM]->position.x, meshList[GEO_BOTTOM]->position.y,
		meshList[GEO_BOTTOM]->position.z);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_LEFT]->position.x, meshList[GEO_LEFT]->position.y,
		meshList[GEO_LEFT]->position.z);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();




	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_RIGHT]->position.x, meshList[GEO_RIGHT]->position.y,
		meshList[GEO_RIGHT]->position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_FRONT]->position.x, meshList[GEO_FRONT]->position.y,
		meshList[GEO_FRONT]->position.z);
	//modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_BACK]->position.x, meshList[GEO_BACK]->position.y,
		meshList[GEO_BACK]->position.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(scale_translate_skybox, scale_translate_skybox,
		0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();


}

void mini_game_2::RenderText(Mesh* mesh, std::string text, Color color)
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

void mini_game_2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void mini_game_2::RenderMeshOnScreen(Mesh* mesh, int x, int y, float
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
	//modelStack.Translate(40, 30, 0);
	modelStack.Translate(x, y, 0);
	//modelStack.Scale(10, 10, 0);
	modelStack.Scale(sizex, sizey, 0);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void mini_game_2::RenderPhysicalWorld()
{
	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();

	//RENDER LIGHTING
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

	

	//RENDER THE CAMERA AND PHYSICAL OBJECTS
	{
		//MAKE THE SKYBOX TRANSLATE CORRECTLY WITH CAMERA
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x, 0, camera.position.z);

		RenderSkybox();

		RenderMesh(meshList[GEO_PLAYERSPHERE], true);
		modelStack.PopMatrix();

		//RENDER THE LIGHTBALL
		modelStack.PushMatrix();
		modelStack.Translate(light[0].position.x - 50, light[0].position.y + 40,
			light[0].position.z);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 1, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(scale_ground, scale_ground, 0);
		RenderMesh(meshList[GEO_GROUND], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//modelStack.Translate(0, -5, 0);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(6, 6, 6);
		RenderMesh(meshList[GEO_HOUSE], true);
		modelStack.PopMatrix();

		//std::cout << meshList[GEO_HOUSE]->position.z;

		modelStack.PushMatrix();
		modelStack.Translate(meshList[GEO_DESK]->position.x, meshList[GEO_DESK]->position.y, meshList[GEO_DESK]->position.z);
		modelStack.Scale(45, 45, 45);
		RenderMesh(meshList[GEO_DESK], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-50, 25, 5);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(0.3f, 0.3f, 0.3f);
		RenderMesh(meshList[GEO_COMPUTER_CASE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-50, 18, -5);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(0.3, 0.3, 0.3);
		RenderMesh(meshList[GEO_MONITOR], true);
		modelStack.PopMatrix();
	}
}

void mini_game_2::RenderUI()
{
	if (tutorial_page_1 == false)
	{

		if (game_over == true)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "You Lose",
				Color(1, 1, 1), 5, 17, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], "Tap enter to retry",
				Color(1, 1, 1), 1.5, 30, 17);
		}

		if (camera.position.x - meshList[GEO_DESK]->position.x <= 28 && camera.position.z < 18.8 && camera.position.z > -18.8 && password_game == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Press x to open", Color(1, 0, 0), 3, 15, 15);
		}
		else if (camera.position.x - meshList[GEO_DESK]->position.x >= 25 && password_game == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Go near the desk", Color(1, 0, 0), 3, 15, 15);
		}

		if (game_over == false && win_mode == false)
		{
			if (password_game == true)
			{
				//RenderMeshOnScreen(Mesh * mesh, int x, int y, float sizex, float sizey);
				RenderMeshOnScreen(meshList[GEO_COMPUTER], 40, 25, 90, 55);
				//void QnA::RenderQuestion(float section_x, float section_y)
				float x = 5;
				float y = 43;
				if (question_number == 1)
				{
					card_1.RenderQuestion(x, y);
				}
				else if (question_number == 2)
				{
					card_2.RenderQuestion(x, y);
				}
				else if (question_number == 3)
				{
					card_3.RenderQuestion(x, y);
				}

				std::ostringstream ss2;
				ss2.str("");
				ss2 << "Question " << std::to_string(question_number);
				//RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 0, 0), wordsize, x, y);
				RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 0, 0), 3, 45, 13);

				std::ostringstream ss3;
				ss3.str("");
				ss3 << "Warnings " << std::to_string(warnings);
				RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 0, 0), 3, 45, 8);
			}

			RenderTextOnScreen(meshList[GEO_TEXT], "DAY 2", Color(0, 1, 0), 3, 55, 55);
			
			
		}
	}
	else
	{
		//TUTORIAL SCREEN
		RenderMeshOnScreen(meshList[GEO_QUAD], 40, 30, 90, 55);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press x to toggle on or off qna section",
			Color(0, 1, 0), 1.7, 5, 50);
		RenderTextOnScreen(meshList[GEO_TEXT], "Use WASD to move around",
			Color(0, 1, 0), 1.7, 5, 46);
		RenderTextOnScreen(meshList[GEO_TEXT], "Use arrow keys to look around",
			Color(0, 1, 0), 1.7, 5, 42);
		RenderTextOnScreen(meshList[GEO_TEXT], "Read the question (in black) carefully",
			Color(0, 1, 0), 1.7, 5, 38);
		RenderTextOnScreen(meshList[GEO_TEXT], "Click on one of the answers (in dark green)",
			Color(0, 1, 0), 1.7, 5, 34);
		RenderTextOnScreen(meshList[GEO_TEXT], "Answer wrongly will result in one warning",
			Color(0, 1, 0), 1.7, 5, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "Three warnings will result in game over",
			Color(0, 1, 0), 1.7, 5, 26);
		RenderTextOnScreen(meshList[GEO_TEXT], "Finish all 3 questions to proceed",
			Color(0, 1, 0), 1.7, 5, 22);
		RenderTextOnScreen(meshList[GEO_TEXT], "LEFT CLICK TO CONTINUE",
			Color(0, 1, 0), 1.7, 5, 12);
	}
}

void mini_game_2::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	RenderPhysicalWorld();
	RenderUI();
}

void mini_game_2::Exit()
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

mini_game_2::~mini_game_2()
{
}