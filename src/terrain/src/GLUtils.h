#pragma once

#include <assert.h>
#include <iostream>

#include "glad/gl.h"

#define LOG_INFO(MESSAGE) std::cout << MESSAGE << "\n";

#define GL_CALL(FUNCTION)                                                      \
  {                                                                            \
    {                                                                          \
      FUNCTION;                                                                \
    }                                                                          \
    assert(glGetError() == GL_NO_ERROR && __FILE__ && __LINE__);               \
  }

#define GL_CALL_WITH_CALLBACK(FUNCTION, CALLBACK)                              \
  {                                                                            \
    {                                                                          \
      FUNCTION;                                                                \
    }                                                                          \
    if (glGetError() != GL_NO_ERROR) {                                         \
      assert(false && __FILE__ && __LINE__);                                   \
      {                                                                        \
        CALLBACK;                                                              \
      }                                                                        \
    }                                                                          \
  }

#define GL_CALL_WITH_RETURN(FUNCTION, CALLBACK, RETURN_VALUE)                  \
  {                                                                            \
    {                                                                          \
      FUNCTION;                                                                \
    }                                                                          \
    if (glGetError() != GL_NO_ERROR) {                                         \
      assert(false && __FILE__ && __LINE__);                                   \
      {                                                                        \
        CALLBACK;                                                              \
      }                                                                        \
      return RETURN_VALUE;                                                     \
    }                                                                          \
  }

#define GL_CALL_WITH_FALSE_RETURN(FUNCTION)                                    \
  GL_CALL_WITH_RETURN(FUNCTION, {}, false)
