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
        setMinHeight(50);

        text = new TextRenderer(canvas, parentWidget);
        text->setTextColor(255, 255, 255, 255);
        text->setWeight(200);

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
                                    if (!t.empty()) {
                                        QString qs = QString::fromUtf8(t.c_str());
                                        qs.chop(1);
                                        setText(qs.toUtf8().toStdString());
                                    }
                                    break;

                                default:
                                    setText(
                                        getText() + s); 
                                    break;
                             } });
    }

    void EditTextRenderer::onRender()
    {
        text->setX(getX() + 5);
        text->setY(getY() + 5);

        text->init();

        pointerPaint.setColor(SkColorSetARGB(alpha, 120, 120, 120));

        if (text->getHeight() <= getMinHeight())
        {
            setHeight(getMinHeight());
        }
        else
        {
            setHeight(text->getHeight());
        }

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

        // Test Rendering
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
} // namespace Skia
