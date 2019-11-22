#include <iostream>
#include "fstream"
#include <vector>
#include <bitset>
void show_binrep(char* a, int length)
{
	const char* beg = reinterpret_cast<const char*>(&a);
	const char* end = beg + length;
	while (beg != end)
		std::cout << std::bitset<CHAR_BIT>(*beg++) << ' ';
	std::cout << '\n';
}

int main(int argc, char* argv[])
{
	std::cout << "Hello World!\n";
	int counter = 0;
	std::string file;
	while (counter < argc) {
		
		std::cout << argv[counter] << std::endl;
		if (counter == 1) {
			file = argv[1];
		}
		++counter;
	}
	std::ifstream input(file, std::ios::binary);
	if (input) {
		// get length of file:
		input.seekg(0, input.end);
		int length = input.tellg();
		input.seekg(0, input.beg);

		// allocate memory:
		char* buffer = new char[length];
		// read data as a block:
		input.read(buffer, length);

		input.close();
		show_binrep(buffer, length);
		// print content:
		//std::cout.write(buffer, length);

		delete[] buffer;
	}
//	
	std::cin;
}
