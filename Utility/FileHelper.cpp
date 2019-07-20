#include "FileHelper.h"
namespace FileHelper {

	/*
	 0 = none 
	 1 = vertex 
	 2 = fragment/pixel 
	*/

	string ReadFileGl(const char * FilePath, int Type)
	{
		string Result;
		std::ifstream File(FilePath);
		if (File.is_open())
		{
			while (!File.eof())
			{
				string Temp;
				std::getline(File, Temp);
				Result += Temp;
			}
		}
		else
		{
			const char *Message = "Error : Can't open File ";  

			OutputDebugStringA(Message);
		}



		File.close();
		return Result;
	}

}