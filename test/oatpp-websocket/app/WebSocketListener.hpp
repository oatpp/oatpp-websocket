//
// Created by Leonid  on 2019-03-26.
//

#ifndef oatpp_test_websocket_app_WebSocketListener_hpp
#define oatpp_test_websocket_app_WebSocketListener_hpp

#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/WebSocket.hpp"

#include "oatpp/data/stream/BufferStream.hpp"

namespace oatpp { namespace test { namespace websocket { namespace app {

class WebSocketListener : public oatpp::websocket::WebSocket::Listener {
private:
  static constexpr const char *const TAG = "WebSocketListener";
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

class WebSocketInstanceListener : public oatpp::websocket::ConnectionHandler::SocketInstanceListener {
private:
  static constexpr const char *const TAG = "SocketInstanceListener";
public:

  /**
   *  Called when socket is created
   */
  void onAfterCreate(const oatpp::websocket::WebSocket &socket, const std::shared_ptr<const ParameterMap>& params) override {
    OATPP_LOGd(TAG, "New incoming connection {}", reinterpret_cast<v_uint64>(std::addressof(socket)));

    // set listener
    socket.setListener(std::make_shared<WebSocketListener>());
  }

  /**
   *  Called before socket instance is destroyed.
   */
  void onBeforeDestroy(const oatpp::websocket::WebSocket &socket) override {
    OATPP_LOGd(TAG, "Closing connection {}", reinterpret_cast<v_uint64>(std::addressof(socket)));
  }

};

}}}}


#endif //oatpp_test_websocket_app_WebSocketListener_hpp
