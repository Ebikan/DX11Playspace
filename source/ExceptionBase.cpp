#include "stdafx.h"
#include <sstream>
#include <sal.h>
#include <string>
#include <exception>
#include "ExceptionBase.h"

ExceptionBase::ExceptionBase(_In_ const char* file, _In_ UINT lineNum) noexcept
	: file(file) ,lineNumSto(lineNum)
{

}

const char* ExceptionBase::what() const noexcept
{
	std::stringstream ss;
	ss << GetType() << "_";
	ss << lineNumSto << "_";
	ss << file << std::endl;
	strBuffer = ss.str();
	return strBuffer.c_str();
}

const int ExceptionBase::lineNum() const noexcept
{
	return lineNumSto;
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