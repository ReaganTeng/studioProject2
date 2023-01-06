#include "QnA.h"
#include <iostream>
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"

QnA::QnA()
{
	section_1_color = Color(0, 0.50196078431372549019607843137255, 0);
	section_2_color = Color(0, 0.50196078431372549019607843137255, 0);
	section_3_color = Color(0, 0.50196078431372549019607843137255, 0);

	float range = 5;
	headline_section_y = 20;
	answer1_section_y = headline_section_y - range;
	answer2_section_y = answer1_section_y - range;
	answer3_section_y = answer2_section_y - range;


	this -> Question_Text = Question_Text;
	this -> Answer_1 = Answer_1;
	this -> Answer_2 = Answer_2;
	this -> Answer_3 = Answer_3;
	this -> correct_answer = correct_answer;

	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//generate default VAO for now
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);
		//load vertex and fragment shaders
		m_programID = LoadShaders("Shader//Texture.vertexshader",
			"Shader//Text.fragmentshader");
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
		//Enable depth test
		glEnable(GL_DEPTH_TEST);
		//Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	}

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,
		16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//MS_gothic.tga");
}

void QnA::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

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

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void QnA::setQuestion(std::string q, std::string a1,
	std::string a2, std::string a3, std::string correct_ans)
{
	Question_Text = q;
	Answer_1 = a1;
	Answer_2 = a2;
	Answer_3 = a3;
	
	correct_answer = correct_ans;
}

void QnA::RenderQuestion(float section_x, float section_y)
{
	float coordinate_y[50];
	std::string s[50];
	float word_size = 1.4;
	std::stringstream ss(Question_Text);

	//RENDER THE QUESTION
	for (int i = 0; i < 50;)
	{
		std::getline(ss, s[i], '\n');
			
		if (i == 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], s[i], Color(0, 0, 0), word_size, section_x, section_y);
		}
		else
		{
			if (i == 1)
			{
				coordinate_y[i] = section_y - 1.8;
				RenderTextOnScreen(meshList[GEO_TEXT], s[i], Color(0, 0, 0), word_size, section_x, coordinate_y[i]);
			}
			else
			{
				coordinate_y[i] = coordinate_y[i - 1] - 1.8;
				RenderTextOnScreen(meshList[GEO_TEXT], s[i], Color(0, 0, 0), word_size, section_x, coordinate_y[i]);
			}
		}
		i++;
	}

	float corx = 3;
	
	RenderTextOnScreen(meshList[GEO_TEXT], "CLICK ONE", Color(0, 0.50196078431372549019607843137255, 0), 
		2, corx, headline_section_y);
	RenderTextOnScreen(meshList[GEO_TEXT], Answer_1, section_1_color, 2, corx, answer1_section_y);
	RenderTextOnScreen(meshList[GEO_TEXT], Answer_2, section_2_color, 2, corx, answer2_section_y);
	RenderTextOnScreen(meshList[GEO_TEXT], Answer_3, section_3_color, 2, corx, answer3_section_y);
	

}

void QnA::Update(int& warning, int& idx)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	float posX = x / w * 80; 
	float posY = 60 - (y / h * 60);


	//change the colour when highlighted
	if (posX < 25)
	{
		if (posY < answer1_section_y + 1.7 && posY > answer1_section_y - 1.7)
		{
			section_1_color = Color(1, 0, 0);
		}
		else
		{
			section_1_color = Color(0, 0.50196078431372549019607843137255, 0);
		}

		if (posY < answer2_section_y + 2 && posY > answer2_section_y - 2)
		{
			section_2_color = Color(1, 0, 0);
		}
		else
		{
			section_2_color = Color(0, 0.50196078431372549019607843137255, 0);
		}

		if (posY < answer3_section_y + 2 && posY > answer3_section_y - 2)
		{
			section_3_color = Color(1, 0, 0);
		}
		else
		{
			section_3_color = Color(0, 0.50196078431372549019607843137255, 0);
		}
	}
	else
	{
		section_1_color = Color(0, 0.50196078431372549019607843137255, 0);
		section_2_color = Color(0, 0.50196078431372549019607843137255, 0);
		section_3_color = Color(0, 0.50196078431372549019607843137255, 0);

	}

	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		if (posX < 25)
		{
			if (posY < answer1_section_y + 1.7 && posY > answer1_section_y - 1.7)
			{
				if (Answer_1 == correct_answer)
				{
					idx += 1;
				}
				else
				{
					warning += 1;
				}
			}
			else if (posY < answer2_section_y + 1.7 && posY > answer2_section_y - 1.7)
			{
				if (Answer_2 == correct_answer)
				{
					idx += 1;
				}
				else
				{
					warning += 1;
				}
			}
			else if (posY < answer3_section_y + 1.7 && posY > answer3_section_y - 1.7)
			{
				if (Answer_3 == correct_answer)
				{
					idx += 1;
				}
				else
				{
					warning += 1;
				}
			}
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
	
}





QnA::~QnA()
{
}

