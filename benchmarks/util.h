/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BIONIC_BENCHMARKS_UTIL_H_
#define _BIONIC_BENCHMARKS_UTIL_H_

#include <map>
#include <mutex>
#include <string>
#include <vector>

typedef void (*benchmark_func_t) (void);

extern std::mutex g_map_lock;

extern std::map<std::string, benchmark_func_t> g_str_to_func;

static int  __attribute__((unused)) EmplaceBenchmark (std::string fn_name, benchmark_func_t fn_ptr) {
  g_map_lock.lock();
  g_str_to_func.emplace(std::string(fn_name), fn_ptr);
  g_map_lock.unlock();
  return 0;
}

#define BIONIC_BENCHMARK(n) \
  int _bionic_benchmark_##n __attribute__((unused)) = EmplaceBenchmark(std::string(#n), reinterpret_cast<benchmark_func_t>(n))

constexpr auto KB = 1024;

typedef struct {
  long cpu_to_lock;
  long num_iterations;
  std::string xmlpath;
  std::vector<std::string> extra_benchmarks;
} bench_opts_t;

// This function returns a pointer less than 2 * alignment + or_mask bytes into the array.
char* GetAlignedMemory(char* orig_ptr, size_t alignment, size_t or_mask);

char* GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes);

char* GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fill_byte);

bool LockToCPU(long cpu_to_lock);

#endif // _BIONIC_BENCHMARKS_UTIL_H
