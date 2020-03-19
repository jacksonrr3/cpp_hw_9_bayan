#include <iostream>
#include <fstream>
#include "options.h"
#include "file_handler.h"


int main(int argc, const char* argv[]) {
	try 
	{
		Options opt;
		opt.parse_comm_line_opt(argc, argv);
		File_Handler fh(opt);
		fh.run();

	}

	catch (const error & ex)
	{
		std::cerr << ex.what() << '\n';
	}


	return 0;
}
