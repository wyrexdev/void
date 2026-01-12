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

        QObject::connect(Signals::System::Mouse::instance(), &Signals::System::Mouse::onClick, [=](float x, float y)
                         { text->unselect(); });

        QObject::connect(Signals::System::Keyboard::instance(), &Signals::System::Keyboard::textInput, [=](const std::string &utf8)
                         {
                            std::string t = getText();

                            if (currentIndex < 0)
                                currentIndex = 0;

                            if (currentIndex > t.size())
                                currentIndex = t.size();

                            t.insert(currentIndex, utf8);

                            setText(t);
                            currentIndex += utf8.size();

                            if(!getText().empty()) {
                                hint->disable();
                            } });

        QObject::connect(Signals::System::Keyboard::instance(), &Signals::System::Keyboard::keyDown, [=](int key, std::string s, Qt::KeyboardModifiers mods)
                         {
                            // std::cout << "Char Code: " << key << std::endl;

                            std::string t = getText();
                            
                            text->unselect();
                            
                            switch(key) {
                                case Qt::Key_Backspace:
                                    if (isSelected)
                                    {
                                        std::string t = getText();
                                    
                                        if (beginIndex < endIndex && endIndex <= t.size())
                                        {
                                            t.erase(beginIndex, endIndex - beginIndex);
                                            setText(t);
                                        
                                            currentIndex = beginIndex;
                                        }
                                    
                                        isSelected = false;
                                    }  
                                    
                                    if (!getText().empty() || currentIndex <= 0) {
                                        QString qs = QString::fromUtf8(getText().c_str());
                                        int qIndex = QString::fromUtf8(
                                            getText().substr(0, currentIndex).c_str()
                                        ).length();
                                    
                                        qs.remove(qIndex - 1, 1);
                                    
                                        setText(qs.toUtf8().toStdString());
                                    
                                        currentIndex = QString::fromUtf8(
                                            qs.left(qIndex - 1).toUtf8()
                                        ).toUtf8().size();
                                    }

                                    if(getText().empty()) {
                                        hint->enable();
                                    }
                                    break;

                                case Qt::Key_A:
                                    if (mods & Qt::ControlModifier) {
                                        text->selectAll();

                                        std::string t = text->getText();

                                        beginIndex = 0;
                                        endIndex = t.size();

                                        isSelected = true;
                                    } 
                                    break;

                                case Qt::Key_Shift: 
                                    if (mods & Qt::ShiftModifier) {
                                        
                                    }
                                break;

                                case Qt::Key_Left:
                                    backChar();
                                    break;

                                case Qt::Key_Right:
                                    nextChar();
                                    break;

                                default:
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

        // Border
        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX(), getY(), 200, getHeight()),
            5, 5,
            borderPaint);

        float x = caretX();

        // Caret / Indicator
        canvas->drawRoundRect(
            SkRect::MakeXYWH(x, getY() + 5, 2, getHeight() - 10),
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

    void EditTextRenderer::nextChar()
    {
        const std::string &t = getText();

        if (isSelected)
        {
            currentIndex = endIndex;
            isSelected = false;
            return;
        }

        if (currentIndex >= (int)t.size())
            return;

        currentIndex = nextUtf8Index(t, currentIndex);
    }

    void EditTextRenderer::backChar()
    {
        const std::string &t = getText();

        if (isSelected)
        {
            currentIndex = beginIndex;
            isSelected = false;
            return;
        }

        if (currentIndex <= 0)
            return;

        currentIndex = prevUtf8Index(t, currentIndex);
    }

    int EditTextRenderer::nextUtf8Index(const std::string &s, int i)
    {
        if (i >= (int)s.size())
            return s.size();

        unsigned char c = s[i];
        if ((c & 0x80) == 0x00)
            return i + 1; // 1 byte
        if ((c & 0xE0) == 0xC0)
            return i + 2; // 2 byte
        if ((c & 0xF0) == 0xE0)
            return i + 3; // 3 byte
        if ((c & 0xF8) == 0xF0)
            return i + 4; // 4 byte

        return i + 1;
    }

    int EditTextRenderer::prevUtf8Index(const std::string &s, int i)
    {
        if (i <= 0)
            return 0;

        i--;
        while (i > 0 && ((s[i] & 0xC0) == 0x80))
        {
            i--;
        }
        return i;
    }

    float EditTextRenderer::caretX()
    {
        std::string t = getText();

        std::string left = t.substr(0, currentIndex);

        SkRect bounds;
        SkScalar width = text->getFont().measureText(
            left.c_str(),
            left.size(),
            SkTextEncoding::kUTF8,
            &bounds);

        return getX() + 5 + width;
    }

} // namespace Skia
