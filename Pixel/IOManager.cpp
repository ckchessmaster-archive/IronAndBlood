#include "IOManager.h"

#include <iostream>
#include <fstream>

namespace Pixel
{
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//find the size of a file
		file.seekg(0, std::ios::end); //seek to the end of the file
		int fileSize = file.tellg(); //get the file size
		file.seekg(0, std::ios::beg); //seek to the begining of the file

		fileSize -= file.tellg(); //reduce the file size by any header bytes that might be present

		//read file
		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);
		file.close();

		return true;
	}
}