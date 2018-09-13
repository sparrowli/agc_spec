/*
 *  Copyright (c) 2018 sparrowli. All Rights Reserved.
 *
 */

#ifndef BASE_SYSTEM_INLINE_H_
#define BASE_SYSTEM_INLINE_H_

#if defined(_MSC_VER)

#define RTC_FORCE_INLINE __forceinline
#define RTC_NO_INLINE __declspec(noinline)

#elif defined(__GNUC__)

#define RTC_FORCE_INLINE __attribute__((__always_inline__))
#define RTC_NO_INLINE __attribute__((__noinline__))

#else

#define RTC_FORCE_INLINE
#define RTC_NO_INLINE

#endif

#endif  // BASE_SYSTEM_INLINE_H_
