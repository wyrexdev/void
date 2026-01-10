#pragma once

#include "Headers/Global.hpp"

class SkiaRenderer;

namespace Skia
{
    class SkiaWidget
    {
    public:
        SkiaWidget();

        void init();
        void render();
        void refresh();

        virtual void onInit() = 0;
        virtual void onRender() = 0;

        SkiaRenderer *parentWidget;
        SkCanvas *canvas;

        bool isHover();

        // --- GET | WIDTH - HEIGHT ---
        float getWidth();
        float getHeight();
        // --- GET | WIDTH - HEIGHT ---

        // --- GET | POSITIONS ---
        float getX();
        float getY();
        // --- GET | POSITIONS ---


        // --- GET | MIN - MAX HEIGHT ---
        float getMinHeight();
        float getMaxHeight();
        // --- GET | MIN - MAX HEIGHT ---

        // --- GET | MIN - MAX WIDTH ---
        float getMinWidth();
        float getMaxWidth();
        // --- GET | MIN - MAX WIDTH ---

     
        // --- SET | WIDTH - HEIGHT ---
        void setWidth(float w);
        void setHeight(float h);
        // --- SET | WIDTH - HEIGHT ---

        // --- SET | POSITIONS ---
        void setX(float x);
        void setY(float y);
        // --- SET | POSITIONS ---


        // --- SET | MIN - MAX HEIGHT ---
        void setMinHeight(float w);
        void setMaxHeight(float h);
        // --- SET | MIN - MAX HEIGHT ---

        // --- SET | MIN - MAX HEIGHT ---
        void setMinWidth(float x);
        void setMaxWidth(float y);
        // --- SET | MIN - MAX HEIGHT ---


        bool isEnabled();
        void enable();
        void disable();
        
    private:
        Math::Vec3 position;
        Math::Vec3 rotation;
        Math::Vec3 scale;

        float minHeight = 0;
        float maxHeight = 0;

        float minWidth = 0;
        float maxWidth = 0;

        bool isInitialized = false;

        bool enabled = true;
    };
} // namespace Skia