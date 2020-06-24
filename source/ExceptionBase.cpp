#include "stdafx.h"
#include <sstream>
#include <sal.h>
#include <string>
#include <exception>
#include "ExceptionBase.h"

ExceptionBase::ExceptionBase(_In_ const char* file, _In_ UINT lineNum) noexcept
	: file(file) , line(lineNum)
{

}

const char* ExceptionBase::what() const noexcept
{
	std::stringstream ss;
	ss << GetType() << "_";
	ss << line << "_";
	ss << file << std::endl;
	strBuffer = ss.str();
	return strBuffer.c_str();
}

const int ExceptionBase::LineNum() const noexcept
{
	return line;
}

const char* ExceptionBase::GetType() const noexcept
{
	strBuffer.clear();
	strBuffer.append("ExceptionBase");
	return strBuffer.c_str();
}

const char* ExceptionBase::OutStr(_In_opt_ std::string what, _In_opt_ std::string xtra) const noexcept
{
	std::stringstream ss;
	ss << what << "_";
	if (!xtra.empty())
		ss << "_" << xtra;
	ss << "." << std::endl;
	strBuffer = ss.str();
	return strBuffer.c_str();
}

const std::string& ExceptionBase::GetFile() const noexcept
{
	return file;
}

std::string ExceptionBase::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}