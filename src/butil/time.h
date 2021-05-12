/**
 * @file time.h
 * @brief 时间相关函数头文件
 * @author wangpengcheng  (wangpengcheng2018@gmail.com)
 * @version 1.0
 * @date 2021-05-12 21:54:07
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
 *    <td> 2021-05-12 21:54:07 </td>
 *    <td> 1.0 </td>
 *    <td> wangpengcheng </td>
 *    <td> 添加注释 </td>
 * </tr>
 * </table>
 */


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

// Date: Wed Aug 11 10:38:17 2010

// Measuring time

#ifndef BUTIL_BAIDU_TIME_H
#define BUTIL_BAIDU_TIME_H

#include <time.h>     // timespec, clock_gettime
#include <sys/time.h> // timeval, gettimeofday
#include <stdint.h>   // int64_t, uint64_t

#if defined(NO_CLOCK_GETTIME_IN_MAC)
#include <mach/mach.h>
#define CLOCK_REALTIME CALENDAR_CLOCK
#define CLOCK_MONOTONIC SYSTEM_CLOCK

typedef int clockid_t;

// clock_gettime is not available in MacOS < 10.12
int clock_gettime(clockid_t id, timespec *time);

#endif

namespace butil
{

    /**
     * 
     * @todo 将所有函数进行抽象类操作
     */
    /**
     * @brief 获取svn编号
     * @return const char* 
     */
    const char *last_changed_revision();

    // ----------------------
    // timespec manipulations
    // ----------------------

    /**
     * @brief  tm->tv_nsec 转换到 [0, 1,000,000,000) 
     * @param  tm                pox时间差主要数据结构单元，输入时间差
     */
    inline void timespec_normalize(timespec *tm)
    {
        if (tm->tv_nsec >= 1000000000L)
        {
            const int64_t added_sec = tm->tv_nsec / 1000000000L;
            tm->tv_sec += added_sec;
            tm->tv_nsec -= added_sec * 1000000000L;
        }
        else if (tm->tv_nsec < 0)
        {
            const int64_t sub_sec = (tm->tv_nsec - 999999999L) / 1000000000L;
            tm->tv_sec += sub_sec;
            tm->tv_nsec -= sub_sec * 1000000000L;
        }
    }

    /**
     * @brief  
     * 时间对象加法操作
     * Add timespec |span| into timespec |*tm|.
     * 并进行标准化
     * @param  tm               目标操作指针
     * @param  span             操作引用
     */
    inline void timespec_add(timespec *tm, const timespec &span)
    {
        tm->tv_sec += span.tv_sec;
        tm->tv_nsec += span.tv_nsec;
        timespec_normalize(tm);
    }

    /**
     * @brief  时间戳对象减法操作 \n 
     * Minus timespec |span| from timespec |*tm|.
     * tm->tv_nsec will be inside [0, 1,000,000,000)
     * @param  tm               目标指针
     * @param  span             参与运算操作引用
     */
    inline void timespec_minus(timespec *tm, const timespec &span)
    {
        tm->tv_sec -= span.tv_sec;
        tm->tv_nsec -= span.tv_nsec;
        timespec_normalize(tm);
    }

    // ------------------------------------------------------------------
    // Get the timespec after specified duration from |start_time|
    // ------------------------------------------------------------------
    /**
     * @brief  对应相关计算操作
     * @param  start_time       开始时间
     * @param  nanoseconds      操作的纳秒数量
     * @return timespec         操作结果值
     * @note 这里使用的是值传入，可以防止对原有数据的修改
     */
    inline timespec nanoseconds_from(timespec start_time, int64_t nanoseconds)
    {
        start_time.tv_nsec += nanoseconds;
        timespec_normalize(&start_time);
        return start_time;
    }
    /**
     * @brief  对应相关计算操作
     * @param  start_time       开始时间
     * @param  microseconds     操作的毫秒数量
     * @return timespec         操作结果值
     * @note 这里使用的是值传入，可以防止对原有数据的修改
     */
    inline timespec microseconds_from(timespec start_time, int64_t microseconds)
    {
        return nanoseconds_from(start_time, microseconds * 1000L);
    }
    /**
     * @brief  对应相关计算操作
     * @param  start_time       开始时间
     * @param  microseconds     操作的微秒数量
     * @return timespec         操作结果值
     * @note 这里使用的是值传入，可以防止对原有数据的修改
     */
    inline timespec milliseconds_from(timespec start_time, int64_t milliseconds)
    {
        return nanoseconds_from(start_time, milliseconds * 1000000L);
    }

    /**
     * @brief  对应相关计算操作
     * @param  start_time       开始时间
     * @param  microseconds     操作的秒数量
     * @return timespec         操作结果值
     * @note 这里使用的是值传入，可以防止对原有数据的修改
     */
    inline timespec seconds_from(timespec start_time, int64_t seconds)
    {
        return nanoseconds_from(start_time, seconds * 1000000000L);
    }

    // --------------------------------------------------------------------
    // Get the timespec after specified duration from now (CLOCK_REALTIME)
    // --------------------------------------------------------------------
    /**
     * @brief  通过clock_gettime获取当前时间的基础操作
     * @param  nanoseconds      微妙数目
     * @return timespec         最终该结果
     */
    inline timespec nanoseconds_from_now(int64_t nanoseconds)
    {
        timespec time;
        clock_gettime(CLOCK_REALTIME, &time);
        return nanoseconds_from(time, nanoseconds);
    }

    inline timespec microseconds_from_now(int64_t microseconds)
    {
        return nanoseconds_from_now(microseconds * 1000L);
    }

    inline timespec milliseconds_from_now(int64_t milliseconds)
    {
        return nanoseconds_from_now(milliseconds * 1000000L);
    }

    inline timespec seconds_from_now(int64_t seconds)
    {
        return nanoseconds_from_now(seconds * 1000000000L);
    }
    /**
     * @brief  对当前时间函数进行操作
     * @param  span             时间操作
     * @return timespec         最终目标时间
     */
    inline timespec timespec_from_now(const timespec &span)
    {
        timespec time;
        clock_gettime(CLOCK_REALTIME, &time);
        timespec_add(&time, span);
        return time;
    }

    // ---------------------------------------------------------------------
    // Convert timespec to and from a single integer.
    // For conversions between timespec and timeval, use TIMEVAL_TO_TIMESPEC
    // and TIMESPEC_TO_TIMEVAL defined in <sys/time.h>
    // ---------------------------------------------------------------------1
    /**
     * @brief  将标准时间参数转换为一个单个的整数
     * @param  ts               My Param doc
     * @return int64_t 
     */
    inline int64_t timespec_to_nanoseconds(const timespec &ts)
    {
        return ts.tv_sec * 1000000000L + ts.tv_nsec;
    }

    inline int64_t timespec_to_microseconds(const timespec &ts)
    {
        return timespec_to_nanoseconds(ts) / 1000L;
    }

    inline int64_t timespec_to_milliseconds(const timespec &ts)
    {
        return timespec_to_nanoseconds(ts) / 1000000L;
    }

    inline int64_t timespec_to_seconds(const timespec &ts)
    {
        return timespec_to_nanoseconds(ts) / 1000000000L;
    }
    /**
     * @brief  将整数的纳秒数转换为时间对象
     * @param  ns               整数纳秒数
     * @return timespec 
     */
    inline timespec nanoseconds_to_timespec(int64_t ns)
    {
        timespec ts;
        ts.tv_sec = ns / 1000000000L;
        ts.tv_nsec = ns - ts.tv_sec * 1000000000L;
        return ts;
    }

    inline timespec microseconds_to_timespec(int64_t us)
    {
        return nanoseconds_to_timespec(us * 1000L);
    }

    inline timespec milliseconds_to_timespec(int64_t ms)
    {
        return nanoseconds_to_timespec(ms * 1000000L);
    }

    inline timespec seconds_to_timespec(int64_t s)
    {
        return nanoseconds_to_timespec(s * 1000000000L);
    }

    // ---------------------------------------------------------------------
    // Convert timeval to and from a single integer.
    // For conversions between timespec and timeval, use TIMEVAL_TO_TIMESPEC
    // and TIMESPEC_TO_TIMEVAL defined in <sys/time.h>
    // ---------------------------------------------------------------------
    /**
     * @brief  将timeval 对象转换为微妙对象
     * @param  tv               timeval 时间基础对象
     * @return int64_t          最终返回的微秒数
     * @code{.cpp}
     * struct timeval
     * {
     * __time_t tv_sec;		// Seconds. 
     * __suseconds_t tv_usec;	// Microseconds.
     * };
     * @endcode
     * 
     */
    inline int64_t timeval_to_microseconds(const timeval &tv)
    {
        return tv.tv_sec * 1000000L + tv.tv_usec;
    }

    inline int64_t timeval_to_milliseconds(const timeval &tv)
    {
        return timeval_to_microseconds(tv) / 1000L;
    }

    inline int64_t timeval_to_seconds(const timeval &tv)
    {
        return timeval_to_microseconds(tv) / 1000000L;
    }

    inline timeval microseconds_to_timeval(int64_t us)
    {
        timeval tv;
        tv.tv_sec = us / 1000000L;
        tv.tv_usec = us - tv.tv_sec * 1000000L;
        return tv;
    }

    inline timeval milliseconds_to_timeval(int64_t ms)
    {
        return microseconds_to_timeval(ms * 1000L);
    }

    inline timeval seconds_to_timeval(int64_t s)
    {
        return microseconds_to_timeval(s * 1000000L);
    }

    // ---------------------------------------------------------------
    // Get system-wide monotonic time.
    // ---------------------------------------------------------------
    /**
     * @brief 获取系统范围内的单调时间
     * @return int64_t 
     */
    extern int64_t monotonic_time_ns();

    inline int64_t monotonic_time_us()
    {
        return monotonic_time_ns() / 1000L;
    }

    inline int64_t monotonic_time_ms()
    {
        return monotonic_time_ns() / 1000000L;
    }

    inline int64_t monotonic_time_s()
    {
        return monotonic_time_ns() / 1000000000L;
    }

    namespace detail
    {
        /**
         * @brief  时间循环
         * @return uint64_t
         * @todo 添加ARM的纳秒级时间计数器，主要用来计算CPU的主要频率
         */
        inline uint64_t clock_cycles()
        {
            // 低位数据读取
            unsigned int lo = 0;
            // 高位数据读取
            unsigned int hi = 0;

            // We cannot use "=A", since this would use %rax on x86_64
            ///< 这里内联汇编函数
            ///< [汇编与C/C++内联嵌入汇编](https://blog.csdn.net/Levet/article/details/78516307)
            ///< [RDTSC指令实现纳秒级计时器](http://blog.chinaunix.net/uid-8048969-id-4877850.html)

            __asm__ __volatile__(
                "rdtsc"
                : "=a"(lo), "=d"(hi));
            // 对高位和低位进行合并
            return ((uint64_t)hi << 32) | lo;
        }
        /**
         * @brief  读取cpu时间频率
         * @return int64_t 
         */
        extern int64_t read_invariant_cpu_frequency();
        // Be positive iff:
        // 1 Intel x86_64 CPU (multiple cores) supporting constant_tsc and
        // nonstop_tsc(check flags in /proc/cpuinfo)

        extern int64_t invariant_cpu_freq; ///< CPU频率基本函数
    }                                      // namespace detail

    // ---------------------------------------------------------------
    // Get cpu-wide (wall-) time.
    // Cost ~9ns on Intel(R) Xeon(R) CPU E5620 @ 2.40GHz
    // ---------------------------------------------------------------
    // note: Inlining shortens time cost per-call for 15ns in a loop of many
    //       calls to this function.
    /**
     * @brief  获取CPU的大概时间
     * @details Cost ~9ns on Intel(R) Xeon(R) CPU E5620 @ 2.40GHz
     * @note 内联在对这个函数的多次调用的循环中，每次调用的时间开销缩短了15纳秒。
     * @return int64_t 
     */
    inline int64_t cpuwide_time_ns()
    {
/**
 * @brief 非百度内部使用接口
 */
#if !defined(BAIDU_INTERNAL)
        // nearly impossible to get the correct invariant cpu frequency on
        // different CPU and machines. CPU-ID rarely works and frequencies
        // in "model name" and "cpu Mhz" are both unreliable.
        // Since clock_gettime() in newer glibc/kernel is much faster(~30ns)
        // which is closer to the previous impl. of cpuwide_time(~10ns), we
        // simply use the monotonic time to get rid of all related issues.
        /**
         * @brief 非百度内部使用接口
         * @details 
         * 几乎不可能在不同的cpu和机器上获得正确的不变cpu频率。
         * CPU-ID很少工作，“型号名称”和“CPU Mhz”中的频率都不可靠。
         * 因为较新的glibc/内核中的clock_gettime（）要快得多（~30ns），这与以前的impl比较接近。
         * 在cpuwide\u时间（~10ns）中，我们简单地利用单调的时间来摆脱所有相关的问题。
         */
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000000000L + now.tv_nsec;
#else
        int64_t cpu_freq = detail::invariant_cpu_freq;
        if (cpu_freq > 0)
        {
            const uint64_t tsc = detail::clock_cycles();
            //Try to avoid overflow
            const uint64_t sec = tsc / cpu_freq;
            const uint64_t remain = tsc % cpu_freq;
            // TODO: should be OK until CPU's frequency exceeds 16GHz.
            return remain * 1000000000L / cpu_freq + sec * 1000000000L;
        }
        else if (!cpu_freq)
        {
            // Lack of necessary features, return system-wide monotonic time instead.
            return monotonic_time_ns();
        }
        else
        {
            // Use a thread-unsafe method(OK to us) to initialize the freq
            // to save a "if" test comparing to using a local static variable
            /**
             * @brief  线程参数初始化安全
             */
            detail::invariant_cpu_freq = detail::read_invariant_cpu_frequency();
            return cpuwide_time_ns();
        }
#endif // defined(BAIDU_INTERNAL)
    }

    inline int64_t cpuwide_time_us()
    {
        return cpuwide_time_ns() / 1000L;
    }

    inline int64_t cpuwide_time_ms()
    {
        return cpuwide_time_ns() / 1000000L;
    }

    inline int64_t cpuwide_time_s()
    {
        return cpuwide_time_ns() / 1000000000L;
    }

    // --------------------------------------------------------------------
    // Get elapse since the Epoch.
    // No gettimeofday_ns() because resolution of timeval is microseconds.
    // Cost ~40ns on 2.6.32_1-12-0-0, Intel(R) Xeon(R) CPU E5620 @ 2.40GHz
    // --------------------------------------------------------------------
    /**
     * @brief 
     * 获取Epoch使用时间主要函数
     * 没有 gettimeofday_ns() 纳秒函数，因为基于timeval的时间统计函数是microseconds的
     * @attention 使用时间 ~40ns on 2.6.32_1-12-0-0, Intel(R) Xeon(R) CPU E5620 @ 2.40GHz
     * @return int64_t 
     */
    inline int64_t gettimeofday_us()
    {
        timeval now;
        gettimeofday(&now, NULL);
        return now.tv_sec * 1000000L + now.tv_usec;
    }

    inline int64_t gettimeofday_ms()
    {
        return gettimeofday_us() / 1000L;
    }

    inline int64_t gettimeofday_s()
    {
        return gettimeofday_us() / 1000000L;
    }

    // ----------------------------------------
    // Control frequency of operations.
    // ----------------------------------------
    // Example:
    //   EveryManyUS every_1s(1000000L);
    //   while (1) {
    //       ...
    //       if (every_1s) {
    //           // be here at most once per second
    //       }
    //   }
    /**
     * @brief 频率操作控制器类，主要用来进行时间间隔的频率控制，相当于自旋转锁
     * @details
     * @code{.cpp}
     * // Example:
     * EveryManyUS every_1s(1000000L);
     * while (1) {
     *      ...
     *      if (every_1s) {
     *      // be here at most once per second
     *      }
     * }
     * @endcode
     */
    class EveryManyUS
    {
    public:
        /**
         * @brief Construct a new Every Many U S object
         * @param  interval_us      间隔时间微妙
         */
        explicit EveryManyUS(int64_t interval_us)
            : _last_time_us(cpuwide_time_us()), _interval_us(interval_us) {}

        operator bool()
        {
            // 当前微妙计数器
            const int64_t now_us = cpuwide_time_us();
            if (now_us < _last_time_us + _interval_us)
            {
                return false;
            }
            _last_time_us = now_us;
            return true;
        }

    private:
        int64_t _last_time_us;      ///< 上一个时间点
        const int64_t _interval_us; ///< 间隔时间周期
    };

    // ---------------
    //  Count elapses
    // ---------------
    /**
     * @brief 基础时间类 
     * 
     */
    class Timer
    {
    public:
        /*! 计时器状态类型 */
        enum TimerType
        {
            STARTED, /*! 开始 */
        };
        /**
         * @brief Construct a new Timer object
         * 自动初始化开始与结束时间
         */
        Timer() : _stop(0),
                  _start(0)
        {
        }
        /**
         * @brief Construct a new Timer object
         * 
         */
        explicit Timer(const TimerType)
        {
            start();
        }

        /**
         * @brief Start this timer
         * 时间开始函数，主要对
         * _start 与_stop 进行赋值
         */
        void start()
        {
            _start = cpuwide_time_ns();
            _stop = _start;
        }

        /**
         * @brief Stop this timer
         * @details 主要通过对时间戳的修改比较进行最终范围结果的确定
         */
        void stop()
        {
            _stop = cpuwide_time_ns();
        }

        // Get the elapse from start() to stop(), in various units.
        /**
         * @brief  获取开始与结束之间的时间差值。
         * @return int64_t 时间数量
         */
        int64_t n_elapsed() const { return _stop - _start; }
        int64_t u_elapsed() const { return n_elapsed() / 1000L; }
        int64_t m_elapsed() const { return u_elapsed() / 1000L; }
        int64_t s_elapsed() const { return m_elapsed() / 1000L; }

        double n_elapsed(double) const { return (double)(_stop - _start); }
        double u_elapsed(double) const { return (double)n_elapsed() / 1000.0; }
        double m_elapsed(double) const { return (double)u_elapsed() / 1000.0; }
        double s_elapsed(double) const { return (double)m_elapsed() / 1000.0; }

    private:
        int64_t _stop;  ///< 停止时间
        int64_t _start; ///< 开始时间
    };

} // namespace butil

#endif // BUTIL_BAIDU_TIME_H
