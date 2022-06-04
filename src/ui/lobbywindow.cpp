#include "vistas.h"
#include "./ui_lobbywindow.h"
#include <QScreen>
#include <QDebug>
#include <QRadioButton>
#include <QMediaPlayer>
#include <QFontDatabase>
#include <QColorDialog>

LobbyWindow::LobbyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LobbyWindow)
    , buttongroup(new QButtonGroup)
{
    ui->setupUi(this);

    mMediaPlayer = new QMediaPlayer(this);
    mMediaPlayer->setMedia(QUrl("qrc:/resources/music/lobby.mp3"));
    mMediaPlayer->setVolume(50);
    mMediaPlayer->play();
    connect(mMediaPlayer,&QMediaPlayer::mediaStatusChanged,mMediaPlayer,&QMediaPlayer::play);

    move(screen()->geometry().center() - frameGeometry().center()); // centrada

    QPixmap p;
    p.load(":/resources/img/lobby.png");
    int w = ui->img->width();
    int h = ui->img->height();
    ui->img->setPixmap(p.scaled(w,h,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    ui->vbCrearPartida->setVisible(false);
    ui->vbUnirsePartida->setVisible(false);

    QObject::connect(ui->btnCrear, &QPushButton::released, this, &LobbyWindow::handleOpcionCrear); // handlers
    QObject::connect(ui->btnListar, &QPushButton::released, this, &LobbyWindow::handleListar);
    QObject::connect(ui->btnConectarse, &QPushButton::released, this, &LobbyWindow::handleOpcionConectarse);
    QObject::connect(ui->sliderJugadores, &QSlider::valueChanged, this, [=] () {
        ui->nJugadores->setText(QString::number(ui->sliderJugadores->value()));
    });
    QObject::connect(ui->btnCrearPartida, &QPushButton::released, this, &LobbyWindow::handleCrear);
    QObject::connect(ui->btnUnirse, &QPushButton::released, this, &LobbyWindow::handleConectarse);

    QRadioButton* radioHarkonnen = new QRadioButton("Harkonnen", this); // botones de faccion
    QPalette palette = radioHarkonnen->palette();
    QColor color = "white";
    palette.setColor(QPalette::WindowText, color);
    radioHarkonnen->setPalette(palette);

    radioHarkonnen->setChecked(true);
    QRadioButton* radioAtreides = new QRadioButton("Atreides", this);
    palette = radioAtreides->palette();
    palette.setColor(QPalette::WindowText, color);
    radioAtreides->setPalette(palette);

    QRadioButton* radioOrdos = new QRadioButton("Ordos", this);
    palette = radioOrdos->palette();
    palette.setColor(QPalette::WindowText, color);
    radioOrdos->setPalette(palette);

    buttongroup->addButton(radioHarkonnen);
    buttongroup->addButton(radioAtreides);
    buttongroup->addButton(radioOrdos);
    buttongroup->setExclusive(true);

    QList<QAbstractButton*> buttonList=buttongroup->buttons();
    for (QList<QAbstractButton*>::const_iterator it=buttonList.cbegin(); it!=buttonList.cend(); ++it) {
        ui->hbFacciones->addWidget(*it);
    }

    int id = QFontDatabase::addApplicationFont(":/resources/fonts/beyond-mars.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont beyond(family);
    beyond.setPointSize(32);

    ui->lblFaccion->setFont(beyond);
    ui->lblNombre->setFont(beyond);

    beyond.setPointSize(16);
        ui->lblNombre_2->setFont(beyond);
            ui->lblNombre_3->setFont(beyond);
                ui->lblJugs->setFont(beyond);
                    ui->nJugadores->setFont(beyond);

    palette = ui->lblNombre->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->lblNombre->setPalette(palette);

    palette = ui->lblNombre_2->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->lblNombre_2->setPalette(palette);

    palette = ui->lblNombre_3->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->lblNombre_3->setPalette(palette);

    palette = ui->lblJugs->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->lblJugs->setPalette(palette);

    palette = ui->lblFaccion->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->lblFaccion->setPalette(palette);

    palette = ui->nJugadores->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->nJugadores->setPalette(palette);
}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::handleOpcionCrear()
{
    ui->vbCrearPartida->setVisible(true);
    ui->vbUnirsePartida->setVisible(false);
}

void LobbyWindow::handleOpcionConectarse()
{
    ui->vbCrearPartida->setVisible(false);
    ui->vbUnirsePartida->setVisible(true);
}

void LobbyWindow::handleCrear()
{
    QString nombre = ui->nombrePartida->toPlainText();
    QString faccion = buttongroup->checkedButton()->text();
    QString n = QString::number(ui->sliderJugadores->value());

    ui->consola->append("Creando partida " + nombre + " con facción " + faccion + ". Jugadores requeridos: " + n + "\n");
}

void LobbyWindow::handleConectarse()
{
    QString nombre = ui->nombrePartida_2->toPlainText();
    QString faccion = buttongroup->checkedButton()->text();

    ui->consola->append("Uniéndose a partida " + nombre + " con facción " + faccion + "\n");
}

void LobbyWindow::handleListar()
{
    ui->consola->append("Listando partidas existentes... \n");
}

void LobbyWindow::on_toggle_clicked(){
    if (ui->toggle->text() == "Play") {
        mMediaPlayer->play();
        ui->toggle->setText("Pause");
    } else {
        mMediaPlayer->pause();
        ui->toggle->setText("Play");
    }
}

void LobbyWindow::on_mute_clicked(){
    if (ui->mute->text() == "Mute") {
        mMediaPlayer->setMuted(true);
        ui->mute->setText("Unmute");
    } else {
        mMediaPlayer->setMuted(false);
        ui->mute->setText("Mute");
    }
}
void LobbyWindow::on_volumen_valueChanged(int value){
    mMediaPlayer->setVolume(value);
}
