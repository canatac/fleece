//
//  FleeceException.hh
//  Fleece
//
//  Created by Jens Alfke on 7/19/16.
//  Copyright (c) 2016 Couchbase. All rights reserved.
//

#pragma once
#include <stdexcept>
#include "PlatformCompat.hh"

namespace fleece {

    // Error codes -- keep these in sync with the public FLError enum in Fleece.h!
    typedef enum {
        NoError = 0,
        MemoryError,        // Out of memory, or allocation failed
        OutOfRange,         // Array index or iterator out of range
        InvalidData,        // Bad input data (NaN, non-string key, etc.)
        EncodeError,        // Structural error encoding (missing value, too many ends, etc.)
        JSONError,          // Error parsing JSON
        UnknownValue,       // Unparseable data in a Value (corrupt? Or from some distant future?)
        PathSyntaxError,    // Invalid Path specifier
        InternalError,      // This shouldn't happen
        NotFound,           // Key not found
        SharedKeysStateError, // Incorrect use of persistent shared keys (not in transaction, etc.)
    } ErrorCode;


    class FleeceException : public std::runtime_error {
    public:

        FleeceException(ErrorCode code_, const char *what)
        :std::runtime_error(what)
        ,code(code_)
        { }

        [[noreturn]] static void _throw(ErrorCode code, const char *what);

        static ErrorCode getCode(const std::exception&) noexcept;

        const ErrorCode code;
    };


    static inline void throwIf(bool bad, ErrorCode error, const char *message) {
        if (_usuallyFalse(bad))
            FleeceException::_throw(error, message);
    }

}
