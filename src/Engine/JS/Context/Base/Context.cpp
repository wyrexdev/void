#include "Engine/JS/Context/Base/Context.hpp"

namespace JIT
{
    namespace Contexts
    {
        Context::Context(VM& vm) : vm(vm) {
            
        }

        VM& Context::getVM() {
            return vm;
        }
    } // namespace Bindings
} // namespace JIT