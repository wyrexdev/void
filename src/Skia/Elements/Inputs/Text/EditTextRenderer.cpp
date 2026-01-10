#include "Skia/Elements/Inputs/Text/EditTextRenderer.hpp"

namespace Skia
{
    EditTextRenderer::EditTextRenderer(SkCanvas *c, SkiaRenderer *p)
    {
        canvas = c;
        parentWidget = p;
    }

    void EditTextRenderer::onInit()
    {
        setMinHeight(30);
        setMinWidth(200);

        text = new TextRenderer(canvas, parentWidget);
        text->setTextColor(255, 255, 255, 255);
        text->setWeight(200);

        hint = new TextRenderer(canvas, parentWidget);
        hint->setTextColor(255, 200, 200, 200);
        hint->setWeight(200);

        pointerPaint.setAntiAlias(true);
        pointerPaint.setColor(SkColorSetRGB(120, 120, 120));

        borderPaint.setAntiAlias(true);
        borderPaint.setStyle(SkPaint::kStroke_Style);
        borderPaint.setStrokeWidth(2.0f);
        borderPaint.setColor(SkColorSetRGB(200, 200, 200));

        blinkThread = std::thread([this]
                                  {
        while (running.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            bool next = !isIndic.load();
            isIndic.store(next);
            alpha.store(next ? 255 : 0);

            QMetaObject::invokeMethod(
                Signals::Skia::instance(),
                "updateSkia",
                Qt::QueuedConnection
            );
        } });

        QObject::connect(Signals::System::Keyboard::instance(), &Signals::System::Keyboard::keyDown, [=](int key, std::string s, Qt::KeyboardModifiers mods)
                         {
                            // std::cout << "Char Code: " << key << std::endl;

                            std::string t = getText();

                             switch(key) {
                                case Qt::Key_Backspace:
                                    if (!getText().empty()) {
                                        QString qs = QString::fromUtf8(getText().c_str());
                                        qs.chop(1);
                                        setText(qs.toUtf8().toStdString());
                                    }

                                    if(getText().empty()) {
                                        hint->enable();
                                    }
                                    break;

                                default:
                                    setText(
                                        getText() + s); 
                                    
                                    if(!getText().empty()) {
                                        hint->disable();
                                    }
                                    break;
                             } });
    }

    void EditTextRenderer::onRender()
    {
        text->setX(getX() + 5);
        text->setY(getY() + 5);

        hint->setX(getX() + 5);
        hint->setY(getY() + 5);

        text->init();
        hint->init();

        pointerPaint.setColor(SkColorSetARGB(alpha, 120, 120, 120));

        float h = text->getHeight();
        float w = text->getWidth();

        if (h < getMinHeight())
        {
            setHeight(getMinHeight());
        }
        else if (h > getMaxHeight())
        {
            setHeight(getMaxHeight());
        }
        else
        {
            setHeight(h);
        }

        if (w < getMinWidth())
        {
            setWidth(getMinWidth());
        }
        else if (w > getMaxWidth())
        {
            setWidth(getMaxWidth());
        }
        else
        {
            setWidth(w);
        }

        std::cout << getWidth() << std::endl;

        // Border
        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX(), getY(), 200, getHeight()),
            5, 5,
            borderPaint);

        // Indicator
        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX() + text->getWidth() + 5, getY() + 5, 2, getHeight() - 10),
            5, 5,
            pointerPaint);

        // Hint Rendering
        hint->render();
        // Text Rendering
        text->render();
    }

    void EditTextRenderer::setText(std::string t)
    {
        text->setText(t);
    }

    std::string EditTextRenderer::getText()
    {
        return text->getText();
    }

    void EditTextRenderer::setHint(std::string h)
    {
        hint->setText(h);
    }

    std::string EditTextRenderer::getHint()
    {
        return hint->getText();
    }
} // namespace Skia
