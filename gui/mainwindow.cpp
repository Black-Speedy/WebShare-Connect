#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "iconutils.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QButtonGroup>
#include <QOverload>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSvgRenderer renderer(QStringLiteral(":/icons/send.svg"));
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);  // Make background transparent

    QPainter painter(&pixmap);
    renderer.render(&painter);

    ui->sendIcon->setPixmap(pixmap);

    ui->settingButton->setIconSize(QSize(20, 20));
    ui->settingButton->setText("  Settings");
    ui->settingButton->setIcon(
        IconUtils::createColoredSvgIcon(":/icons/settings.svg", QColor("#8b5cf6"), QSize(20, 20)) // dunno what i cooked here but it works
        );

    ui->sendButton->setCheckable(true);
    ui->receiveButton->setCheckable(true);

    ui->sendButton->setChecked(true);

    QButtonGroup* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->sendButton, 0);
    buttonGroup->addButton(ui->receiveButton, 1);
    buttonGroup->setExclusive(true);

    connect(buttonGroup,
            &QButtonGroup::buttonClicked,
            this,
            [this](QAbstractButton* btn) {
                if (btn == ui->sendButton)
                    ui->cardFrame->setCurrentIndex(0);
                else if (btn == ui->receiveButton)
                    ui->cardFrame->setCurrentIndex(1);
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}
