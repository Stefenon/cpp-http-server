#include "inc/server/HttpServer.h"
#include "inc/html/HtmlDocument.h"
#include "inc/router/HttpRouter.h"
#include "inc/response/HtmlResponse.h"
#include "inc/response/JsonResponse.h"

#include <thread>
#include <chrono>

using namespace std;

constexpr int PORT = 5000;
constexpr int CONNECTION_QUEUE_SIZE = 100;
constexpr int BUFFER_SIZE = 1024;

static JsonResponse apply_delay(Request req)
{
	std::unordered_map<std::string, std::string> params = req.get_path_params();

	int delay = std::stoi(params.at("delay"));

	std::this_thread::sleep_for(std::chrono::seconds(delay));

	nlohmann::json res_body;

	res_body["delay"] = delay;

	return JsonResponse(res_body, HttpStatusCode::HTTP_200_OK);
}

static HtmlResponse get_html(Request req)
{
	std::unordered_multimap<std::string, std::string> tags = req.get_query_params();

	HtmlDocument doc = HtmlDocument();
	doc.head.add_child_element(HtmlElement("title", "Title 123"));
	doc.body.add_child_element(HtmlElement("h1", "Header 345", {{"class", "test"}}));

	HtmlElement new_div = HtmlElement("div");
	new_div.set_text("One more test");

	for (const auto &query_param : tags)
	{
		HtmlElement header_1 = HtmlElement("h1", query_param.first, {{"class", "header_1"}});
		HtmlElement header_2 = HtmlElement("h2", query_param.second, {{"class", "header_2"}});
		header_2.set_tail("Text after header");
		header_2.set_attribute("new_attr", "abc");
		new_div.add_child_element(header_1);
		new_div.add_child_element(header_2);
	}

	doc.body.add_child_element(new_div);

	return HtmlResponse(doc, HttpStatusCode::HTTP_202_ACCEPTED);
}

static JsonResponse post_json(Request req)
{
	std::cout << "inside endpoint function" << std::endl;
	for (const auto &[k, v] : req.get_path_params())
	{
		std::cout << k << "=" << v << std::endl;
	}
	nlohmann::json body;
	const std::string json_str = req.get_body_as_str();

	if (!json_str.empty())
	{
		body = nlohmann::json::parse(json_str);
	}
	else
	{
		body["numeric_field"] = 123;
		body["text_field"] = 123;
		nlohmann::json object_field;
		object_field["field_a"] = 1;
		object_field["field_b"] = "B";
		body["object_field"] = object_field;
		std::vector<int> array_field = {1, 2, 3, 4};
		body["array_field"] = array_field;
	}

	return JsonResponse(body, HttpStatusCode::HTTP_200_OK);
}

static JsonResponse post_json_with_params(Request req)
{
	nlohmann::json body;
	const std::string json_str = req.get_body_as_str();

	if (!json_str.empty())
	{
		body = nlohmann::json::parse(json_str);
	}
	else
	{
		body["numeric_field"] = 123;
		body["text_field"] = 123;
		nlohmann::json object_field;
		object_field["field_a"] = 1;
		object_field["field_b"] = "B";
		body["object_field"] = object_field;
		std::vector<int> array_field = {1, 2, 3, 4};
		body["array_field"] = array_field;
	}

	std::unordered_map<std::string, std::string> path_params = req.get_path_params();

	body["param_1"] = path_params["param_1"];
	body["param_2"] = path_params["param_2"];

	return JsonResponse(body, HttpStatusCode::HTTP_201_CREATED);
}

int main()
{
	cout << "Create HTTP router" << endl;
	HttpRouter router;
	router.get("/{delay}", apply_delay);
	router.get("/html", get_html);
	router.post("/json", post_json);
	router.post("/json/{param_1}/{param_2}", post_json_with_params);

	cout << "Create HTTP server instance" << endl;
	HttpServer server(PORT, CONNECTION_QUEUE_SIZE, BUFFER_SIZE);
	server.set_router(router);

	cout << "Start listening" << endl;
	server.start();

	return 0;
}
