#include <string>

namespace JIT
{
    class VM {
        public:
        void init();
        void eval(std::string js);
        void registerGlobals();
        void tick();
    };
} // namespace JIT
