#ifndef VISTAS_H
#define VISTAS_H

#include <QMainWindow>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
    class LobbyWindow;
    class FactionWindow;
    class CreditsWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void jugar();
    void credits();

private slots:

private:
    Ui::MainWindow *ui;
};

class QMediaPlayer;

class LobbyWindow : public QMainWindow
{
    Q_OBJECT

public:
    LobbyWindow(QWidget *parent = nullptr);
    ~LobbyWindow();

signals:
    void jugar();

private slots:
  void handleOpcionCrear();
  void handleOpcionConectarse();
  void handleListar();
  void handleCrear();
  void handleConectarse();
  void on_toggle_clicked();
  void on_mute_clicked();
  void on_volumen_valueChanged(int value);

private:
    Ui::LobbyWindow *ui;
    QButtonGroup *buttongroup;
    QMediaPlayer *mMediaPlayer;
};

class FactionWindow : public QMainWindow
{
    Q_OBJECT

public:
    FactionWindow(QWidget *parent = nullptr, int* faction = nullptr);
    ~FactionWindow();
    int* faction;

signals:
    void jugar();

private slots:

    void handleHarkonnen();
    void handleOrdos();
    void handleAtreides();

private:
    Ui::FactionWindow *ui;
    QButtonGroup *buttongroup;
    QMediaPlayer *mMediaPlayer;
};

class CreditsWindow : public QMainWindow
{
    Q_OBJECT

public:
    CreditsWindow(QWidget *parent = nullptr);
    ~CreditsWindow();

signals:
    void volver();

private slots:

private:
    Ui::CreditsWindow *ui;
    QButtonGroup *buttongroup;
    QMediaPlayer *mMediaPlayer;
};

#endif // VISTAS_H
