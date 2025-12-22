#pragma once

#include <QObject>

namespace Signals
{
    class URLPReview : public QObject
    {
        Q_OBJECT
    public:
        explicit URLPReview(QObject *parent = nullptr) : QObject(parent) {}

        static void show(std::string url) {
            emit instance()->showPreview(url);
        }

        static void close() {
            emit instance()->closePreview();
        }

        static URLPReview *instance()
        {
            static URLPReview inst;
            return &inst;
        }
    signals:
        void showPreview(std::string url);
        void closePreview();
    };
} // namespace Signals
