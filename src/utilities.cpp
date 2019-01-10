#include "utilities.h"

QFont computeFontForText(QRect rect, int flags, const QString &text, double scale)
{
    QFont correctFont = QApplication::font();

    rect.setWidth(static_cast<int>(rect.width() * scale));
    rect.setHeight(static_cast<int>(rect.height() * scale));

    for (int i = 1; i < 1000; ++i) {
        correctFont.setPixelSize(i);
        auto tempRect = QFontMetrics(correctFont).boundingRect(rect, flags, text);
        if (tempRect.height() > rect.height() || tempRect.width() > rect.width()) {
            correctFont.setPixelSize(i - 1);
            return correctFont;
        }
    }
    return correctFont;
}

int generateRandomNumber(int from, int to)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> uid(from, to);
    return uid(gen);
}
