#include "GameGraphics.h"
int w = 1920/ 1.7f, h = 1080 / 1.7f;


int mainMenu()
{
	std::ifstream mazeTxt("Maze.txt");
	int size = 0;
	if (mazeTxt.is_open() == true) {
		char c;
		while (mazeTxt.get(c)) {
			if (c == '\n')
			{
				break;
			}
			size++;
		}
		mazeTxt.close();
	}
	else
		return -1;
	// create the window
	sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosWindowF;
	sf::RenderWindow window(sf::VideoMode(w, h), "Main Menu", sf::Style::Titlebar | sf::Style::Close);
	sf::View view;
	view.setSize(w, h);
	view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);

	sf::Font font;
	if (!font.loadFromFile("Fonts/SourceSansPro-Regular.ttf"))
	{
		return 1;
	}
	const float buttonSize = 100.f;

	sf::Text title;
	title.setFont(font);
	title.setCharacterSize(80);
	title.setOutlineColor(sf::Color::Black);
	title.setOutlineThickness(5);
	// Init Buttons
	sf::RectangleShape startB(sf::Vector2f(buttonSize * 3, buttonSize));
	startB.setPosition(w/2 - startB.getSize().x/2, title.getPosition().y + title.getCharacterSize() + 150.f);
	sf::Text startBText;
	startBText.setFont(font);

	sf::Text sizeText;
	sizeText.setFont(font);
	sizeText.setCharacterSize(30);
	sizeText.setFillColor(sf::Color::White);
	sizeText.setPosition(sf::Vector2f(w/2 - 10, startB.getPosition().y + startB.getPoint(3).y + 40));
	
	sf::RectangleShape LoadB(sf::Vector2f(buttonSize*2, buttonSize/1.5));
	LoadB.setPosition(sizeText.getPosition() + sf::Vector2f(-LoadB.getSize().x/2 + 10, 70));
	sf::Text LoadBText;
	LoadBText.setFont(font);

	sf::RectangleShape addSizeB(sf::Vector2f(buttonSize/2, buttonSize/2));
	addSizeB.setPosition(sizeText.getPosition() + sf::Vector2f( 50.f, -5));
	sf::Text addSizeBText;
	addSizeBText.setFont(font);

	sf::RectangleShape minusSizeB(sf::Vector2f(buttonSize / 2, buttonSize / 2));
	minusSizeB.setPosition(sizeText.getPosition() + sf::Vector2f(-50.f - sizeText.getCharacterSize() + 5, -5));
	sf::Text minusSizeBText;
	minusSizeBText.setFont(font);
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// Update Mouse position
		mousePosScreen = sf::Mouse::getPosition();
		mousePosWindow = sf::Mouse::getPosition(window);
		mousePosWindowF = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

		// Generate Buttons text
		std::stringstream buttonStr;
		buttonStr << "  MAZE SOLVING\n\t VISUALIZER";
		title.setString(buttonStr.str());
		centerTextAt(title, sf::Vector2f(w / 2, 100));

		buttonStr.str("");
		buttonStr << "START";
		startBText.setString(buttonStr.str());
		setOriginCenterRect(startB, startBText);

		buttonStr.str("");
		buttonStr << "+";
		addSizeBText.setString(buttonStr.str());
		setOriginCenterRect(addSizeB, addSizeBText, sf::Color(240, 100, 0), 10.f);

		buttonStr.str("");
		buttonStr << "-";
		minusSizeBText.setString(buttonStr.str());
		setOriginCenterRect(minusSizeB, minusSizeBText, sf::Color(240, 100, 0), 10.f);

		buttonStr.str("");
		buttonStr << size;
		sizeText.setString(buttonStr.str());
		centerTextAt(sizeText, sf::Vector2f(w / 2, startB.getPosition().y + startB.getPoint(3).y + 60));

		buttonStr.str("");
		buttonStr << "Load";
		LoadBText.setString(buttonStr.str());
		setOriginCenterRect(LoadB, LoadBText);


		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event e;
		while (window.pollEvent(e))
		{
			// "close requested" event: we close the window
			if (e.type == sf::Event::Closed)
			{
				window.close();
				return -1;
			}

			// Buttons
			if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
			{
				if (collides(mousePosWindowF, startB))
				{
					window.close();
					return size;
				}

				if (collides(mousePosWindowF, addSizeB))
				{
					if (size < 200)
						size++;
				}

				if (collides(mousePosWindowF, minusSizeB))
				{
					if (size > 2)
						size--;
				}
				
				if (collides(mousePosWindowF, LoadB))
				{
					window.close();
					return -2;
				}
			}
		}

		// clear the window with black color
		window.clear(sf::Color(200, 200, 200, 200));
		window.setView(view);
		// draw everything here...
		window.draw(title);
		window.draw(startB);
		window.draw(startBText);
		window.draw(addSizeB);
		window.draw(addSizeBText);
		window.draw(minusSizeB);
		window.draw(minusSizeBText);
		window.draw(LoadB);
		window.draw(LoadBText);
		window.draw(sizeText);

		// end the current frame
		window.display();
	}

	return 0;
}

std::string fileSelector()
{
	// create the window
	sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosWindowF;
	sf::RenderWindow window(sf::VideoMode(300, 450), "Select your file", sf::Style::Titlebar | sf::Style::Close);
	sf::View view;
	view.setSize(300, 450);
	view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);

	sf::Font font;
	if (!font.loadFromFile("Fonts/SourceSansPro-Regular.ttf"))
	{
		return "";
	}
	const float buttonSize = 100.f;

	// Load filenames vector
	std::vector<std::string> filenames;
	std::vector<sf::Text> names;
	sf::Text added;
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("Mazes/"))
	{
		if (dirEntry.path().extension() == ".txt")
		{
			std::cout << dirEntry << std::endl;
			filenames.push_back(dirEntry.path().filename().string());
			added.setString(dirEntry.path().filename().string());
			names.push_back(added);
		}
	}
	
	for (int i = 0; i < names.size(); i++)
	{
		names[i].setFont(font);
		names[i].setCharacterSize(22);
		names[i].setFillColor(sf::Color::Black);
		names[i].setPosition(sf::Vector2f(0.f, i * 25.f));
	}
	sf::RectangleShape selectedRect(sf::Vector2f(300, 25));
	selectedRect.setFillColor(sf::Color::Black);
	int selected = 0;
	selectedRect.setPosition(0, 0);
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// Update Mouse position
		mousePosScreen = sf::Mouse::getPosition();
		mousePosWindow = sf::Mouse::getPosition(window);
		mousePosWindowF = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));


		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event e;
		while (window.pollEvent(e))
		{
			// "close requested" event: we close the window
			if (e.type == sf::Event::Closed)
			{
				window.close();
				return "";
			}

			if (e.type == sf::Event::KeyReleased)
			{
				if (e.key.code == sf::Keyboard::Up)
				{
					if (selected > 0)
					selected--;
				}
				if (e.key.code == sf::Keyboard::Down)
				{
					if (selected < filenames.size()-1)
					selected++;
				}
			}
		}
		if (names[selected].getPosition().y >= 450)
		{
			for (int i = 0; i < names.size(); i++)
				names[i].setPosition(names[i].getPosition() + sf::Vector2f(0.f, -25.f));
		}
		if (names[selected].getPosition().y < 0)
		{
			for (int i = 0; i < names.size(); i++)
				names[i].setPosition(names[i].getPosition() + sf::Vector2f(0.f, 25.f));
		}
		for (int i = 0; i < names.size(); i++)
			names[i].setFillColor(sf::Color::Black);
		names[selected].setFillColor(sf::Color::White);
		selectedRect.setPosition(names[selected].getPosition());

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //Left
		{
			return names[selected].getString();
		}

		// clear the window with black color

		window.clear(sf::Color(200, 200, 200, 200));
		window.setView(view);
		// draw everything here...
		window.draw(selectedRect);
		for (int i = 0; i < names.size(); i++)
		{
			window.draw(names[i]);
		}
		// end the current frame
		window.display();
	}
}


bool collides(sf::Vector2f position, sf::RectangleShape rect)
{
	if (rect.getPosition().x+ rect.getSize().x < position.x)
	{
		return false;
	}
	if (rect.getPosition().x > position.x)
	{
		return false;
	}
	if (rect.getPosition().y + rect.getSize().y < position.y)
	{
		return false;
	}
	if (rect.getPosition().y > position.y)
	{
		return false;
	}
	return true;
}

void setOriginCenterRect(sf::RectangleShape& shape, sf::Text& text, sf::Color color, float offset)
{
	shape.setFillColor(color);

	text.setCharacterSize(shape.getSize().y - offset);
	text.setOrigin(text.getGlobalBounds().width / 2.f + text.getLocalBounds().left, text.getGlobalBounds().height / 2.f + text.getLocalBounds().top);
	text.setPosition(shape.getPosition() + (shape.getSize() / 2.f));
	text.setFillColor(sf::Color::Black);
}

bool checkSwap(sf::Event e)
{
	if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Middle)
		return true;
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)
		return true;
	return false;
}
void setText(sf::Text& text, sf::Vector2f position, int size)
{
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(size);
	text.setPosition(position);
}

void centerTextAt(sf::Text& text, sf::Vector2f position)
{
	text.setOrigin(text.getGlobalBounds().width / 2.f + text.getLocalBounds().left, text.getGlobalBounds().height / 2.f + text.getLocalBounds().top);
	text.setPosition(position);
}