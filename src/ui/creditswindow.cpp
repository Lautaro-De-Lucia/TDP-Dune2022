#include "vistas.h"
#include "./ui_creditswindow.h"
#include <QDebug>
#include <QDesktopWidget>

#include <QScreen>
#include <QRadioButton>

#include <QMediaPlayer>
#include <QFontDatabase>
#include <unistd.h>

#include <iostream>

#include "client_audioplayer.h"

CreditsWindow::CreditsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CreditsWindow)
{
    ui->setupUi(this);

    ui->img->setStyleSheet("QLabel { background-image: url(:/resources/img/credits.png); }"); // fondo
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint); // no resizeable
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

    QBrush tb(Qt::transparent); // botones transparentes

    ui->botonVolver->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));

    QObject::connect(ui->botonVolver, &QPushButton::released, this, &CreditsWindow::volver);
}

CreditsWindow::~CreditsWindow()
{
    delete ui;
}
