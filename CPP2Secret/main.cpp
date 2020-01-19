#include "core.h"
#include "crtdbg.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(65001);
	core::handle_args(argc, argv);
	
	_CrtDumpMemoryLeaks();
}
