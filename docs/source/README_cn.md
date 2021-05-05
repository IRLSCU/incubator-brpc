[English version](README.md)

[![Build Status](https://api.travis-ci.com/apache/incubator-brpc.svg?branch=master)](https://travis-ci.com/github/apache/incubator-brpc)

# ![brpc](docs/images/logo.png)

百度内最常使用的工业级RPC框架, 有1,000,000+个实例(不包含client)和上千种多种服务。"brpc"的含义是"better RPC"。

你可以使用它：

* 搭建能在**一个端口**支持多协议的服务, 或访问各种服务
  * restful http/https, [h2](https://http2.github.io/http2-spec)/[gRPC](https://grpc.io)。使用brpc的http实现比[libcurl](https://curl.haxx.se/libcurl/)方便多了。从其他语言通过HTTP/h2+json访问基于protobuf的协议.
  * [redis](cn/redis_client.md)和[memcached](cn/memcache_client.md), 线程安全，比官方client更方便。
  * [rtmp](https://github.com/brpc/brpc/blob/master/src/brpc/rtmp.h)/[flv](https://en.wikipedia.org/wiki/Flash_Video)/[hls](https://en.wikipedia.org/wiki/HTTP_Live_Streaming), 可用于搭建[流媒体服务](https://github.com/brpc/media-server).
  * hadoop_rpc(可能开源)
  * 支持[rdma](https://en.wikipedia.org/wiki/Remote_direct_memory_access)(即将开源)
  * 支持[thrift](cn/thrift.md) , 线程安全，比官方client更方便
  * 各种百度内使用的协议: [baidu_std](cn/baidu_std.md), [streaming_rpc](cn/streaming_rpc.md), hulu_pbrpc, [sofa_pbrpc](https://github.com/baidu/sofa-pbrpc), nova_pbrpc, public_pbrpc, ubrpc和使用nshead的各种协议.
  * 基于工业级的[RAFT算法](https://raft.github.io)实现搭建[高可用](https://en.wikipedia.org/wiki/High_availability)分布式系统，已在[braft](https://github.com/brpc/braft)开源。
* Server能[同步](cn/server.md)或[异步](cn/server.md#异步service)处理请求。
* Client支持[同步](cn/client.md#同步访问)、[异步](cn/client.md#异步访问)、[半同步](cn/client.md#半同步)，或使用[组合channels](cn/combo_channel.md)简化复杂的分库或并发访问。
* [通过http界面](cn/builtin_service.md)调试服务, 使用[cpu](cn/cpu_profiler.md), [heap](cn/heap_profiler.md), [contention](cn/contention_profiler.md) profilers.
* 获得[更好的延时和吞吐](cn/overview.md#更好的延时和吞吐).
* 把你组织中使用的协议快速地[加入brpc](cn/new_protocol.md)，或定制各类组件, 包括[命名服务](cn/load_balancing.md#命名服务) (dns, zk, etcd), [负载均衡](cn/load_balancing.md#负载均衡) (rr, random, consistent hashing)

# 试一下!

* 通过[概述](cn/overview.md)了解哪里可以用brpc及其优势。
* 阅读[编译步骤](cn/getting_started.md)了解如何开始使用, 之后可以运行一下[示例程序](https://github.com/brpc/brpc/tree/master/example/).
* 文档:
  * [性能测试](cn/benchmark.md)
  * [bvar](cn/bvar.md)
    * [bvar_c++](cn/bvar_c++.md)
  * [bthread](cn/bthread.md)
    * [bthread or not](cn/bthread_or_not.md)
    * [thread-local](cn/thread_local.md)
    * [Execution Queue](cn/execution_queue.md)
  * Client
    * [基础功能](cn/client.md)
    * [错误码](cn/error_code.md)
    * [组合channels](cn/combo_channel.md)
    * [访问http/h2](cn/http_client.md)
    * [访问gRPC](cn/http_derivatives.md#h2grpc)
    * [访问thrift](cn/thrift.md#client端访问thrift-server)
    * [访问UB](cn/ub_client.md)
    * [Streaming RPC](cn/streaming_rpc.md)
    * [访问redis](cn/redis_client.md)
    * [访问memcached](cn/memcache_client.md)
    * [Backup request](cn/backup_request.md)
    * [Dummy server](cn/dummy_server.md)
  * Server
    * [基础功能](cn/server.md)
    * [搭建http/h2服务](cn/http_service.md)
    * [搭建gRPC服务](cn/http_derivatives.md#h2grpc)
    * [搭建thrift服务](cn/thrift.md#server端处理thrift请求)
    * [搭建Nshead服务](cn/nshead_service.md)
    * [高效率排查server卡顿](cn/server_debugging.md)
    * [推送](cn/server_push.md)
    * [雪崩](cn/avalanche.md)
    * [自适应限流](cn/auto_concurrency_limiter.md)
    * [流媒体服务](https://github.com/brpc/media-server)
    * [json2pb](cn/json2pb.md)
  * [内置服务](cn/builtin_service.md)
    * [status](cn/status.md)
    * [vars](cn/vars.md)
    * [connections](cn/connections.md)
    * [flags](cn/flags.md)
    * [rpcz](cn/rpcz.md)
    * [cpu_profiler](cn/cpu_profiler.md)
    * [heap_profiler](cn/heap_profiler.md)
    * [contention_profiler](cn/contention_profiler.md)
  * 工具
    * [rpc_press](cn/rpc_press.md)
    * [rpc_replay](cn/rpc_replay.md)
    * [rpc_view](cn/rpc_view.md)
    * [benchmark_http](cn/benchmark_http.md)
    * [parallel_http](cn/parallel_http.md)
  * 其他
    * [IOBuf](cn/iobuf.md)
    * [Streaming Log](cn/streaming_log.md)
    * [FlatMap](cn/flatmap.md)
    * [brpc外功修炼宝典](cn/brpc_intro.pptx)(培训材料)
    * [搭建大型服务入门](docs/en/tutorial_on_building_services.pptx)(培训材料)
    * [brpc内功修炼宝典](docs/en/brpc_internal.pptx)(培训材料)
  * 深入RPC
    * [New Protocol](cn/new_protocol.md)
    * [Atomic instructions](cn/atomic_instructions.md)
    * [IO](cn/io.md)
    * [Threading Overview](cn/threading_overview.md)
    * [Load Balancing](cn/load_balancing.md)
    * [Locality-aware](cn/lalb.md)
    * [Consistent Hashing](cn/consistent_hashing.md)
    * [Memory Management](cn/memory_management.md)
    * [Timer keeping](cn/timer_keeping.md)
    * [bthread_id](cn/bthread_id.md)
  * Use cases inside Baidu
    * [百度地图api入口](cn/case_apicontrol.md)
    * [联盟DSP](cn/case_baidu_dsp.md)
    * [ELF学习框架](cn/case_elf.md)
    * [云平台代理服务](cn/case_ubrpc.md)

# 贡献代码

请参考[这里](CONTRIBUTING.md#chinese-version)。

# 反馈和参与

* bug、疑惑、修改建议都欢迎提在[Github Issues](https://github.com/apache/incubator-brpc/issues)中
* 订阅邮件列表(dev-subscribe@brpc.apache.org)获得项目最新信息
