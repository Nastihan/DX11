#include "NastihanException.h"
#include <sstream>



NastihanException::NastihanException(int line, const char* file) noexcept
	: line(line),file(file)
{
}

const char* NastihanException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* NastihanException::GetType() const noexcept
{
	return "Nastihan Exception";
}

int NastihanException::GetLine() const noexcept
{
	return line;
}

const std::string& NastihanException::GetFile() const noexcept
{
	return file;
}

std::string NastihanException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str(); 
}
