#include "Response.h"

class HtmlResponse : public Response
{
public:
  HtmlResponse(std::string &new_content, HttpStatusCode new_status_code) : Response(new_content, new_status_code, "text/html") {};

  std::string content_to_string() const override;
  void set_content_from_string(std::string &new_content) override;
};
