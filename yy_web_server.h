/*

  MIT License

  Copyright (c) 2024-2025 Yafiyogi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#pragma once

#include <memory>

#include "CivetServer.h"

#include "yy_cpp/yy_init.h"
#include "yy_cpp/yy_flat_map.h"
#include "yy_cpp/yy_vector.h"
#include "yy_cpp/yy_utility.h"

namespace yafiyogi::yy_web {

struct CivetWebLib final
{
    void init() noexcept
    {
      mg_init_library(0);
    }

    void tear_down() noexcept
    {
      mg_exit_library();
    }
};

using CivetHandlerPtr = std::unique_ptr<CivetHandler>;

class WebServer final
{
  public:
    using option_store = std::vector<std::string>;

    static constexpr const std::string_view civetweb_options_no{"no"};
    static constexpr const std::string_view civetweb_options_yes{"yes"};
    static constexpr const std::string_view decode_query_string{"decode_query_string"};
    static constexpr const std::string_view decode_url{"decode_url"};
    static constexpr const std::string_view document_root{"document_root"};
    static constexpr const std::string_view enable_directory_listing{"enable_directory_listing"};
    static constexpr const std::string_view enable_http2{"enable_http2"};
    static constexpr const std::string_view enable_keep_alive{"enable_keep_alive"};
    static constexpr const std::string_view keep_alive_timeout_ms{"keep_alive_timeout_ms"};
    static constexpr const std::string_view linger_timeout_ms{"linger_timeout_ms"};
    static constexpr const std::string_view listening_ports{"listening_ports"};
    static constexpr const std::string_view tcp_nodelay{"tcp_nodelay"};

    class Options
    {
      public:
        void Add(std::string_view option, std::string_view value)
        {
          m_options.emplace_back(option);
          m_options.emplace_back(value);
        }

        void Add(std::string && option, std::string &&  value)
        {
          m_options.emplace_back(std::move(option));
          m_options.emplace_back(std::move(value));
        }

        const option_store & Args() const
        {
          return m_options;
        }


      private:
        option_store m_options{};
    };

    explicit WebServer(Options & options) noexcept;
    WebServer() = delete;
    WebServer(const WebServer &) = delete;
    WebServer(WebServer &&) noexcept = default;
    ~WebServer() noexcept;

    WebServer & operator=(const WebServer &) = delete;
    WebServer & operator=(WebServer &&) noexcept = default;

    bool AddHandler(std::string uri,
                    CivetHandlerPtr && p_handler);

    template<typename InputHandlerType>
    bool AddHandler(std::string uri,
                    std::unique_ptr<InputHandlerType> && p_handler)
    {
      using HandlerPtr = std::unique_ptr<InputHandlerType>;
      static_assert(std::is_base_of_v<CivetHandler, InputHandlerType>,
                    "p_handler must be of type std::unique_ptr<CivitHandler>.");
      return AddHandler(std::move(uri),
                        yy_util::static_unique_cast<CivetHandler>(std::forward<HandlerPtr>(p_handler)));
    }

    template<typename InputHandlerType>
    bool AddHandler(std::string_view uri,
                    std::unique_ptr<InputHandlerType> && p_handler)
    {
      return AddHandler(std::string{uri}, std::forward<std::unique_ptr<InputHandlerType>>(p_handler));
    }

  private:
    using lib_type = yy_util::Init<CivetWebLib>;
    using lib_ptr = std::unique_ptr<lib_type>;
    lib_ptr m_lib;

    using server_type = CivetServer;
    using server_ptr = std::unique_ptr<server_type>;
    server_ptr m_server;

    using handlers_type = yy_data::flat_map<std::string, CivetHandlerPtr, yy_data::ClearAction::Clear, yy_data::ClearAction::Clear>;
    handlers_type m_handlers;
};

} // namespace yafiyogi::yy_web
