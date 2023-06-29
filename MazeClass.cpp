
#include <iostream>
#include "MazeClass.h"

bool operator< (coord a, coord b) { return std::make_pair(a.x, a.y) < std::make_pair(b.x, b.y); }
bool operator== (const coord &a, const coord &b) { return (a.x == b.x && a.y == b.y); }

coord maze::queue()
{
	coord next;
	if (toVisit.size() > 0)
	{
		next = toVisit.front(); // Returns first element of toVisit
		toVisit.erase(toVisit.begin());
	}
	else
	{
		next = { -1, -1 };
		std::cout << "ERROR: No more elements in queue" << std::endl;
	}
	return next;
	//std::cout << "(" << next.x << "," << next.y << ")" << std::endl;
}
coord maze::stack()
{
	coord next;
	if (toVisit.size() > 0)
	{
		next = toVisit.back(); // Returns the last element
		toVisit.pop_back(); // Deletes the last element
	}
	else
	{
		next = { -1, -1 };
		std::cout << "ERROR: No more elements in queue" << std::endl;
	}
	//std::cout << "(" << next.x << "," << next.y << ")" << std::endl;
	return next;
}
coord maze::randomDel()
{
	coord next;
	if (toVisit.size() > 0)
	{
		int choice = rand() % toVisit.size();
		next = toVisit[choice];
		toVisit.erase(toVisit.begin() + choice);
	}
	else
	{
		next = { -1, -1 };
		std::cout << "ERROR: No more elements" << std::endl;
	}
	return next;
}

void maze::solveDfs()
{
	if (solved)
	{
		return;
	}
	cur = stack();

	if (cur.x == -1)
	{
		solved = true;
		return;
	}
	if (mazeArr[cur.x][cur.y] == 1)
	{
		solved = true;
		toVisit.clear();
		//std::cout << "FOUND AT (" << cur.x << "," << cur.y << ")" << std::endl;
		coord prev = parent[cur];

		//getPath(prev);
		return;
	}
	else
	{
		mazeArr[cur.x][cur.y] = -1;
	}
	visited.push_back(cur);
	insertSides(cur);
	//displayToVisit();
}
void maze::solveBfs()
{
	if (solved)
	{
		return;
	}
	cur = queue();

	if (cur.x == -1)
	{
		solved = true;
		return;
	}
	if (mazeArr[cur.x][cur.y] == 1)
	{
		solved = true;
		toVisit.clear();
		//std::cout << "FOUND AT (" << cur.x << "," << cur.y << ")" << std::endl;
		//coord prev = parent[cur];
		//displayParents();

		//getPath(prev);
		return;
	}
	else
	{
		mazeArr[cur.x][cur.y] = -1;
	}
	visited.push_back(cur);
	insertSides(cur);

	//displayToVisit();
}

void maze::solveRandom()
{
	if (solved)
	{
		return;
	}
	cur = randomDel();

	if (cur.x == -1)
	{
		solved = true;
		return;
	}
	if (mazeArr[cur.x][cur.y] == 1)
	{
		solved = true;
		toVisit.clear();
		//std::cout << "FOUND AT (" << cur.x << "," << cur.y << ")" << std::endl;
		coord prev = parent[cur];

		//getPath(prev);
		return;
	}
	else
	{
		mazeArr[cur.x][cur.y] = -1;
	}
	visited.push_back(cur);
	insertSides(cur);
	//displayToVisit();
}

void maze::solveAStar()
{
	if (solved)
	{
		return;
	}
	cur = getAStar();

	if (cur.x == -1)
	{
		solved = true;
		return;
	}
	if (mazeArr[cur.x][cur.y] == 1)
	{
		solved = true;
		toVisit.clear();
		//std::cout << "FOUND AT (" << cur.x << "," << cur.y << ")" << std::endl;
		coord prev = parent[cur];

		//getPath(prev);
		return;
	}
	else
	{
		mazeArr[cur.x][cur.y] = -1;
	}
	visited.push_back(cur);
	insertSides(cur);
	//displayToVisit();
}
//Gcost: distancia a first Prioridad 3
//Hcost: distancia al objetivo	Prioridad 2
// Fcost: distancia total Prioridad 1
coord maze::getAStar()
{
	// Sort the array by fcost, then by hcost, then by gcost
	// 
	coord next = { -1, -1 };
	if (toVisit.size() == 0) return next;
	next = toVisit[0];
	int ind = 0;
	for (int i = 0; i < toVisit.size(); i++)
	{
		// Compare the elements based on the fcost
		if (next.fCost > toVisit[i].fCost)
		{
			next = toVisit[i];
			ind = i;
		}
		else if (next.fCost == toVisit[i].fCost)
		{
			if (next.hCost > toVisit[i].hCost)
			{
				next = toVisit[i];
				ind = i;
			}
			else if (next.hCost == toVisit[i].hCost)
			{
				if (next.gCost > toVisit[i].gCost)
				{
					next = toVisit[i];
					ind = i;
				}
			}
		}
	}
	// Sort the array by hcost
	displayToVisit();
	toVisit.erase(toVisit.begin() + ind);
	return next;

	/*for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            // Compare the elements based on the first variable
            if (arr[j].first > arr[j + 1].first) {
                swap(arr[j], arr[j + 1]);
            } else if (arr[j].first == arr[j + 1].first) {
                // If the first variables are the same, compare the second variables
                if (arr[j].second > arr[j + 1].second) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
    }*/
}
int maze::distance(coord x1, coord x2)
{
	return 10 * (abs(x2.x - x1.x) + abs(x2.y - x1.y));
	//return 10*sqrt(pow(abs(x2.x - x1.x) + 1, 2) + pow(abs(x2.y - x1.y) + 1,2));
}
  
coord maze::getPath(coord prev)
{
	if (prev == first)
	{
		return prev;
	}
	//std::cout << "(" << prev.x << ", " << prev.y << ")" << std::endl;
	mazeArr[prev.x][prev.y] = -2;
	if (prev == last)
	{
		mazeArr[prev.x][prev.y] = 1;
	}
	prev = parent[prev];

	return prev;

}

int maze::getMazeEl(char c)
{
	switch (c)
	{
	case '#':
		return 2;
	case '.':
		return 0;
	case '*':
		return 1;
	case 's':
		return 3;
	default:
		break;
	}
	return -1;
}

char maze::getMazeChar(int x)
{
	switch (x)
	{
	case 2:
		return '#';
	case 0:
		return '.';
	case 1:
		return '*';
	case 3:
		return 's';
	case -1:
		return ',';
	case -2:
		return 'x';
	default:
		break;
	}
	return '.';
}


void maze::insertSides(coord xy)
{
	// Get a list of the four sides
	coord sides[4];
	bool areInRange[4] = { 0 };
	// ######
	// ######
	// ######
	sides[0] = { xy.x - 1, xy.y }; // Up
	sides[1] = { xy.x, xy.y - 1 }; //Left
	sides[2] = { xy.x + 1, xy.y }; // Down
	sides[3] = { xy.x, xy.y + 1 }; // Right
	// Check they are in range
	for (int i = 0; i < 4; i++)
	{
		if (sides[i].x < 0 || sides[i].x >= size || sides[i].y < 0 || sides[i].y >= size)
		{
			areInRange[i] = false;
		}
		else
		{
			areInRange[i] = true;
		}
	}

	// f t t t
	// Check they are walkable paths
	for (int i = 0; i < 4; i++)
	{
		if (!areInRange[i])
		{
			continue;
		}
		if (mazeArr[sides[i].x][sides[i].y] == 2)
		{
			areInRange[i] = false;
		}
	}
	// f f t f
	// Check they are not in visited or in to visit
	for (int i = 0; i < 4; i++)
	{
		if (!areInRange[i])
		{
			continue;
		}
		for (int j = 0; j < toVisit.size(); j++)
		{
			if (sides[i] == toVisit[j])
			{
				//areInRange[i] = false;
				toVisit.erase(toVisit.begin() + j);
			}
		}
		for (int j = 0; j < visited.size(); j++)
		{
			if (sides[i] == visited[j])
			{
				areInRange[i] = false;
			}
		}
	}
	// Add to to visit
	for (int i = 0; i < 4; i++)
	{
		if (areInRange[i])
		{
			sides[i].gCost = xy.gCost + 10;
			sides[i].hCost = distance(sides[i], last);
			sides[i].fCost = sides[i].gCost + sides[i].hCost;
			toVisit.push_back(sides[i]);
			parent[sides[i]] = xy;
		}
	}
}

int maze::sizeX()
{
	return size;
}

int maze::sizeY()
{
	return size;
}

int maze::getElement(coord xy)
{
	return mazeArr[xy.x][xy.y];
}

void maze::setElement(coord xy, int value)
{
	mazeArr[xy.x][xy.y] = value;
}

void maze::displayMatrix()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			std::cout <<mazeArr[i][j];
		}
		std::cout << std::endl;
	}
}

void maze::saveTXT()
{
	std::ofstream ofs;
	if (fname == "")
		ofs.open("Maze.txt", std::ofstream::out | std::ofstream::trunc);
	else
		ofs.open(fname, std::ofstream::out | std::ofstream::trunc);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			ofs << getMazeChar(mazeArr[i][j]);
		}
		ofs << '\n';
	}
	ofs.close();
}

void maze::saveNewTXT()
{
	std::ofstream ofs;
	std::string newFileName;
	// Get the biggest number
	int num = 0;
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("Mazes/"))
	{
		if (dirEntry.path().extension() == ".txt")
		{
			if (num < std::stoi(dirEntry.path().stem().string()))
			{
				num = std::stoi(dirEntry.path().stem().string());
			}
		}

	}
	// Set and pad the 0s to the left
	newFileName = std::to_string(num+1);
	newFileName.insert(0, 4 - newFileName.size(), '0');
	newFileName += ".txt";
	newFileName = "Mazes/" + newFileName;

	std::cout << newFileName << std::endl;

	ofs.open(newFileName, std::ofstream::out | std::ofstream::trunc);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			ofs << getMazeChar(mazeArr[i][j]);
		}
		ofs << '\n';
	}
	ofs.close();
}


void maze::swap(coord x1, coord x2)
{
	int tmp = mazeArr[x1.x][x1.y];

	mazeArr[x1.x][x1.y] = mazeArr[x2.x][x2.y];
	mazeArr[x2.x][x2.y] = tmp;
}

void maze::setFirst(coord xy)
{
	if (visited.size() > 0)
		visited.clear();
	if (toVisit.size() > 0)
		toVisit.clear();
	parent.clear();
	first = xy;
	visited.push_back(first);
	insertSides(first);
}

void maze::setLast(coord xy)
{
	last = xy;
}

void maze::Restart()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (mazeArr[i][j] < 0)
			{
				mazeArr[i][j] = 0;
			}
		}
	}
	setFirst(first);
	solved = false;
}

void maze::generateRandom()
{
	clearMaze();
	/*coord sides[4];
	sides[0] = { -1, 0 };
	sides[1] = { 0, -1 };
	sides[2] = { 1, 0 };
	sides[3] = { 0, 1 };

	int choice[2] = { -1, -1 };

	choice[0] = 2 * (rand() % 2);
	choice[1] = 2 * (rand() % 2) + 1;

	for (int i = choice[0]/2; i < mazeArr.size(); i += 2)
	{
		for (int j = choice[0] / 2; j < mazeArr[i].size(); j += 2)
		{
			mazeArr[i][j] = 2;
		}
	}
	


	for (int i = choice[0]/2; i < mazeArr.size()-1; i += 2)
	{
		for (int j = choice[0] / 2; j < mazeArr[i].size()-1; j += 2)
		{
			int a = choice[rand() % 2];
			if (i + sides[a].x < 0 || j + sides[a].y < 0 || i + sides[a].x >= size || j + sides[a].y > size)
				continue;
			mazeArr[i + sides[a].x][j + sides[a].y] = 2;
			if (rand() % 32 < 3)
			{
				mazeArr[i + sides[a].x][j + sides[a].y] = 0;
			}

		}
	}*/
	int density = 1;
	density = size * size * density / 4;
	//valores

	//laberinto 
	srand(time(NULL));
	for (int i = 0; i < density; i++) {
		int x = rand() % (size - 4) + 2; // 2 18 
		x = (x / 2) * 2;
		int y = rand() % (size - 4) + 2;
		y = (y / 2) * 2;
		mazeArr[y][x] = 2;
		for (int j = 0; j < density*8; j++) {
			int mx[4] = { x,  x,  x + 2, x - 2 };
			int my[4] = { y + 2,y - 2, y ,  y };
			int r = rand() % 4;
			if (mazeArr[my[r]][mx[r]] == 0) {
				mazeArr[my[r]][mx[r]] = 2;
				mazeArr[my[r] + (y - my[r]) / 2][mx[r] + (x - mx[r]) / 2] = 2;
			}

		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == 0 || j == 0 || i == size - 1 || j == size - 1) {
				mazeArr[i][j] = 2;
			}
		}
	}

	mazeArr[first.x][first.y] = 3;
	if (rand() % 2)
		mazeArr[first.x + 1][first.y] = 0;
	else
		mazeArr[first.x][first.y + 1] = 0;


	mazeArr[last.x][last.y] = 1;
	setFirst(first);
	setLast(last);
}

void maze::clearMaze()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			coord cur = { i, j };
			if (cur == first || cur == last)
				continue;
			mazeArr[i][j] = 0;
		}
	}
	setFirst(first);
	setLast(last);
}

void maze::swap(int x1, int x2)
{
	coord tmp = toVisit[x1];
	toVisit[x1] = toVisit[x2];
	toVisit[x2] = tmp;
}

//int main()
//{
//	//int sx = 0, sy = 0, tmp = 1;
//	//std::ifstream file("Maze.txt");
//
//	maze m(10, 10);
//	m.displayMatrix();
//
//	coord xy = { 4, 4 };
//	m.setLast(xy);
//	m.displayMatrix();
//
//	system("pause");
//	system("cls");
//	while(!m.solved)
//		m.solveBfs();
//	
//	coord prev = m.getLast();
//	while (true)
//	{
//		prev = m.getPath(prev);
//		system("cls");
//		m.displayMatrix();
//	}
//
//	//m.displayMatrix();
//	return 0;
//}


