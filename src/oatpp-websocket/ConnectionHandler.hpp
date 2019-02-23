/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi, <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_websocket_ConnectionHandler_hpp
#define oatpp_websocket_ConnectionHandler_hpp

#include "WebSocket.hpp"

#include "oatpp/network/server/ConnectionHandler.hpp"

namespace oatpp { namespace websocket {
  
class ConnectionHandler : public oatpp::base::Controllable, public network::server::ConnectionHandler {
public:
  class SocketInstanceListener {
  public:
    /**
     *  Called when socket is created
     */
    virtual void onAfterCreate(const WebSocket& socket) = 0;
    
    /**
     *  Called before socket instance is destroyed.
     */
    virtual void onBeforeDestroy(const WebSocket& socket) = 0;
  };
private:
  std::shared_ptr<SocketInstanceListener> m_listener;
public:
  ConnectionHandler()
    : m_listener(nullptr)
  {}
public:
  
  static std::shared_ptr<ConnectionHandler> createShared(){
    return std::make_shared<ConnectionHandler>();
  }
  
  void setSocketInstanceListener(const std::shared_ptr<SocketInstanceListener>& listener) {
    m_listener = listener;
  }
  
  void handleConnection(const std::shared_ptr<oatpp::data::stream::IOStream>& connection) override;

  void stop() override {
    // DO NOTHING
  }
  
};
  
}}

#endif /* oatpp_websocket_ConnectionHandler_hpp */
