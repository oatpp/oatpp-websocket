//
// Created by Leonid  on 2019-03-25.
//

#include "FullTest.hpp"

#include "app/Controller.hpp"
#include "app/WebSocketListener.hpp"

#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/ConnectionHandler.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/Interface.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include "oatpp/macro/component.hpp"

namespace oatpp { namespace test { namespace websocket {

namespace {

//#define OATPP_TEST_USE_PORT 8000

class TestComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, virtualInterface)([] {
    return oatpp::network::virtual_::Interface::obtainShared("virtualhost");
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
#ifdef OATPP_TEST_USE_PORT
    return oatpp::network::server::SimpleTCPConnectionProvider::createShared(OATPP_TEST_USE_PORT);
#else
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, interface);
    return oatpp::network::virtual_::server::ConnectionProvider::createShared(interface);
#endif
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    auto objectMapper = std::make_shared<oatpp::json::ObjectMapper>();
    objectMapper->deserializerConfig().mapper.allowUnknownFields = false;
    return objectMapper;
  }());

  /**
   *  Create websocket connection handler
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::websocket::ConnectionHandler>, websocketConnectionHandler)([] {
    auto connectionHandler = oatpp::websocket::ConnectionHandler::createShared();
    connectionHandler->setSocketInstanceListener(std::make_shared<app::WebSocketInstanceListener>());
    return connectionHandler;
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider)([this] {
#ifdef OATPP_TEST_USE_PORT
    return oatpp::network::client::SimpleTCPConnectionProvider::createShared("127.0.0.1", OATPP_TEST_USE_PORT);
#else
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, interface);
    return oatpp::network::virtual_::client::ConnectionProvider::createShared(interface);
#endif
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::websocket::Connector>, connector)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);
    return oatpp::websocket::Connector::createShared(clientConnectionProvider);
  }());

};

class ClientWebSocketListener : public oatpp::websocket::WebSocket::Listener {
private:
  static constexpr const char *const TAG = "ClientWebSocketListener";
private:
  oatpp::data::stream::BufferOutputStream m_messageBuffer;
public:

  void onPing(const WebSocket &socket, const oatpp::String &message) override {
    OATPP_LOGd(TAG, "Ping frame received. Sending Pong back.");
    socket.sendPong("");
  }

  void onPong(const WebSocket &socket, const oatpp::String &message) override {
    OATPP_LOGd(TAG, "Pong frame received. Do nothing.");
    // DO NOTHING
  }

  void onClose(const WebSocket &socket, v_uint16 code, const oatpp::String &message) override {
    OATPP_LOGd(TAG, "Close frame received. Code={}, Message='{}'", code, message);
  }

  /**
   * Called when "text" or "binary" frame received.
   * When all data of message is read, readMessage is called again with size == 0 to
   * indicate end of the message
   */
  void readMessage(const WebSocket &socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) override {
    if (size == 0) {
      auto wholeMessage = m_messageBuffer.toString();
      OATPP_LOGd(TAG, "Message='{}'", wholeMessage);
      socket.sendOneFrameText("Hello from oatpp! Your message was: " + wholeMessage);
      m_messageBuffer.setCurrentPosition(0);
    } else if (size > 0) {
      m_messageBuffer.writeSimple(data, size);
    }
  };

};

}

void FullTest::onRun() {

  TestComponent component;

  oatpp::test::web::ClientServerTestRunner runner;

  runner.addController(app::Controller::createShared());

  runner.run([] {


    OATPP_COMPONENT(std::shared_ptr<oatpp::websocket::Connector>, connector);

    auto connection = connector->connect("ws");
    oatpp::websocket::WebSocket socket(connection, true);

    socket.setListener(std::make_shared<ClientWebSocketListener>());

    //std::this_thread::sleep_for(std::chrono::minutes(10));

  }, std::chrono::minutes(10));

}

}}}