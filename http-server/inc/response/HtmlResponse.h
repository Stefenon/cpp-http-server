#include "Response.h"
#include "html/HtmlDocument.h"

class HtmlResponse : public Response
{
public:
  HtmlResponse(HtmlDocument &new_content, HttpStatusCode new_status_code);

  std::string content_to_string() const override;
  void set_content_from_string(std::string &new_content) override;

  HtmlDocument html_content;
};
