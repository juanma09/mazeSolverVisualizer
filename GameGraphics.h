#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


int mainMenu();
std::string fileSelector();
bool collides(sf::Vector2f position, sf::RectangleShape rect);
void setOriginCenterRect(sf::RectangleShape& shape, sf::Text& text, sf::Color color = sf::Color::Yellow, float offset = 30.f);
void setText(sf::Text& text, sf::Vector2f position, int size);
bool checkSwap(sf::Event e);
void centerTextAt(sf::Text& text, sf::Vector2f position);