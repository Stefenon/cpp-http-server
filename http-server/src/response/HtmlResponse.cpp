#include "response/HtmlResponse.h"

void HtmlResponse::set_content_from_string(std::string &new_content)
{
  Response::set_content_from_string(new_content);
}

std::string HtmlResponse::content_to_string() const
{
  return content;
}
