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

#ifndef oatpp_websocket_AsyncConnectionHandler_hpp
#define oatpp_websocket_AsyncConnectionHandler_hpp

#include "./AsyncWebSocket.hpp"

#include "oatpp/core/async/Executor.hpp"
#include "oatpp/network/server/ConnectionHandler.hpp"

namespace oatpp { namespace websocket {
  
class AsyncConnectionHandler : public base::Countable, public network::server::ConnectionHandler {
public:
  static const v_int32 THREAD_NUM_DEFAULT;
public:
  
  class SocketInstanceListener {
  public:
    
    /**
     *  Called when socket is created
     *  This method should not block
     */
    virtual void onAfterCreate_NonBlocking(const std::shared_ptr<AsyncWebSocket>& socket) = 0;
    
    /**
     *  Called before socket instance is destroyed.
     *  This method should not block
     */
    virtual void onBeforeDestroy_NonBlocking(const std::shared_ptr<AsyncWebSocket>& socket) = 0;
  };
  
private:
  std::shared_ptr<oatpp::async::Executor> m_executor;
  std::shared_ptr<SocketInstanceListener> m_listener;
public:
  
  AsyncConnectionHandler(v_int32 threadCount = THREAD_NUM_DEFAULT)
    : m_executor(std::make_shared<oatpp::async::Executor>(threadCount))
    , m_listener(nullptr)
  {
    m_executor->detach();
  }
  
  AsyncConnectionHandler(const std::shared_ptr<oatpp::async::Executor>& executor)
    : m_executor(executor)
    , m_listener(nullptr)
  {}
public:
  
  static std::shared_ptr<AsyncConnectionHandler> createShared(v_int32 threadCount = THREAD_NUM_DEFAULT){
    return std::make_shared<AsyncConnectionHandler>(threadCount);
  }
  
  static std::shared_ptr<AsyncConnectionHandler> createShared(const std::shared_ptr<oatpp::async::Executor>& executor){
    return std::make_shared<AsyncConnectionHandler>(executor);
  }
  
  void setSocketInstanceListener(const std::shared_ptr<SocketInstanceListener>& listener) {
    m_listener = listener;
  }
  
  void handleConnection(const std::shared_ptr<oatpp::data::stream::IOStream>& connection) override;
  
  void stop() override {
    m_executor->stop();
  }
  
};
  
}}

#endif /* oatpp_websocket_AsyncConnectionHandler_hpp */
