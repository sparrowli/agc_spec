/*
 *  Copyright 2018 sparrowli. All Rights Reserved.
 *
 */

#ifndef BASE_CONSTRUCTORMAGIC_H_
#define BASE_CONSTRUCTORMAGIC_H_

// Put this in the declarations for a class to be unassignable.
#define RTC_DISALLOW_ASSIGN(TypeName) void operator=(const TypeName&) = delete

// A macro to disallow the copy constructor and operator= functions. This should
// be used in the declarations for a class.
#define RTC_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;          \
  RTC_DISALLOW_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the default
// constructor, copy constructor and operator= functions.
//
// This should be used in the declarations for a class that wants to prevent
// anyone from instantiating it. This is especially useful for classes
// containing only static methods.
#define RTC_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                               \
  RTC_DISALLOW_COPY_AND_ASSIGN(TypeName)

#endif  // BASE_CONSTRUCTORMAGIC_H_
