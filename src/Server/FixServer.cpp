#include "FixServer.h"

#include "Poco/File.h"
#include "Poco/Path.h"

using namespace Fix::Server;

FixServer::FixServer(RestApi& api)
    : restApi{api}
{}

int FixServer::main(const std::vector<std::string> &args) {
  createFixDirectory();
  serve();
  return Application::EXIT_OK;
}

void FixServer::createFixDirectory() const {
  using Poco::Path;
  Path fixDirPath{Path::current()};
  fixDirPath.pushDirectory(".fix");
  using Poco::File;
  File fixDir{fixDirPath};
  fixDir.createDirectory();
}

void FixServer::serve() {
  using namespace Poco::Net;
  Poco::UInt16 port = config().getInt("FixServer.port", 8080);
  ServerSocket svs{port};
  HTTPServer srv{new RequestHandlerFactory(restApi), svs, new HTTPServerParams};
  srv.start();
  logger().information("Server started.");
  waitForTerminationRequest();
  srv.stop();
  logger().information("Server stopped.");
}

void FixServer::initialize(Application &self) {
  loadConfiguration();
  ServerApplication::initialize(self);
}
