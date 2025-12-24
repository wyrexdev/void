#pragma once

#include "Engine/JS/Context/Base/Context.hpp"

namespace JIT
{
    namespace Contexts
    {
        class BrowserContext : public Context
        {
        public:
            BrowserContext(VM &vm) : Context(vm) {}

            void init() override;
        private:
        };
    } // namespace Bindings
} // namespace JIT