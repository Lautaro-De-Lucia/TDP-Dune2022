#include "vistas.h"
#include "./ui_factionwindow.h"

#include <QScreen>
#include <QDebug>
#include <QRadioButton>
#include <QMediaPlayer>
#include <QFontDatabase>
#include <unistd.h>

#include <QDesktopWidget>

#include <iostream>

FactionWindow::FactionWindow(QWidget *parent, int* faction)
    : QMainWindow(parent)
    , ui(new Ui::FactionWindow)
{

    this->faction = faction;

    ui->setupUi(this);

    ui->img->setStyleSheet("QLabel { background-image: url(:/resources/img/factions.png); }"); // fondo
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint); // no resizeable
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

    QBrush tb(Qt::transparent); // botones transparentes

    ui->botonAtreides->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));
    ui->botonHarkonnen->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));
    ui->botonOrdos->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));

    QObject::connect(ui->botonAtreides, &QPushButton::released, this, &FactionWindow::handleAtreides);
    QObject::connect(ui->botonHarkonnen, &QPushButton::released, this, &FactionWindow::handleHarkonnen);
    QObject::connect(ui->botonOrdos, &QPushButton::released, this, &FactionWindow::handleOrdos);
}

void FactionWindow::handleAtreides(){
    *(this->faction) = 2;
    emit jugar();
}

void FactionWindow::handleHarkonnen(){
    *(this->faction) = 1;
    emit jugar();
}

void FactionWindow::handleOrdos(){
    *(this->faction) = 3;
    emit jugar();
}

FactionWindow::~FactionWindow() {
    delete ui;
}
