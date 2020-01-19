#include "core.h"
#include <iostream>
#include "filestream.h"
#include "getopt.h"

void core::handle_args(const int& argc, char* argv[])
{
	std::string file;
	std::string message;
	bool verbose = false;
	int opt;
	while ((opt = getopt(argc, argv, "vf:m:h")) != -1)
	{
		switch (opt)
		{
		case 'v':
			verbose = true;
			break;
		case 'f':
			file = optarg;
			break;
		case 'm':
			message = optarg;
			break;
		case 'h':
			std::cout << "INSTRUCTIONS:" << std::endl;
			std::cout << "The program accepts these options:" << std::endl;
			std::cout << "\tf: Path to the file." << std::endl;
			std::cout << "\tv: Prints additional information to console." << std::endl;
			std::cout << "\tm: Message to write to file." << std::endl;
			std::cout << "\th: This text." << std::endl;
			exit(0);
		case '?':
			std::cerr << "Bad input! " << std::endl;
			exit(1);

		default:
			std::cerr << "Bad options" << std::endl;
			exit(1);
		}
	}
	if (message.empty())
	{
		filestream::read_file(file, verbose);
	}
	else
	{
		filestream::write_message(file, message);
	}
}
