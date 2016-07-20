#include "RESTRequestHandler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "Json.h"
#include "RestApi.h"

using namespace Fix::Server;

RESTRequestHandler::RESTRequestHandler(RestApi &api)
  : restApi{api}
{}

void RESTRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  auto uri = request.getURI();
  auto method = request.getMethod();
  std::ostringstream stream;
  request.write(stream);
  auto content = stream.str();

  auto apiResponse = restApi.process(uri, method, content);

  response.setChunkedTransferEncoding(true);
  response.setContentType("application/json");

  std::ostream& ostr = response.send();
  ostr << apiResponse;
}
