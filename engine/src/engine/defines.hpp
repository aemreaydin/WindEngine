#ifndef WINDENGINE_DEFINES_HPP
#define WINDENGINE_DEFINES_HPP

#include <cstdint>

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

using F32 = float;
using F64 = double;

using B32 = int;
using B8 = char;

static_assert( sizeof( U8 ) == 1 );
static_assert( sizeof( U16 ) == 2 );
static_assert( sizeof( U32 ) == 4 );
static_assert( sizeof( U64 ) == 8 );

static_assert( sizeof( I8 ) == 1 );
static_assert( sizeof( I16 ) == 2 );
static_assert( sizeof( I32 ) == 4 );
static_assert( sizeof( I64 ) == 8 );

static_assert( sizeof( F32 ) == 4 );
static_assert( sizeof( F64 ) == 8 );

constexpr auto kTrue = 1;
constexpr auto kFalse = 0;

#if __APPLE__
constexpr auto kPlatformApple = 1;
#elif _WIN32
constexpr auto kPlatformWindows = 1;
#elif __linux
constexpr auto kPlatformLinux = 1;
#endif

#ifdef DLLEXPORT
#ifdef _MSC_VER
#define WINDAPI __declspec( dllexport );
#else
#define WINDAPI
#endif
#else
#ifdef _MSC_VER
#define WINDAPI __declspec( dllimport );
#else
#define WINDAPI
#endif
#endif
#endif  // WINDENGINE_DEFINES_HPP
