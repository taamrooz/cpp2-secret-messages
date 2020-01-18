#include "core.h"
#include "crtdbg.h"

int main(int argc, char* argv[])
{
	core::handle_args(argc, argv);
	
	_CrtDumpMemoryLeaks();
}
