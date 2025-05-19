// mainwindow.cpp
#include "mainwindow.h"
#include "gamescene.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_stackedWidget(new QStackedWidget(this))
{
    // 设置主窗口为全屏
    showFullScreen();

    // 创建堆叠窗口
    setCentralWidget(m_stackedWidget);

    // 设置主菜单
    setupMainMenu();

    // 创建游戏视图和场景
    m_view = new QGraphicsView();
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setBackgroundBrush(Qt::lightGray);


    m_scene = new GameScene(this);

    connect(m_scene, &GameScene::returnToMainMenu, this, &MainWindow::showMainMenu);

    m_view->setScene(m_scene);

    // 添加视图到堆叠窗口
    m_stackedWidget->addWidget(m_mainMenu);
    m_stackedWidget->addWidget(m_view);
}

void MainWindow::setupMainMenu()
{
    m_mainMenu = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_mainMenu);

    // 设置主菜单样式
    m_mainMenu->setStyleSheet("background-color: #333;");

    // 创建标题
    QLabel *title = new QLabel("小游戏");
    title->setStyleSheet("font-size: 48px; color: white; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);

    // 创建按钮
    QPushButton *startButton = new QPushButton("开始游戏");
    QPushButton *informationButton = new QPushButton("游戏说明");
    QPushButton *exitButton = new QPushButton("退出游戏");

    // 设置按钮样式
    QString buttonStyle = "QPushButton {"
                          "background-color: #4CAF50;"
                          "border: none;"
                          "color: white;"
                          "padding: 15px 32px;"
                          "text-align: center;"
                          "text-decoration: none;"
                          "font-size: 24px;"
                          "margin: 4px 2px;"
                          "border-radius: 8px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #45a049;"
                          "}";

    startButton->setStyleSheet(buttonStyle);
    informationButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle.replace("#4CAF50", "#f44336").replace("#45a049", "#d32f2f"));

    // 连接按钮信号
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(informationButton, &QPushButton::clicked, this, &MainWindow::showInformation);
    connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    // 添加控件到布局
    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(50);
    layout->addWidget(startButton);
    layout->addSpacing(20);
    layout->addWidget(informationButton);
    layout->addSpacing(20);
    layout->addWidget(exitButton);
    layout->addStretch();

    // 设置布局对齐方式
    layout->setAlignment(Qt::AlignCenter);
}

void MainWindow::startGame()
{
    m_stackedWidget->setCurrentIndex(1); // 切换到游戏视图
    m_view->setFocus(); // 确保焦点在视图上
}

void MainWindow::exitGame()
{
    QApplication::quit();
}

MainWindow::~MainWindow()
{
    // 清理资源
}

void MainWindow::showMainMenu()
{
    m_stackedWidget->setCurrentIndex(0); // 切换回主菜单
    m_scene->clear(); // 可选：清除场景内容
    // 重置游戏状态等其他清理工作...
}

void MainWindow::showInformation(){

}
