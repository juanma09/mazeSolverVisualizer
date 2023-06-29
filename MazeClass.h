#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <filesystem>
#include <windows.h>

struct coord
{
	int x;
	int y;
	int fCost;
	int hCost;
	int gCost;
};

bool operator< (coord a, coord b);
bool operator== (const coord& a, const coord& b);

class maze
{
private:
	std::string fname;
	std::vector< std::vector<int> > mazeArr;
	int size;
	std::vector<coord> visited;
	std::vector<coord> toVisit;
	std::map<coord, coord> parent;
	std::vector<coord> path;
	coord first = { -1, -1 };
	coord last = { -1,-1 };
	coord cur;
public:
	bool solved = false;
	maze(std::string filename)
	{
		fname = filename;
		std::ifstream mazeTxt(filename);
		// Get the size of the txt
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
		{
			return;
		}
		// Resize
		mazeArr.resize(size);
		for (int i = 0; i < size; i++)
		{
			mazeArr[i].resize(size, 0);
		}
		mazeTxt.open(filename);
		if (mazeTxt.is_open() == true) {
			char c; int i = 0;
			while (mazeTxt.get(c)) {
				if (getMazeEl(c) != -1)
				{
					int tmp = getMazeEl(c);
					mazeArr[i / size][i % size] = tmp;

					if (tmp == 3)
					{
						first = { i / size, i % size };
					}

					if (tmp == 1)
					{
						last = { i / size,i % size };
					}

					i++;
				}
			}
			mazeTxt.close();
		}
		displayMatrix();

		if (first.x == -1)
		{
			if (last.x != 0 || last.y != 0)
			{
				mazeArr[0][0] = 3;
				first = { 0,0 };
			}
			else
			{
				mazeArr[0][1] = 3;
				first = { 0,1 };
			}
		}
		if (last.x == -1)
		{
			if (first.x != 0 || first.y != 1)
			{
				mazeArr[0][1] = 1;
				last = { 0,1 };
			}
			else
			{
				mazeArr[0][0] = 1;
				last = { 0,0 };
			}
		}
		//std::cout << i << ' ' << j;
		setFirst(first);
		//displayToVisit();

	}
	maze(int sx)
	{
		size = sx;
		fname = "";
		mazeArr.resize(size);
		for (int i = 0; i < size; i++)
		{
			mazeArr[i].resize(size, 0);
		}

		std::ifstream mazeTxt("Maze.txt");
		int textSize = 0;
		if (mazeTxt.is_open() == true) {
			char c; 
			while (mazeTxt.get(c)) {
				if (c == '\n')
				{
					break;
				}
				textSize++;
			}
			mazeTxt.close();
		}
		else
			return;
		if (textSize == size)
		{
			mazeTxt.open("Maze.txt");
			if (mazeTxt.is_open() == true) {
				char c; int i = 0;
				while (mazeTxt.get(c)) {
					if (getMazeEl(c) != -1)
					{
						int tmp = getMazeEl(c);
						mazeArr[i / size][i % size] = tmp;

						if (tmp == 3)
						{
							first = { i / size, i % size };
						}

						if (tmp == 1)
						{
							last = { i / size,i % size };
						}

						i++;
					}
				}
				mazeTxt.close();
			}
			else
				return;
		}

		if (first.x == -1)
		{
			if (last.x != 0 || last.y != 0)
			{
				mazeArr[0][0] = 3;
				first = { 0,0 };
			}
			else
			{
				mazeArr[0][1] = 3;
				first = { 0,1 };
			}
		}
		if (last.x == -1)
		{
			if (first.x != 0 || first.y != 1)
			{
				mazeArr[0][1] = 1;
				last = { 0,1 };
			}
			else
			{
				mazeArr[0][0] = 1;
				last = { 0,0 };
			}
		}


		//std::cout << i << ' ' << j;
		setFirst(first);
		//displayToVisit();

	}

	~maze()
	{

	}

	coord getLast() { return last; }
	coord getFirst() { return first; }
	coord getCur() { return cur;  }
	coord queue();
	coord stack();
	coord randomDel();
	void solveDfs();
	void solveBfs();
	void solveRandom();

	void solveAStar();
	coord getAStar();
	void swap(int x1, int x2);
	int distance(coord x1, coord x2);

	coord getPath(coord prev);
	int getMazeEl(char c);
	char getMazeChar(int x);
	void insertSides(coord xy);
	int sizeX();
	int sizeY();
	int getElement(coord xy);
	void setElement(coord xy, int value);

	int getVisitedSize() { return visited.size(); }

	void displayToVisit()
	{
		for (int i = 0; i < toVisit.size(); i++)
		{
			std::cout << "(" << toVisit[i].x << "," << toVisit[i].y << "): " << toVisit[i].fCost << ", ";
			std::cout << toVisit[i].hCost << ", " << toVisit[i].gCost << ";";
		}
		std::cout << std::endl;
	}
	void displayParents()
	{
		for (const auto& elem : parent)
		{
			std::cout << elem.first.x << ", " << elem.first.y << ": " << elem.second.x << ", " << elem.second.y << std::endl;
		}
	}

	void displayMatrix();
	void saveNewTXT();
	void saveTXT();

	void swap(coord x1, coord x2);

	void setFirst(coord xy);
	void setLast(coord xy);

	void Restart();
	void Resize(int s)
	{
		for (int i = 0; i < size; i++)
		{
			mazeArr[i].resize(s);
		}
		mazeArr.resize(s);
	}
	void generateRandom();
	void clearMaze();

};
