#include <iostream>
#include <string>
#include "filestream.h"



int main(int argc, char* argv[])
{
	int counter = 0;
	std::string file;
	while (counter < argc) {
		if (counter == 1) {
			file = argv[1];
		}
		++counter;
	}
	filestream::read_file(file);
}
