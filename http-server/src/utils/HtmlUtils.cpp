#include "utils/HtmlUtils.h"

std::string Html::render(std::filesystem::path html_template, std::unordered_map<std::string, std::string> context)
{
  std::ifstream file(html_template);

  if (!file.is_open())
  {
    throw std::runtime_error("Could not open template: " + html_template.generic_string());
  }

  std::stringstream stream;
  stream << file.rdbuf();

  std::string html_str = stream.str();
  ssize_t context_var_start = 0;
  ssize_t context_var_end = 0;

  while (true)
  {
    context_var_start = html_str.substr(context_var_end).find("{{");

    if (context_var_start == std::string::npos)
    {
      break;
    }

    context_var_start += context_var_end;

    context_var_end = html_str.find("}}");
    std::cout << context_var_start << ", " << context_var_end << std::endl;
    if (context_var_end == std::string::npos)
    {
      throw std::runtime_error("Malformed HTML template. Variable scope was never closed.");
    }
    std::string context_var_key = html_str.substr(context_var_start + 2, context_var_end - (context_var_start + 2));

    auto it = context.find(context_var_key);
    if (it == context.end())
    {
      throw std::runtime_error("Variable \"" + context_var_key + "\" not found in context");
    }
    std::string context_variable = it->second;

    html_str.replace(context_var_start, context_var_end - context_var_start + 2, context_variable);
  }

  return html_str;
}