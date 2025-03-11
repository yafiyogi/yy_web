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

#include "fmt/format.h"
#include "fmt/compile.h"
#include "spdlog/spdlog.h"

#include "yy_web_server.h"

namespace yafiyogi::yy_web {

using namespace std::string_view_literals;
using namespace fmt::literals;

WebServer::WebServer(Options & options) noexcept:
  m_lib(std::make_unique<lib_type>()), // m_lib must be created before server!
  m_server(std::make_unique<server_type>(options.Args()))
{
}

WebServer::~WebServer() noexcept
{
  if(m_server)
  {
    m_handlers.visit([this](auto & uri, auto & handler) {
      m_server->removeHandler(uri);
      handler.reset();
    });
  }

  m_server.reset();
  m_lib.reset(); // m_lib must be reset after server!
}


bool WebServer::AddHandler(std::string uri,
                           CivetHandlerPtr && p_handler)
{
  if(m_server)
  {
    if(!uri.starts_with("/"sv))
    {
      uri = fmt::format("/{}"_cf, uri);
    }
    spdlog::debug("Web Server: adding handler at [{}]"sv, uri);


    auto l_handler = p_handler.get();
    auto [ignored, inserted] = m_handlers.emplace(uri, std::move(p_handler));

    if(inserted)
    {
      m_server->addHandler(uri, l_handler);
    }

    return inserted;
  }
  return false;
}

} // namespace yafiyogi::yy_web
