// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

/**
 * @file server.cpp
 * @brief A server to receive EchoRequest and send back EchoResponse.
 * @author wangpengcheng  (wangpengcheng2018@gmail.com)
 * @version 1.0
 * @date 2021-05-06 17:00:33
 * @copyright Copyright (c) 2021  IRLSCU
 * 
 * @par 修改日志:
 * <table>
 * <tr>
 *    <th> Commit date</th>
 *    <th> Version </th> 
 *    <th> Author </th>  
 *    <th> Description </th>
 * </tr>
 * <tr>
 *    <td> 2021-05-06 17:00:33 </td>
 *    <td> 1.0 </td>
 *    <td> wangpengcheng </td>
 *    <td> 添加注释 </td>
 * </tr>
 * </table>
 */

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <iostream>
#include <stdio.h>
#include "echo.pb.h"

/**
 * @brief 是否输出 Echo attachment as well
 */
DEFINE_bool(echo_attachment, true, "Echo attachment as well");
/**
 * @brief 指定端口
 */
DEFINE_int32(port, 8000, "TCP Port of this server");
/**
 * @brief gflag 定时清理关闭连接参数
 */
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
                                 "read/write operations during the last `idle_timeout_s'");
/**
 * @brief 输出日志时间
 */
DEFINE_int32(logoff_ms, 2000, "Maximum duration of server's LOGOFF state "
                              "(waiting for client to close connection before server stops)");

namespace example
{

    /**
     * @brief 
     * Your implementation of example::EchoService
     * Notice that implementing brpc::Describable grants the ability to put
     * additional information in /status.
     * @see example::EchoService and @see brpc::Describable
     */
    class EchoServiceImpl : public EchoService
    {
    public:
        /**
         * @brief 基础构造函数
         */
        EchoServiceImpl(){};
        /**
         * @brief 虚析构函数，防止子类析构时内存泄露
         */
        virtual ~EchoServiceImpl(){};
        /**
         * @brief  主要RPC接口函数，基于protobuff 中的server进行改造
         * @param  cntl_base        控制句柄
         * @param  request          请求数据结构体
         * @param  response         响应数据结构体
         * @param  done             是否关闭
         */
        virtual void Echo(google::protobuf::RpcController *cntl_base,
                          const EchoRequest *request,
                          EchoResponse *response,
                          google::protobuf::Closure *done)
        {
            /*!
            This object helps you to call done->Run() in RAII style. If you need
            to process the request asynchronously, pass done_guard.release().
            */
            
            brpc::ClosureGuard done_guard(done);
            // 指针类型向下转换
            brpc::Controller *cntl =
                static_cast<brpc::Controller *>(cntl_base);

            // The purpose of following logs is to help you to understand
            // how clients interact with servers more intuitively. You should
            // remove these logs in performance-sensitive servers.
            // 打印日志信息
            LOG(INFO) << "Received request[log_id=" << cntl->log_id()
                      << "] from " << cntl->remote_side()
                      << " to " << cntl->local_side()
                      << ": " << request->message()
                      << " (attached=" << cntl->request_attachment() << ")";

            // Fill response.
            response->set_message(request->message());

            // You can compress the response by setting Controller, but be aware
            // that compression may be costly, evaluate before turning on.
            // cntl->set_response_compress_type(brpc::COMPRESS_TYPE_GZIP);

            if (FLAGS_echo_attachment)
            {
                // Set attachment which is wired to network directly instead of
                // being serialized into protobuf messages.
                cntl->response_attachment().append(cntl->request_attachment());
            }
        }
    };
} // namespace example

int main(int argc, char *argv[])
{

    printf("hello word\n");
    int a = 0;
    a = a + 1;
    std::cout << "hello word" << std::endl;
    std::cout << a << std::endl;
    // Parse gflags. We recommend you to use gflags as well.
    GFLAGS_NS::ParseCommandLineFlags(&argc, &argv, true);

    // Generally you only need one Server.
    brpc::Server server;

    // Instance of your service.
    example::EchoServiceImpl echo_service_impl;

    // Add the service into server. Notice the second parameter, because the
    // service is put on stack, we don't want server to delete it, otherwise
    // use brpc::SERVER_OWNS_SERVICE.
    if (server.AddService(&echo_service_impl,
                          brpc::SERVER_DOESNT_OWN_SERVICE) != 0)
    {
        LOG(ERROR) << "Fail to add service";
        return -1;
    }

    // Start the server.
    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    if (server.Start(FLAGS_port, &options) != 0)
    {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }

    // Wait until Ctrl-C is pressed, then Stop() and Join() the server.
    server.RunUntilAskedToQuit();
    return 0;
}
