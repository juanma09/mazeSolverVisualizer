// MazeSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameGraphics.h"
#include "MazeClass.h"

// Define colors
sf::Color ButtonC = sf::Color::Yellow;
sf::Color DisabledButtonC = sf::Color(255, 255, 0, 100);
sf::Color BackGround;

float w = 1920.f, h = 1080.f;

int gameLoop(maze m1)
{
	//Init game
	std::stringstream tss;
	float gridSizeF = (h - 200.f) / m1.sizeX();
	unsigned gridSizeU = (unsigned)(gridSizeF);
	float dt = 0.f;
	sf::Clock dtClock;
	sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("Fonts/SourceSansPro-Regular.ttf"))
	{
		return 1;
	}
	text.setCharacterSize(28);
	text.setFillColor(sf::Color::White);
	text.setFont(font);
	text.setPosition(20.f, 20.f);
	text.setString("TEST");

	//Init windows
	sf::RenderWindow window(sf::VideoMode(w, h), "Maze Solver Visualizer");
	sf::View view;
	view.setSize(w, h);
	view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
	float viewSpeed = 200.f;
	view.move(sf::Vector2f(-20, -20));

	//Init game elements
	

	int mapSize = m1.sizeX();
	std::vector<std::vector<sf::RectangleShape>> tilemap;
	std::vector<std::vector<sf::Text>> textTileMap;
	tilemap.resize(mapSize, std::vector<sf::RectangleShape>());
	textTileMap.resize(mapSize, std::vector<sf::Text>());

	const float buttonSize = 100.f;

	coord xy;

	for (int i = 0; i < mapSize; i++)
	{
		tilemap[i].resize(mapSize, sf::RectangleShape());
		textTileMap[i].resize(mapSize, sf::Text());
		for (int j = 0; j < mapSize; j++)
		{
			tss.str("");
			tilemap[i][j].setSize(sf::Vector2f(gridSizeF, gridSizeF));
			tilemap[i][j].setOutlineThickness(1.f);
			tilemap[i][j].setOutlineColor(sf::Color::Black);
			tilemap[i][j].setPosition(i * gridSizeF, j * gridSizeF);

			xy = { i, j };
			//tss << i << ", " << j;
			switch (m1.getElement(xy))
			{
			case 2:
				tilemap[i][j].setFillColor(sf::Color::Black);
				break;
			case 1:
				tilemap[i][j].setFillColor(sf::Color::Yellow);
				break;
			case 0:
				tilemap[i][j].setFillColor(sf::Color::White);
				break;
			case 3:
				tilemap[i][j].setFillColor(sf::Color::Green);
				break;
			default:
				break;
			}
			textTileMap[i][j].setFont(font);
			textTileMap[i][j].setCharacterSize(24);
			textTileMap[i][j].setFillColor(sf::Color::Red);
			textTileMap[i][j].setFont(font);
			textTileMap[i][j].setPosition(i * gridSizeF + 2, j * gridSizeF + 2);
			textTileMap[i][j].setString(tss.str());
		}
	}

	// Init Buttons

	sf::RectangleShape startB(sf::Vector2f(buttonSize * 4, buttonSize));
	startB.setPosition((w+(mapSize * gridSizeF)- startB.getSize().x)/2, 100.f);
	sf::Text startBText;
	startBText.setFont(font);

	sf::RectangleShape algoB(sf::Vector2f(buttonSize * 3, buttonSize));
	algoB.setPosition((w + 3 *(mapSize * gridSizeF) - 2* algoB.getSize().x) / 4, startB.getPosition().y + startB.getPoint(2).y + 100);
	sf::Text algoBText;
	algoBText.setFont(font);
	sf::Text algoText;
	algoText.setString("Algorithm: ");
	algoText.setFont(font);
	setText(algoText, algoB.getPosition() + sf::Vector2f(0, -50.f), 40);

	sf::RectangleShape speedB(sf::Vector2f(buttonSize * 3, buttonSize));
	speedB.setPosition((3 * w + (mapSize * gridSizeF) - 2 * speedB.getSize().x) / 4, startB.getPosition().y + startB.getPoint(2).y + 100);
	sf::Text speedBText;
	speedBText.setFont(font);
	sf::Text speedText;
	speedText.setFont(font);
	speedText.setString("Speed: ");
	setText(speedText, speedB.getPosition() + sf::Vector2f(0, -50.f), 40);


	sf::RectangleShape randomB(sf::Vector2f(buttonSize * 3, buttonSize));
	randomB.setPosition((3 * w + (mapSize * gridSizeF) - 2 * speedB.getSize().x) / 4, speedB.getPosition().y + speedB.getPoint(2).y + 100);
	sf::Text randomBText;
	randomBText.setFont(font);

	sf::RectangleShape clearB(sf::Vector2f(buttonSize * 3, buttonSize));
	clearB.setPosition((w + 3 * (mapSize * gridSizeF) - 2 * algoB.getSize().x) / 4, algoB.getPosition().y + startB.getPoint(2).y + 100);
	sf::Text clearBText;
	clearBText.setFont(font);

	sf::RectangleShape backB(sf::Vector2f(buttonSize*2, buttonSize/1.5f));
	backB.setPosition(w - backB.getSize().x, 0);
	sf::Text backBText;
	backBText.setFont(font);

	sf::RectangleShape saveB(sf::Vector2f(buttonSize * 2, buttonSize / 1.5f));
	saveB.setPosition(w - saveB.getSize().x, backB.getSize().y + 10);
	sf::Text saveBText;
	saveBText.setFont(font);

	
	sf::RectangleShape tileSelector(sf::Vector2f(gridSizeF, gridSizeF));
	tileSelector.setFillColor(sf::Color::Transparent);
	tileSelector.setOutlineThickness(1.f);
	tileSelector.setOutlineColor(sf::Color::Green);

	sf::Text pathLengthText;
	pathLengthText.setFont(font);
	setText(pathLengthText, startB.getPosition() + sf::Vector2f(0, (gridSizeF * mapSize) - startB.getPosition().y - 100), 40);


	bool selected = false, locked = false;
	coord path = m1.getLast();
	coord selectedCoord = { -1, -1 };
	float timerSolve = 0.f;
	float solveSpeed = .05f;
	int algorithm = 0, speedChoice = 2, pathLength = 0;
	int window_w, window_h;
	while (window.isOpen())
	{
		// Update dt
		dt = dtClock.restart().asSeconds();

		//Update mouse positions
		mousePosScreen = sf::Mouse::getPosition();
		mousePosWindow = sf::Mouse::getPosition(window);
		window.setView(view);
		mousePosView = window.mapPixelToCoords(mousePosWindow);
		if (mousePosView.x > -gridSizeF)
			mousePosGrid.x = mousePosView.x / gridSizeU;
		if (mousePosView.y > -gridSizeF)
			mousePosGrid.y = mousePosView.y / gridSizeU;
		window.setView(window.getDefaultView());

		// Update game elements
		if (mousePosGrid.x >= 0 && mousePosGrid.y >= 0 && mousePosGrid.x < mapSize && mousePosGrid.y < mapSize)
		{
			tileSelector.setPosition(mousePosGrid.x * gridSizeF, mousePosGrid.y * gridSizeF);
			tileSelector.setOutlineColor(sf::Color::Green);
		}
		else
		{
			tileSelector.setOutlineColor(sf::Color::Transparent);
		}

		//Update UI
		std::stringstream ss;
		ss << "Screen: " << mousePosScreen.x << " " << mousePosScreen.y << "\n"
			<< "Window: " << mousePosWindow.x << " " << mousePosWindow.y << "\n"
			<< "View: " << mousePosView.x << " " << mousePosView.y << "\n"
			<< "Grid: " << mousePosGrid.x << " " << mousePosGrid.y << "\n";
		text.setString(ss.str());

		std::stringstream buttonStr;
		switch (algorithm)
		{
		case 0:
			buttonStr << "BFS";
			break;
		case 1:
			buttonStr << "DFS";
			break;
		case 2:
			buttonStr << "Random";
			break;
		case 3:
			buttonStr << "A*";
		default:
			break;
		}
		algoBText.setString(buttonStr.str());

		buttonStr.str("");
		if (m1.solved)
			buttonStr << "Restart";
		else
			buttonStr << "Start";
		startBText.setString(buttonStr.str());
		if (!m1.solved && locked)
			setOriginCenterRect(startB, startBText, sf::Color(240, 100, 0, 100));
		else
			setOriginCenterRect(startB, startBText, sf::Color(240, 100, 0));

		buttonStr.str("");
		switch (speedChoice)
		{
		case 0: // x.25
			buttonStr << "x.25";
			solveSpeed = 0.2f / 0.5f;
			break;
		case 1: // x.5
			buttonStr << "x.5";
			solveSpeed = 0.2f / 1.f;
			break; 
		case 2: // x1
			buttonStr << "x1";
			solveSpeed = 0.2f / 2.f;
			break; 
		case 3: // x2
			buttonStr << "x2";
			solveSpeed = 0.2f / 4.f;
			break;
		case 4: // x4
			buttonStr << "x4";
			solveSpeed = 0.f;
			break;
		}
		speedBText.setString(buttonStr.str());
		setOriginCenterRect(speedB, speedBText);

		buttonStr.str("");
		buttonStr << "Back";
		backBText.setString(buttonStr.str());
		setOriginCenterRect(backB, backBText);

		buttonStr.str("");
		buttonStr << "Random";
		randomBText.setString(buttonStr.str());

		buttonStr.str("");
		buttonStr << "Clear";
		clearBText.setString(buttonStr.str());

		buttonStr.str("");
		buttonStr << "Save";
		saveBText.setString(buttonStr.str());

		buttonStr.str("");
		if (m1.solved)
		{
			if (pathLength)
			{
				buttonStr << "The path is " << pathLength << " steps long." << std:: endl << m1.getVisitedSize() << " nodes were visited.";
			}
			if (m1.getCur().x == -1)
			{
				buttonStr << "ERROR";
			}
		}
		pathLengthText.setString(buttonStr.str());

		if (!locked)
		{
			setOriginCenterRect(randomB, randomBText);
			setOriginCenterRect(clearB, clearBText);
			setOriginCenterRect(algoB, algoBText);
			setOriginCenterRect(saveB, saveBText);
		}
		else
		{
			setOriginCenterRect(randomB, randomBText, DisabledButtonC);
			setOriginCenterRect(clearB, clearBText, DisabledButtonC);
			setOriginCenterRect(algoB, algoBText, DisabledButtonC);
			setOriginCenterRect(saveB, saveBText, DisabledButtonC);
		}

		

		//Events
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				if (!locked)
				{
					m1.saveTXT();
				}
				window.close();
				return -1;
			}

			if (e.type == sf::Event::Resized)
			{

				sf::Vector2u size = window.getSize();
				float  heightRatio = h / w;
				float  widthRatio = w / h;
				if (size.y * widthRatio <= size.x)
				{
					size.x = size.y * widthRatio;
				}
				else if (size.x * heightRatio <= size.y)
				{
					size.y = size.x * heightRatio;
				}
				window.setSize(size);
			}


			if (!locked)
			{
				if (mousePosView.x > 0 && mousePosView.y > 0)
				{
					if (mousePosGrid.x >= 0 && mousePosGrid.y >= 0 && mousePosGrid.x < mapSize && mousePosGrid.y < mapSize)
					{
						coord cur = { mousePosGrid.x, mousePosGrid.y };
						if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
						{
							if (m1.getElement(cur) == 0)
							{
								tilemap[mousePosGrid.x][mousePosGrid.y].setFillColor(sf::Color::Black);
								m1.setElement(cur, 2);
							}
						}
						if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
						{
							if (m1.getElement(cur) == 2)
							{
								tilemap[mousePosGrid.x][mousePosGrid.y].setFillColor(sf::Color::White);
								m1.setElement(cur, 0);
							}
						}
						if (checkSwap(e))
						{
							if (!selected)
							{
								selectedCoord = { (int)mousePosGrid.x , (int)mousePosGrid.y };
								selected = true;
							}
							else
							{
								if (m1.getElement(cur) == 3)
									m1.setFirst(selectedCoord);
								if (m1.getElement(cur) == 1)
									m1.setLast(selectedCoord);

								if (m1.getElement(selectedCoord) == 3)
									m1.setFirst(cur);
								if (m1.getElement(selectedCoord) == 1)
									m1.setLast(cur);

								m1.swap(selectedCoord, cur);
								path = m1.getLast();
								selected = false;
							}
						}
					}
				}
			}
			
			// Buttons
			if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
			{
				if (collides(mousePosView, algoB) && !locked)
				{
					algorithm += 1;
					algorithm %= 4;
				}

				if (collides(mousePosView, startB))
				{
					if (m1.solved)
					{
						m1.Restart();
						m1.solved = false;
						locked = false;
						path = m1.getLast();
						pathLength = 0;
					}
					else
					{
						if (!locked)
						{
							m1.saveTXT();
							m1.setFirst(m1.getFirst());
						}
						locked = true;
					}
				}

				if (collides(mousePosView, speedB))
				{
					speedChoice += 1;
					speedChoice %= 5;
				}

				if (collides(mousePosView, backB))
				{
					return 2;
				}

				if (collides(mousePosView, randomB) && !locked)
				{
					m1.generateRandom();
				}

				if (collides(mousePosView, clearB) && !locked)
				{
					m1.clearMaze();
				}

				if (collides(mousePosView, saveB) && !locked)
				{
					m1.saveNewTXT();
					return 2;
				}
			}
			
		}
		//Update
		if (locked && !m1.solved)
		{
			timerSolve += dt;
			if (timerSolve > solveSpeed/2)
			{
				timerSolve = 0.f;
				//cur = m1.getPath(cur);
				switch (algorithm)
				{
				case 0:
					m1.solveBfs();
					break;
				case 1:
					m1.solveDfs();
					break;
				case 2:
					m1.solveRandom();
					break;
				case 3:
					m1.solveAStar();
					break;
				default:
					break;
				}
			}
		}
		if (m1.solved && m1.getCur().x != -1)
		{
			timerSolve += dt;
			if (timerSolve > solveSpeed)
			{
				timerSolve = 0.f;
				if (!(path == m1.getFirst()))
				{
					path = m1.getPath(path);
					pathLength++;
				}

			}
		}
		// Update Grid
		for (int x = 0; x < mapSize; x++)
		{
			for (int y = 0; y < mapSize; y++)
			{
				xy = { x, y };
				switch (m1.getElement(xy))
				{
				case 2:
					tilemap[x][y].setFillColor(sf::Color::Black);
					break;
				case 1:
					tilemap[x][y].setFillColor(sf::Color::Yellow);
					break;
				case 0:
					tilemap[x][y].setFillColor(sf::Color::White);
					break;
				case -1:
					tilemap[x][y].setFillColor(sf::Color::Red);
					break;
				case -2:
					tilemap[x][y].setFillColor(sf::Color::Blue);
					break;
				case 3:
					tilemap[x][y].setFillColor(sf::Color::Green);
					break;
				default:
					break;
				}
				if (selected)
				{
					tilemap[selectedCoord.x][selectedCoord.y].setFillColor(sf::Color::Magenta);
				}
			}
		}
		// Update Input
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //Left
		//{
		//	view.move(viewSpeed * dt, 0.f);
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) //Up
		//{
		//	view.move(0.f, viewSpeed * dt);
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //Down
		//{
		//	view.move(0.f, -viewSpeed * dt);
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //Right
		//{
		//	view.move(-viewSpeed * dt, 0.f);
		//}

		// Render
		window.clear(sf::Color(200, 200, 200, 200));
		window.setView(view);

		// Render game elements
		for (int x = 0; x < mapSize; x++)
		{
			for (int y = 0; y < mapSize; y++)
			{
				window.draw(tilemap[x][y]);
				window.draw(textTileMap[x][y]);
			}
		}
		window.draw(tileSelector);

		//Render Buttons
		window.draw(algoB);
		window.draw(algoBText);
		window.draw(algoText);
		window.draw(startB);
		window.draw(startBText);
		window.draw(speedB);
		window.draw(speedBText);
		window.draw(speedText);
		window.draw(backB);
		window.draw(backBText);
		window.draw(randomB);
		window.draw(randomBText);
		window.draw(clearB);
		window.draw(clearBText);
		window.draw(saveB);
		window.draw(saveBText);
		window.draw(pathLengthText);
		

		window.setView(window.getDefaultView());

		// Render ui
		//window.draw(text);
		window.display();
	}

	return 0;
}

int main()
{
	srand(time(NULL));
	int again = 2;
	int size = 20;
	while (again == 2)
	{
		size = mainMenu();
		if (size > -1)
		{
			maze m1(size);
			//m1.generateRandom();
			again = gameLoop(m1);
		}
		else if (size == -2)
		{
			std::string filen = "Mazes/" + fileSelector();
			if (filen == "Mazes/") continue;

			maze m1(filen);
			again = gameLoop(m1);
		}
		else
		{
			break;
		}
	}
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
