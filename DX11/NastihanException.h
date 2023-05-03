#pragma once
#include <exception>
#include <string>


class NastihanException : public std::exception
{
public:
	NastihanException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	std::string file;
	int line;
protected:
	mutable std::string whatBuffer;
};