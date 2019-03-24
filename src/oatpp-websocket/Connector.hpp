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

#ifndef oatpp_websocket_Connector_hpp
#define oatpp_websocket_Connector_hpp

#include "oatpp/web/client/HttpRequestExecutor.hpp"

namespace oatpp { namespace websocket {

/**
 * Helper class to do client websocket connect.
 */
class Connector {
public:
  /**
   * Convenience typedef for &id:oatpp::data::stream::IOStream;.
   */
  typedef oatpp::data::stream::IOStream Connection;

  /**
   * Convenience typedef for &id:oatpp::async::Action;.
   */
  typedef oatpp::async::Action Action;
  typedef Action (oatpp::async::AbstractCoroutine::*AsyncCallback)(const std::shared_ptr<Connection>&);
private:
  std::shared_ptr<oatpp::network::ClientConnectionProvider> m_connectionProvider;
  oatpp::web::client::HttpRequestExecutor m_requestExecutor;
public:

  /**
   * Constructor.
   * @param connectionProvider - &id:oatpp::network::ClientConnectionProvider;.
   */
  Connector(const std::shared_ptr<oatpp::network::ClientConnectionProvider>& connectionProvider);
public:

  /**
   * Create shared Connector.
   * @param connectionProvider - &id:oatpp::network::ClientConnectionProvider;.
   * @return - `std::shared_ptr` to Connector.
   */
  static std::shared_ptr<Connector> createShared(const std::shared_ptr<oatpp::network::ClientConnectionProvider>& connectionProvider) {
    return std::make_shared<Connector>(connectionProvider);
  }

  /**
   * Connect to server, do websocket-handshake and return Connection which can be then passed to WebSocket constructor. <br>
   * **(Blocking call)**
   * @param path - path to websocket endpoint.
   * @return - &l:Connector::Connection;.
   */
  std::shared_ptr<Connection> connect(const oatpp::String& path);

  /**
   * Connect to server, do websocket-handshake and return Connection which can be then passed to WebSocket constructor. <br>
   * **(Async call)**
   * @param parentCoroutine - caller coroutine. &id:oatpp::async::AbstractCoroutine;.
   * @param callback - pointer to callback function.
   * @param path - path to websocket endpoint.
   * @return - &id:oatpp::async::Action;.
   */
  Action connectAsync(oatpp::async::AbstractCoroutine* parentCoroutine, AsyncCallback callback, const oatpp::String& path);
  
};
  
}}

#endif /* oatpp_websocket_Connector_hpp */
