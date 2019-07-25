#include "FileHelper.h"
namespace FileHelper {

	/*
	 0 = none
	 1 = vertex
	 2 = fragment/pixel
	*/

	string ReadFileGl(const char * FilePath)
	{
		string Result {"Error"};
		std::ifstream File(FilePath);
		if (File.is_open())
		{
			std::stringstream SStream;
			SStream << File.rdbuf();
			Result = SStream.str();

			File.close();
			return Result;
		}
		else
		{
			const char *Message = "Error : Can't open File ";

			OutputDebugStringA(Message);
		}
		return Result;
	}

}