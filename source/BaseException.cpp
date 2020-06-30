#include "stdafx.h"
#include <sstream>
#include <sal.h>
#include <string>
#include <exception>
#include "BaseException.h"

BaseException::BaseException(_In_ const char* file, _In_ UINT lineNum) noexcept
	: file(file) , line(lineNum)
{

}

const char* BaseException::what() const noexcept
{
	std::stringstream ss;
	ss << GetType() << "_";
	ss << line << "_";
	ss << file << std::endl;
	strBuffer = ss.str();
	return strBuffer.c_str();
}

const int BaseException::LineNum() const noexcept
{
	return line;
}

const char* BaseException::GetType() const noexcept
{
	strBuffer.clear();
	strBuffer.append("BaseException");
	return strBuffer.c_str();
}

const char* BaseException::OutStr(_In_opt_ std::string what, _In_opt_ std::string xtra) const noexcept
{
	std::stringstream ss;
	ss << what << "_";
	if (!xtra.empty())
		ss << "_" << xtra;
	ss << "." << std::endl;
	strBuffer = ss.str();
	return strBuffer.c_str();
}

const std::string& BaseException::GetFile() const noexcept
{
	return file;
}

std::string BaseException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}