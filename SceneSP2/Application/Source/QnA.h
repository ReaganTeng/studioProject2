#pragma once
#include <string>
#include <sstream>
#include "Mesh.h"
#include "Application.h"
#include "MatrixStack.h"

class QnA
{
private:
	enum GEOMETRY_TYPE
	{
		GEO_TEXT,

		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_LIGHTENABLED,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL
	};

	MS modelStack, viewStack, projectionStack;
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];

	std::string Question_Text;
	std::string Answer_1;
	std::string Answer_2;
	std::string Answer_3;
	std::string correct_answer;

	float question_word_size;
	float answers_word_size;

	Color section_1_color;
	Color section_2_color;
	Color section_3_color;

	float headline_section_y;
	float answer1_section_y;
	float answer2_section_y;
	float answer3_section_y;
	
	

public:
	QnA();

	// Setter Function
	void setQuestion(std::string q, std::string a1,
			std::string a2, std::string a3, std::string correct_ans);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	// Function to ask questions
	void RenderQuestion(float section_x, float section_y);
	void Update(int& warning, int& idx);

	

	~QnA();
};

