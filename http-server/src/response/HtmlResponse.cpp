#include "response/HtmlResponse.h"

HtmlResponse::HtmlResponse(HtmlDocument &new_content, HttpStatusCode new_status_code) : Response(new_content.to_string(), new_status_code, "text/html")
{
  html_content = new_content;
}

void HtmlResponse::set_content_from_string(std::string &new_content)
{
  Response::set_content_from_string(new_content);
}

std::string HtmlResponse::content_to_string() const
{
  return html_content.to_string();
}
