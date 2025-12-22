#pragma once

#include <atomic>

namespace InputEvent
{
    namespace Mouse
    {
        static std::atomic<int> mouseX, mouseY;
    } // namespace Mouse

} // namespace InputEvent
