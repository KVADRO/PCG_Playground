#pragma once

#include <vector>
#include <functional>

namespace vv
{
    struct CallOnDestroy
    {
        std::vector<std::function<void()>> callbacks;

        ~CallOnDestroy() { (*this)(); }

        void operator()()
        {
            for (const auto& callback : callbacks) {
                callback();
            }

            callbacks.clear();
        }
    };
}