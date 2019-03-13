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

#include "AsyncConnectionHandler.hpp"

namespace oatpp { namespace websocket {

const v_int32 AsyncConnectionHandler::THREAD_NUM_DEFAULT = OATPP_ASYNC_EXECUTOR_THREAD_NUM_DEFAULT;

void AsyncConnectionHandler::handleConnection(const std::shared_ptr<oatpp::data::stream::IOStream>& connection) {
  
  class SocketCoroutine : public oatpp::async::Coroutine<SocketCoroutine> {
  private:
    std::shared_ptr<oatpp::data::stream::IOStream> m_connection;
    std::shared_ptr<SocketInstanceListener> m_listener;
    std::shared_ptr<AsyncWebSocket> m_socket;
  public:
    
    SocketCoroutine(const std::shared_ptr<oatpp::data::stream::IOStream>& connection,
                    const std::shared_ptr<SocketInstanceListener>& listener)
      : m_connection(connection)
      , m_listener(listener)
      , m_socket(nullptr)
    {}
    
    ~SocketCoroutine() {
      if(m_listener && m_socket) {
        m_listener->onBeforeDestroy_NonBlocking(m_socket);
      }
    }
    
    Action act() override {
      m_socket = AsyncWebSocket::createShared(m_connection, false);
      if(m_listener) {
        m_listener->onAfterCreate_NonBlocking(m_socket);
      }
      return m_socket->listenAsync(this, finish());
    }
    
  };
  
  m_executor->execute<SocketCoroutine>(connection, m_listener);
  
}
  
}}