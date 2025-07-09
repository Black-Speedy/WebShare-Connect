#ifndef ICONUTILS_H
#define ICONUTILS_H

#include <QIcon>
#include <QColor>
#include <QString>
#include <QSize>

class IconUtils {
public:
    static QIcon createColoredSvgIcon(const QString &svgPath, const QColor &color, const QSize &size);
};

#endif // ICONUTILS_H
