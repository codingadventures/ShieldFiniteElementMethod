#ifndef FILE_H
#define FILE_H
#pragma once
#include "Common.h"

class File
{
public:
	int m_fileSize;
	std::string m_folderPath;
	std::string m_fileName;
	std::ifstream m_file;

	File();
	File(const File &other);
	File& operator=(const File &other);
	File(const std::string &folderPath, const std::string &fileName);
	~File();
	void open(const std::string &folderPath, const std::string &fileName);
	void open();
	void close();
	bool isOpen();
	void print();
	//appends the contents of @param file to current file
	File& append(File &file);
	//prepends the contents of @param file to current file
	File& prepend(File &file);

	std::string asString();
	//operator std::string();
	//check on file name
	bool operator==(const File &other) const;


private:
	void setSize();
};


#endif