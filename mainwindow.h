// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class GameScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startGame();
    void exitGame();
    void showMainMenu();
    void showInformation();

private:
    QStackedWidget *m_stackedWidget;
    QGraphicsView *m_view;
    GameScene *m_scene;
    QWidget *m_mainMenu;

    void setupMainMenu();
};

#endif // MAINWINDOW_H
