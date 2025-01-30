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

#include <string_view>

#include "fmt/compile.h"
#include "spdlog/spdlog.h"

#include "yy_web_handler.h"

namespace yafiyogi::yy_web {

using namespace std::string_view_literals;
using namespace fmt::literals;

namespace {

constexpr std::string_view access_log_format{"Web Access from: [{}:{}] to [{}:{}]"sv};

} // anonymous namespace

WebHandler::WebHandler(logger_ptr access_log):
  m_access_log(std::move(access_log))
{
}


bool WebHandler::handleGet(CivetServer * /* server */,
                           struct mg_connection * conn)
{
  const struct mg_request_info * ri = mg_get_request_info(conn);

  logger_ptr logger{m_access_log};
  if(!logger)
  {
    logger = spdlog::default_logger();
  }

  if(logger)
  {
    logger->info(access_log_format,
                 ri->remote_addr,
                 ri->remote_port,
                 ri->local_uri,
                 ri->server_port);
  }

  if(spdlog::level::debug >= spdlog::get_level())
  {
    spdlog::debug("WebHandler::HandleGet()"sv);
    spdlog::debug("\trequest_method = {}"sv, ri->request_method);
    spdlog::debug("\trequest_uri = {}"sv, ri->request_uri);
    spdlog::debug("\tlocal_uri = {}"sv, ri->local_uri);
    spdlog::debug("\thttp_version = {}"sv, ri->http_version);
    //spdlog::debug("\tquery_string = {}"sv, ri->query_string);
    //spdlog::debug("\tremote_user = {}"sv, ri->remote_user);
    spdlog::debug("\tremote_addr = {}"sv, ri->remote_addr);
    spdlog::debug("\tremote_port = {}"sv, ri->remote_port);
    spdlog::debug("\tis_ssl = {}"sv, ri->is_ssl);
    spdlog::debug("\tnum_headers = {}"sv, ri->num_headers);
    if (ri->num_headers > 0) {
      int i;
      for (i = 0; i < ri->num_headers; i++) {
        spdlog::debug("\t\t{} = {}"sv,
                      ri->http_headers[i].name,
                      ri->http_headers[i].value);
      }
    }
    spdlog::debug("");
  }

  return DoGet(conn, ri);
}

} // namespace yafiyogi::yy_web
