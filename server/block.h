#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

class Block {
	int x;
	int y;
	int width;
	int height;
	std::string name;

public:
	Block(int scrw, int scrh) {
		width = rand() % 7 + 3;
		height = rand() % 7 + 3;
		x = rand() % (scrw - width);
		y = rand() % (scrh - height);
	}

	int Area() {
		return width * height;
	}

	int GetX() {
		return x;
	}

	int GetY() {
		return y;
	}

	void Print() {
		std::cout << "X: " << x << std::endl;
		std::cout << "Y: " << y << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
		std::cout << "Area: " << Area() << std::endl;
		std::cout << "Name: " << name << std::endl;
	}

	void Rename(std::string new_name) {
		name = new_name;
	}

	void Rename() {
		std::string new_name;
		std::cout << "Enter new name, please: ";
		std::cin >> new_name;
		Rename(new_name);
	}
};
/*
int main() {
	srand(time(NULL));

	std::cout << "Block 1" << std::endl;
	Block block1(200, 200);
	block1.Rename();
	block1.Print();

	std::cout << "Block 2" << std::endl;
	Block block2(1080, 920);
	block2.Rename();
	block2.Print();

	return 0;
};
*/
