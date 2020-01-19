#pragma once
#include <string>

extern std::string optarg;
extern int optind;

int getopt(int argc, char* const argv[], const char* opstring);