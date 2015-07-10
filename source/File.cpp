#include "File.h"

using namespace std;

File::File(const std::string &folderPath, const std::string &fileName)
{
	open(folderPath, fileName);
}

File::File()
{

}

File::File(const File &other)
{
	*this = other;
}

File& File::operator=(const File &other)
{
	open(other.m_folderPath, other.m_fileName);
	return *this;
}

File::~File()
{
	close();
}

void File::open(const std::string &folderPath, const std::string &fileName)
{
	m_folderPath = folderPath;
	m_fileName = fileName;
	open();


}

void File::open()
{

	//Note: there is a bug in the gcc compiler 4.6 so we cannot the assignment operator on ifstream objects
	//workaround is to use the open function
	m_file.open(string(m_folderPath + m_fileName).c_str());
	if (!m_file.is_open())
	{
		LOGI("Unable to open file %s%s\n", m_folderPath.c_str(), m_fileName.c_str());
		return;
	}

	setSize();
}



void File::close()
{
	m_file.close();
}

bool File::isOpen()
{
	return m_file.is_open();
}

void File::print()
{
	m_file.seekg(0, ios::beg);
	string line;
	if (m_file.is_open())
	{
		while ( getline (m_file,line) )
	{	
			LOGI("%s\n", line.c_str());
		}	
	}
	else
		LOGI("Unable to open file\n");

	m_file.seekg(0, ios::beg);
}

File& File::append(File &file)
{
	std::ofstream tempOut("temp.txt", std::ios_base::binary);
	tempOut << m_file.rdbuf() << file.m_file.rdbuf();
	tempOut.close();
	file.m_file.seekg(0,ios::beg);
	m_file.close();
	m_file.open("temp.txt");
	setSize();
	return *this;
}

File& File::prepend(File &file)
{
	std::ofstream tempOut("temp.txt", std::ios_base::binary);
	tempOut << file.m_file.rdbuf() << m_file.rdbuf();
	tempOut.close();
	file.m_file.seekg(0,ios::beg);
	m_file.close();
	m_file.open("temp.txt");
	setSize();
	return *this;
}


std::string File::asString()
{
	m_file.seekg(0, ios::beg);
	string line,result;
	while(getline(m_file,line))
		result.append(line + "\n");
	m_file.seekg(0, ios::beg);
	return result;
}

bool File::operator==(const File &other) const
{
	return m_folderPath + m_fileName == other.m_folderPath + other.m_fileName;
}

void File::setSize()
{
	m_file.seekg(0, ios::end);
	m_fileSize = m_file.tellg();
	m_file.seekg(0, ios::beg);
}

bool numericalFileSort(const std::string a, const std::string b)
{
	return (a.length() < b.length() || (a.length() == b.length() && (a < b)));
};


//File::operator std::string()
//{
//	return asString();
//}
