#include <iostream>										
#include <memory>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
//#include "imgui.h"
//#include "imgui-SFML.h"

sf::Font myFont;
int wWidth = 800;
int wHeight = 600;

sf::Text text("Sample Text", myFont, 24);

void setWindow(int& Width, int& Heigth);

void loadFromFile(const std::string& fileName, class circles& circleShape, class rectangles& rectangleShape);

class shapesCreate {

	//Circle
	float m_circleRadius = 25.f, m_circleSpeedX = 1.f, m_circleSpeedY = 1.f;
	int m_circleSegments = 32;
	sf::Text m_circleString, m_rectangleString;
	sf::CircleShape m_circle;


	//Rectangle
	float m_rx = 100.f, m_ry = 100.f, m_rectangleSpeedX = 1.f, m_rectangleSpeedY = 1.f;
	sf::Vector2f m_rSize;
	sf::RectangleShape m_rect;


public:
	shapesCreate() {}

	//Name, posXY, circleSpeedXY, RGB, raio, segmentos
	shapesCreate(std::string text = "TEXT", float posX = 80.f, float posY = 100.f, float circleSpeedX = 1.f, float circleSpeedY = 1.f, int R = 0, int G = 0, int B = 0, float circleRadius = 50.f, int circleSegments = 32)
		: m_circleRadius(circleRadius)
		, m_circleSegments(circleSegments)
		, m_circleSpeedX(circleSpeedX)
		, m_circleSpeedY(circleSpeedY)
		, m_circle(circleRadius, circleSegments)
		, m_circleString(text, myFont, 18)
	{
		m_circle.setPosition(posX, posY);  //Colocando a posição do circulo em cima na esquerda
		m_circle.setFillColor(sf::Color(R, G, B));
	}
	//Name, PosXY, velocityXY, RGB, WidhHigh
	shapesCreate(
		std::string text,
		float posX,
		float posY,
		float rectangleSpeedX,
		float rectangleSpeedY,
		int R, int G, int B,
		float rx,
		float ry)
		: m_rx(rx)
		, m_ry(ry)
		, m_rectangleSpeedX(rectangleSpeedX)
		, m_rectangleSpeedY(rectangleSpeedY)
		, m_rSize(rx, ry)
		, m_rect(m_rSize)
		, m_rectangleString(text, myFont, 18)
	{
		m_rect.setPosition(posX, posY);
		m_rect.setFillColor(sf::Color(R, G, B));
	}

	void circleUpdate() {
		// Update the circle's position
		m_circle.setPosition(m_circle.getPosition().x + m_circleSpeedX, m_circle.getPosition().y + m_circleSpeedY);
		if ((m_circle.getPosition().x < 0) || (m_circle.getPosition().x + 2 * m_circleRadius > wWidth)) m_circleSpeedX *= -1.f;
		if ((m_circle.getPosition().y < 0) || (m_circle.getPosition().y + 2 * m_circleRadius > wHeight)) m_circleSpeedY *= -1.f;
		centerTextCircle(m_circleString, m_circle);
	}

	void centerTextCircle(sf::Text& text, const sf::CircleShape& circle) {
		float circleCenterX = circle.getPosition().x + circle.getRadius();
		float circleCenterY = circle.getPosition().y + circle.getRadius();

		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

		text.setPosition(circleCenterX, circleCenterY);
	}

	void drawCircle(sf::RenderWindow& window) {
		window.draw(m_circle);
		window.draw(m_circleString);
	}

	void rectUpdate() 
	{
		m_rect.setPosition(m_rect.getPosition().x + m_rectangleSpeedX, m_rect.getPosition().y + m_rectangleSpeedY);
		if ((m_rect.getPosition().x < 0) || (m_rect.getPosition().x + m_rx > wWidth)) m_rectangleSpeedX *= -1.f;
		if ((m_rect.getPosition().y < 0) || (m_rect.getPosition().y + m_ry > wHeight)) m_rectangleSpeedY *= -1.f;

		centerTextRect(m_rectangleString, m_rect);
	}

	void centerTextRect(sf::Text& text, const sf::RectangleShape& rect) 
	{
		float RectCenterX = rect.getPosition().x + rect.getSize().x / 2.f;
		float RectCenterY = rect.getPosition().y + rect.getSize().y / 2.f;

		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

		text.setPosition(RectCenterX, RectCenterY);
	}

	void drawRect(sf::RenderWindow& window)
	{
		window.draw(m_rect);
		window.draw(m_rectangleString);
	}
};

class circles
{
	std::string m_circle = "Circle";

	std::vector<shapesCreate> m_vecCircles;

public:
	circles() {}

	circles(const std::string& name)
		:m_circle(name)
	{
	}

	void addCircle(const shapesCreate& circle)
	{
		m_vecCircles.push_back(circle);
	}

	void vecCircleUpdate()
	{
		for (auto& s : m_vecCircles)
		{
			s.circleUpdate();
		}
	}

	void vecCircleDraw(sf::RenderWindow& window)
	{
		for (auto& s : m_vecCircles)
		{
			s.drawCircle(window);
		}
	}

};

class rectangles
{
	std::string m_rectangle = "Rectangle";

	std::vector<shapesCreate> m_vecRect;


public:
	rectangles(){}

	rectangles(const std::string& name)
		:m_rectangle(name)
	{}

	void addRect(const shapesCreate& rect)
	{
		m_vecRect.push_back(rect);
	}

	void vecRectUpdate()
	{
		for (auto& s : m_vecRect)
		{
			s.rectUpdate();
		}
	}

	void vecRectDraw(sf::RenderWindow& window)
	{
		for (auto& s : m_vecRect)
		{
			s.drawRect(window);
		}
	}
};

int main()
{
	//Cria uma tela com a quantidade de pixels iguais a: w * h
	//Esquerda em cima � (0, 0) e direita em baixo � (w, h)
	//Tentar pegar Widht e Heght de um arquivo externo
	circles circleShape;
	rectangles rectangleShape;

	loadFromFile("arquivo.txt", circleShape, rectangleShape);
	setWindow(wWidth, wHeight);
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
	window.setFramerateLimit(60);//limitando o framerate pra evitar bugs
	sf::Clock deltaClock;//Essa variável nos da o tempo entre um frame e outro

	if (!myFont.loadFromFile("fonts/Square.ttf"))
	{

		std::cerr << "Could not load font!\n";
		exit(-1);

	}

	text.setPosition(0, wHeight - (float)text.getCharacterSize());
	char displayString[255] = "Sample Text";

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//ImGui::SFML::ProcessEvent(window, event);
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "Key pressed with code = " << event.key.code << "\n";

				if (event.key.code == sf::Keyboard::X)
				{
				}
			}
		}

		circleShape.vecCircleUpdate();
		rectangleShape.vecRectUpdate();
		
		window.clear(sf::Color::Black);

		circleShape.vecCircleDraw(window);
		rectangleShape.vecRectDraw(window);

		window.draw(text);

		window.display();
	}

	return 0;
}

void setWindow(int& Width, int& Heigth)
{
	wWidth = Width;
	wHeight = Heigth;
}

void loadFromFile(const std::string& fileName, class circles& circleShape, class rectangles& rectangleShape)
{
	std::fstream fin(fileName);

	std::string name, fontName;
	sf::Font font;
	int i;
	int w, h, R, G, B, tamanhoFont;
	float velocityX, velocityY, posX, posY, raio, rectW, rectH;
	while (fin >> name)
	{
		if (name == "Window")
		{
			fin >> w >> h;
			setWindow(w, h);
		}
		else if (name == "Font")
		{
			fin >> tamanhoFont >> R >> G >> B;
	
			text.setCharacterSize(tamanhoFont);
			text.setFillColor(sf::Color(R, G, B));
		}
		else if (name == "Circle")
		{
			//Name, posXY, circleSpeedXY, RGB, raio, segmentos
			fin >> name >> posX >> posY >> velocityX >> velocityY >> R >> G >> B >> raio;
			circleShape.addCircle(shapesCreate(name, posX, posY, velocityX, velocityY, R, G, B, raio));
		}
		else if (name == "Rectangle")
		{
			//Name, PosXY, velocityXY, RGB, WidhHigh
			fin >> name >> posX >> posY >> velocityX >> velocityY >> R >> G >> B >> rectW >> rectH;
			rectangleShape.addRect(shapesCreate(name, posX, posY, velocityX, velocityY, R, G, B, rectW, rectH));
		}
		else
		{
			std::cerr << "Could not read the file";
			return;
		}
	}

}
