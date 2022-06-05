#include "vistas.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->img->setStyleSheet("QLabel { background-image: url(:/resources/img/dune.png); }"); // fondo
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint); // no resizeable
    //move(screen()->geometry().center() - frameGeometry().center()); // centrada

    QBrush tb(Qt::transparent); // botones transparentes
    ui->botonCreditos->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));
    ui->botonJugar->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));
    ui->botonSalir->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));

    QObject::connect(ui->botonJugar, &QPushButton::released, this, &MainWindow::jugar); // handlers
    QObject::connect(ui->botonCreditos, &QPushButton::released, this, &MainWindow::handleCreditos);
    QObject::connect(ui->botonSalir, &QPushButton::released, this, &QCoreApplication::quit);
}

void MainWindow::handleCreditos(){
    qDebug() << "Creditos";
}

MainWindow::~MainWindow()
{
    delete ui;
}
