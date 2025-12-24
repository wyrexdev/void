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

            virtual void init() = 0;

            VM &getVM();
        private:
            VM &vm;
        };
    } // namespace Bindings
} // namespace JIT