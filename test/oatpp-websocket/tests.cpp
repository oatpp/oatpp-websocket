
#include "oatpp-test/UnitTest.hpp"

#include "FullAsyncTest.hpp"
#include "FullTest.hpp"

#include "oatpp/concurrency/SpinLock.hpp"
#include "oatpp/Environment.hpp"

#include "oatpp-websocket/Handshaker.hpp"

#include <iostream>

namespace {

class Test : public oatpp::test::UnitTest {
public:
  Test() : oatpp::test::UnitTest("MyTag")
  {}

  void onRun() override {

    oatpp::websocket::Handshaker::Headers headers;
    oatpp::websocket::Handshaker::clientsideHandshake(headers);

  }
};

void runTests() {
  //OATPP_RUN_TEST(oatpp::test::websocket::FullTest);
  OATPP_RUN_TEST(oatpp::test::websocket::FullAsyncTest);
}

}

int main() {

  oatpp::Environment::init();

  runTests();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::Environment::getObjectsCount() == 0);

  oatpp::Environment::destroy();

  return 0;
}
