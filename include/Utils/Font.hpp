#pragma once

#include <QString>
#include <QFontDatabase>
#include <QStringList>

class Font
{
public:
    static QString getNano()
    {
        static QString family;
        if (family.isEmpty())
        {
            int id = QFontDatabase::addApplicationFont(":/fonts/nano.ttf");
            family = QFontDatabase::applicationFontFamilies(id).at(0);
        }

        return family;
    }

    static QString getNunito()
    {
        static QString family;
        if (family.isEmpty())
        {
            int id = QFontDatabase::addApplicationFont(":/fonts/nunito.ttf");
            family = QFontDatabase::applicationFontFamilies(id).at(0);
        }

        return family;
    }
};