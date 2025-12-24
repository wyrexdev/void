#pragma once

#include "Engine/JS/VM.hpp"

namespace JIT
{
    namespace Contexts
    {
        class Context
        {
        public:
            Context(VM &vm);
            
        private:
            VM &vm;
        };
    } // namespace Bindings
} // namespace JIT