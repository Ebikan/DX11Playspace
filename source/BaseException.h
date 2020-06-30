#pragma once
#include <string>
#include <exception>


class BaseException : public std::exception
{
public:
	BaseException(_In_ const char* file, _In_ UINT lineNum) noexcept;
	const char* what() const noexcept override;
	const int LineNum() const noexcept;
	virtual const char* GetType() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
	virtual const char* OutStr(_In_opt_ std::string EXType = "BaseException", _In_opt_ std::string xtra = "") const noexcept;
private:
	const char* file;
	const UINT line;
protected:
	// localized buffer for string output and processing.
	mutable std::string strBuffer;
};

