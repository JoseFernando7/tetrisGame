#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include<sstream>

using namespace sf;
using namespace std;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{
	int x, y;
}
a[4], b[4];

int figuras[7][4] = 
{
	1,3,5,7, //Palo
	2,4,5,7, //S
	3,5,4,6, //Z
	3,5,4,7, //T
	2,3,5,7, //L invertida
	3,5,7,6, //J
	2,3,4,5, //O
};

bool check()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
		{
			return false;
		}
		
		else if (field[a[i].y] [a[i].x])
		{
			return false;
		}
	}
	
	return true;
}

int main()
{
	srand(time(0));
	
	RenderWindow window(VideoMode(360, 550), "Tetris.exe");
	window.setFramerateLimit(60);
	
	Texture t, t1, t2;
	t.loadFromFile("images/tiles.png");
	t1.loadFromFile("images/Back.png");
	t2.loadFromFile("images/Cosa.png");
	
	Sprite sprite(t), background(t1), frame(t2);
	sprite.setTextureRect(IntRect(0, 0, 18, 18));
	
	int dx = 0;
	int score = 0;
	bool rotate = false;
	int colorNum = 1;
	float timer = 0, delay = 0.3;
	bool game_over = false;
	
	Clock clock;
	
	while(window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
			
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Up)
				{
					rotate = true;
				}
				
				else if (e.key.code == Keyboard::Left)
				{
					dx =- 1;
				}
				
				else if (e.key.code == Keyboard::Right)
				{
					dx = 1;
				}
			}
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			delay = 0.05;
		}
		
		//score int to string
		stringstream ss;
		ss << score;
		
		string scoreStr;
		ss >> scoreStr;
		
		//Texto de puntaje
		Font font;
		font.loadFromFile("PressStart2P.ttf");
		
		Text texto;
		texto.setFont(font);
		texto.setString(scoreStr);
		texto.setCharacterSize(25);
		texto.setFillColor(Color::White);
		texto.setPosition(227, 163);
		
		//Texto de Game Over
		Text gameOver;
		gameOver.setFont(font);
		gameOver.setString("Game Over");
		gameOver.setCharacterSize(30);
		gameOver.setOrigin(130, 50);
		gameOver.setFillColor(Color::White);
		gameOver.setPosition(360 / 2, (550 / 2) - 50);
		
		//Texto de puntaje final
		Text puntajeFinal;
		puntajeFinal.setFont(font);
		puntajeFinal.setString("Your Score");
		puntajeFinal.setCharacterSize(23);
		puntajeFinal.setOrigin(115, 0);
		puntajeFinal.setFillColor(Color::White);
		puntajeFinal.setPosition(360 / 2, 550 / 2);
		
		//Texto de enter
		Text enter;
		enter.setFont(font);
		enter.setString("END");
		enter.setCharacterSize(15);
		enter.setFillColor(Color::White);
		enter.setPosition(10, 530);
		
		// Para mover a izquierda y derecha
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		
		if (!check())
		{
			for (int i = 0; i < 4; i++)
			{
				a[i] = b[i];
			}
		}
		
		// Para rotar la figura
		if (rotate)
		{
			Point p = a[1]; // Este es el centro de la rotación
			
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			
			if (!check())
			{
				for (int i = 0; i < 4; i++)
				{
					a[i] = b[i];
				}
			}
		}
		
		// Tick
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}
			
			if (!check())
			{
				for (int i = 0; i < 4; i++)
				{
					field[b[i].y] [b[i].x] = colorNum;
				}
				
				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figuras[n][i] % 2;
					a[i].y = figuras[n][i] / 2;
				}
			}
			timer = 0; 
		}
		
		//Revisar Lineas
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j])
				{
					count++;
					//printf("field[%d][%d]\n", i, j);
				}
				field[k][j] = field[i][j];
				
				if (field[1][j])
				{
					game_over = true;
				}
			}
			
			if (count < N)
			{
				k--;
			}
			
			else if (count == N)
			{
				//printf("Linea\n");
				score += 100;
			}
			
		}
		
		dx = 0;
		rotate = false;
		delay = 0.3;
		
		// Para dibujar
		window.clear(Color::White);
		window.draw(background);
		window.draw(texto);
		
		if (game_over)
		{
			texto.setCharacterSize(30);
			texto.setPosition((360 / 2) - 15, (550 / 2) + 60);
			window.clear(Color::Black);
			window.draw(gameOver);
			window.draw(puntajeFinal);
			window.draw(texto);
			window.draw(enter);
		}
		
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				sprite.setTextureRect(IntRect(field[i][j]*18, 0, 18, 18));
				sprite.setPosition(j * 18, i * 18);
				sprite.setScale(1.2f, 1.2f);
				sprite.move(28, 31);
				
				if (!game_over)
				{
					window.draw(sprite);
				}
			}
		}
		
		for (int i = 0; i < 4; i++)
		{
			sprite.setTextureRect(IntRect(colorNum*18, 0, 18, 18));
			sprite.setPosition(a[i].x*18, a[i].y*18);
			sprite.setScale(1.2f, 1.2f);
			sprite.move(28, 31);
			
			if (!game_over)
			{
				window.draw(sprite);
			}
			
		}
		
		if (!game_over)
		{
			window.draw(frame);
		}
		
		
		window.display();
	}
	
	return 0;
}
