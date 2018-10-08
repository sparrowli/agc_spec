/*
 *  Copyright 2018 sparrowli. All rights reserved.
 *
 */

#include "base/logging.h"

#import <Foundation/Foundation.h>


namespace rtc {
std::string DescriptionFromOSStatus(OSStatus err) {
  NSError* error =
      [NSError errorWithDomain:NSOSStatusErrorDomain code:err userInfo:nil];
  return error.description.UTF8String;
}
}  // namespace rtc
