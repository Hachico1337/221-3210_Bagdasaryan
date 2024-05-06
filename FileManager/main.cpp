#include <iostream>
#include <string>
#include <fstream>
#include "windows.h"


bool readFile(const char* path) {
	std::cout << "Reading \n";

	std::ifstream fIn;

	fIn.open(path);

	if (!fIn) {

		fIn.close();
		return false;
	}
	else {
		std::string line;
		while (std::getline(fIn, line)) {
			std::cout << line << "\n";
		}
	}

	fIn.close();

	return true;
}

bool writeToFile(const char* path, const char * text) {
	std::cout << "Writing\n";

	std::ofstream fOut;

	fOut.open(path);

	if (!fOut) {

		fOut.close();
		return false;
	}
	else {
		fOut << text;
		fOut.close();
	}


	return true;
}


int main() {

	SetConsoleTitleA("F1 Write | F2 Read | F3 Exit");

	while(1)
	{
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			writeToFile("test.bagdasaryan", "i hate this lab i hate this lab i hate this lab i hate this lab i hate this lab");
		}
		else if (GetAsyncKeyState(VK_F2) & 1)
		{
			readFile("test.bagdasaryan");
		}
		if(GetAsyncKeyState(VK_F3))
		{
			exit(0);
		}
	}


	return 0;
}