#include "iconutils.h"

#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>

QIcon IconUtils::createColoredSvgIcon(const QString &svgPath, const QColor &color, const QSize &size) {
    QSvgRenderer renderer(svgPath);
    QPixmap base(size);
    base.fill(Qt::transparent);

    QPainter painter(&base);
    painter.setRenderHint(QPainter::Antialiasing, true);

    renderer.render(&painter);

    // Apply color overlay via composition mode
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(base.rect(), color);

    return QIcon(base);
}
