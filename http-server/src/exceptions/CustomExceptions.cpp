#include "exceptions/CustomExceptions.h"

const char* CustomException::what() const noexcept
{
	return message.c_str();
}
