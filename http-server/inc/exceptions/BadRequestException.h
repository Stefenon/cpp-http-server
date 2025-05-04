#pragma once

#include "CustomException.h"

class BadRequestException : public CustomException {
    public:
        BadRequestException(const std::string& new_message) : CustomException(new_message) {}
};