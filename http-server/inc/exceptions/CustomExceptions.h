#pragma once

#include <exception>
#include <string>

class CustomException : public std::exception
{
protected:
	std::string message;

public:
	const char *what() const noexcept override;
	CustomException(const std::string &new_message) : message(new_message) {}
};

class BadRequestException : public CustomException
{
public:
	BadRequestException(const std::string &new_message) : CustomException(new_message) {}
};

class EndpointNotFoundException : public CustomException
{
public:
	EndpointNotFoundException(const std::string &new_message) : CustomException(new_message) {}
};

class MethodNotAllowedException : public CustomException
{
public:
	MethodNotAllowedException(const std::string &new_message) : CustomException(new_message) {}
};

class PayloadTooLargeException : public CustomException
{
public:
	PayloadTooLargeException(const std::string &new_message) : CustomException(new_message) {}
};

class RequestHeaderFieldsTooLargeException : public CustomException
{
public:
	RequestHeaderFieldsTooLargeException(const std::string &new_message) : CustomException(new_message) {}
};