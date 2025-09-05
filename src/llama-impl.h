#pragma once

#include "ggml.h" // for ggml_log_level

#include <string>
#include <vector>

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifdef __GNUC__
#    if defined(__MINGW32__) && !defined(__clang__)
#        define LLAMA_ATTRIBUTE_FORMAT(...) __attribute__((format(gnu_printf, __VA_ARGS__)))
#    else
#        define LLAMA_ATTRIBUTE_FORMAT(...) __attribute__((format(printf, __VA_ARGS__)))
#    endif
#else
#    define LLAMA_ATTRIBUTE_FORMAT(...)
#endif

//
// logging
//

LLAMA_ATTRIBUTE_FORMAT(2, 3)
void llama_log_internal        (ggml_log_level level, const char * format, ...);
void llama_log_callback_default(ggml_log_level level, const char * text, void * user_data);

#ifdef __ANDROID__
#define LLAMA_LOG(...)         __android_log_print(ANDROID_LOG_INFO, "bare", __VA_ARGS__)  // llama_log_internal(GGML_LOG_LEVEL_NONE , __VA_ARGS__)
#define LLAMA_LOG_INFO(...)    __android_log_print(ANDROID_LOG_INFO, "bare", __VA_ARGS__)  // llama_log_internal(GGML_LOG_LEVEL_INFO , __VA_ARGS__)
#define LLAMA_LOG_WARN(...)    __android_log_print(ANDROID_LOG_WARN, "bare", __VA_ARGS__)  // llama_log_internal(GGML_LOG_LEVEL_WARN , __VA_ARGS__)
#define LLAMA_LOG_ERROR(...)   __android_log_print(ANDROID_LOG_ERROR, "bare", __VA_ARGS__)  // llama_log_internal(GGML_LOG_LEVEL_ERROR, __VA_ARGS__)
#define LLAMA_LOG_DEBUG(...)   __android_log_print(ANDROID_LOG_DEBUG, "bare", __VA_ARGS__)  // llama_log_internal(GGML_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LLAMA_LOG_CONT(...)    __android_log_print(ANDROID_LOG_INFO, "bare", __VA_ARGS__)  // llama_log_internal(GGML_LOG_LEVEL_CONT , __VA_ARGS__)
#else
#define LLAMA_LOG(...)       llama_log_internal(GGML_LOG_LEVEL_NONE , __VA_ARGS__)
#define LLAMA_LOG_INFO(...)  llama_log_internal(GGML_LOG_LEVEL_INFO , __VA_ARGS__)
#define LLAMA_LOG_WARN(...)  llama_log_internal(GGML_LOG_LEVEL_WARN , __VA_ARGS__)
#define LLAMA_LOG_ERROR(...) llama_log_internal(GGML_LOG_LEVEL_ERROR, __VA_ARGS__)
#define LLAMA_LOG_DEBUG(...) llama_log_internal(GGML_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LLAMA_LOG_CONT(...)  llama_log_internal(GGML_LOG_LEVEL_CONT , __VA_ARGS__)
#endif

// Debug-only logging macro that's only enabled in debug builds at compile time
#ifndef NDEBUG
#define LLAMA_LOG_CMAKE_DEBUG(...) LLAMA_LOG_DEBUG(__VA_ARGS__)
#else
#define LLAMA_LOG_CMAKE_DEBUG(...)
#endif

//
// helpers
//

template <typename T>
struct no_init {
    T value;
    no_init() { /* do nothing */ }
};

struct time_meas {
    time_meas(int64_t & t_acc, bool disable = false);
    ~time_meas();

    const int64_t t_start_us;

    int64_t & t_acc;
};

void replace_all(std::string & s, const std::string & search, const std::string & replace);

// TODO: rename to llama_format ?
LLAMA_ATTRIBUTE_FORMAT(1, 2)
std::string format(const char * fmt, ...);

std::string llama_format_tensor_shape(const std::vector<int64_t> & ne);
std::string llama_format_tensor_shape(const struct ggml_tensor * t);

std::string gguf_kv_to_str(const struct gguf_context * ctx_gguf, int i);