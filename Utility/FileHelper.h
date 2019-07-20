#pragma once
#include "Usable_Windows.h"
#include <string>
#include <fstream>
#include <filesystem>

namespace FileHelper {

	using std::string;
	/*
	0 = vertexShader
	1 = FragmentShader
	*/
	/// at the moment the int is just a place holder 
	 string ReadFileGl(const char *FilePath, int Type);

}