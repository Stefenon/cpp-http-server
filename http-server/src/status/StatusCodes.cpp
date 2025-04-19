#include "status/StatusCodes.h"

const std::unordered_map<HttpStatusCode, std::string> status_messages = {
    // 1xx Informational
    {HttpStatusCode::HTTP_100_CONTINUE, "Continue"},
    {HttpStatusCode::HTTP_101_SWITCHING_PROTOCOLS, "Switching Protocols"},
    {HttpStatusCode::HTTP_102_PROCESSING, "Processing"},
    {HttpStatusCode::HTTP_103_EARLY_HINTS, "Early Hints"},

    // 2xx Success
    {HttpStatusCode::HTTP_200_OK, "OK"},
    {HttpStatusCode::HTTP_201_CREATED, "Created"},
    {HttpStatusCode::HTTP_202_ACCEPTED, "Accepted"},
    {HttpStatusCode::HTTP_203_NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information"},
    {HttpStatusCode::HTTP_204_NO_CONTENT, "No Content"},
    {HttpStatusCode::HTTP_205_RESET_CONTENT, "Reset Content"},
    {HttpStatusCode::HTTP_206_PARTIAL_CONTENT, "Partial Content"},
    {HttpStatusCode::HTTP_207_MULTI_STATUS, "Multi-Status"},
    {HttpStatusCode::HTTP_208_ALREADY_REPORTED, "Already Reported"},
    {HttpStatusCode::HTTP_226_IM_USED, "IM Used"},

    // 3xx Redirection
    {HttpStatusCode::HTTP_300_MULTIPLE_CHOICES, "Multiple Choices"},
    {HttpStatusCode::HTTP_301_MOVED_PERMANENTLY, "Moved Permanently"},
    {HttpStatusCode::HTTP_302_FOUND, "Found"},
    {HttpStatusCode::HTTP_303_SEE_OTHER, "See Other"},
    {HttpStatusCode::HTTP_304_NOT_MODIFIED, "Not Modified"},
    {HttpStatusCode::HTTP_305_USE_PROXY, "Use Proxy"},
    {HttpStatusCode::HTTP_307_TEMPORARY_REDIRECT, "Temporary Redirect"},
    {HttpStatusCode::HTTP_308_PERMANENT_REDIRECT, "Permanent Redirect"},

    // 4xx Client Error
    {HttpStatusCode::HTTP_400_BAD_REQUEST, "Bad Request"},
    {HttpStatusCode::HTTP_401_UNAUTHORIZED, "Unauthorized"},
    {HttpStatusCode::HTTP_402_PAYMENT_REQUIRED, "Payment Required"},
    {HttpStatusCode::HTTP_403_FORBIDDEN, "Forbidden"},
    {HttpStatusCode::HTTP_404_NOT_FOUND, "Not Found"},
    {HttpStatusCode::HTTP_405_METHOD_NOT_ALLOWED, "Method Not Allowed"},
    {HttpStatusCode::HTTP_406_NOT_ACCEPTABLE, "Not Acceptable"},
    {HttpStatusCode::HTTP_407_PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
    {HttpStatusCode::HTTP_408_REQUEST_TIMEOUT, "Request Timeout"},
    {HttpStatusCode::HTTP_409_CONFLICT, "Conflict"},
    {HttpStatusCode::HTTP_410_GONE, "Gone"},
    {HttpStatusCode::HTTP_411_LENGTH_REQUIRED, "Length Required"},
    {HttpStatusCode::HTTP_412_PRECONDITION_FAILED, "Precondition Failed"},
    {HttpStatusCode::HTTP_413_PAYLOAD_TOO_LARGE, "Payload Too Large"},
    {HttpStatusCode::HTTP_414_URI_TOO_LONG, "URI Too Long"},
    {HttpStatusCode::HTTP_415_UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"},
    {HttpStatusCode::HTTP_416_RANGE_NOT_SATISFIABLE, "Range Not Satisfiable"},
    {HttpStatusCode::HTTP_417_EXPECTATION_FAILED, "Expectation Failed"},
    {HttpStatusCode::HTTP_418_IM_A_TEAPOT, "I'm a teapot"},
    {HttpStatusCode::HTTP_421_MISDIRECTED_REQUEST, "Misdirected Request"},
    {HttpStatusCode::HTTP_422_UNPROCESSABLE_ENTITY, "Unprocessable Entity"},
    {HttpStatusCode::HTTP_423_LOCKED, "Locked"},
    {HttpStatusCode::HTTP_424_FAILED_DEPENDENCY, "Failed Dependency"},
    {HttpStatusCode::HTTP_425_TOO_EARLY, "Too Early"},
    {HttpStatusCode::HTTP_426_UPGRADE_REQUIRED, "Upgrade Required"},
    {HttpStatusCode::HTTP_428_PRECONDITION_REQUIRED, "Precondition Required"},
    {HttpStatusCode::HTTP_429_TOO_MANY_REQUESTS, "Too Many Requests"},
    {HttpStatusCode::HTTP_431_REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large"},
    {HttpStatusCode::HTTP_451_UNAVAILABLE_FOR_LEGAL_REASONS, "Unavailable For Legal Reasons"},

    // 5xx Server Error
    {HttpStatusCode::HTTP_500_INTERNAL_SERVER_ERROR, "Internal Server Error"},
    {HttpStatusCode::HTTP_501_NOT_IMPLEMENTED, "Not Implemented"},
    {HttpStatusCode::HTTP_502_BAD_GATEWAY, "Bad Gateway"},
    {HttpStatusCode::HTTP_503_SERVICE_UNAVAILABLE, "Service Unavailable"},
    {HttpStatusCode::HTTP_504_GATEWAY_TIMEOUT, "Gateway Timeout"},
    {HttpStatusCode::HTTP_505_HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"},
    {HttpStatusCode::HTTP_506_VARIANT_ALSO_NEGOTIATES, "Variant Also Negotiates"},
    {HttpStatusCode::HTTP_507_INSUFFICIENT_STORAGE, "Insufficient Storage"},
    {HttpStatusCode::HTTP_508_LOOP_DETECTED, "Loop Detected"},
    {HttpStatusCode::HTTP_510_NOT_EXTENDED, "Not Extended"},
    {HttpStatusCode::HTTP_511_NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required"}
};

std::string HttpStatus::get_message(HttpStatusCode code)
{
    auto it = status_messages.find(code);
    if (it != status_messages.end()) {
        return it->second;
    }

    throw std::runtime_error("Unknown Status Code");
}

std::string HttpStatus::get_status_line(HttpStatusCode code) {
    return std::to_string(static_cast<int>(code)) + " " + get_message(code);
}

