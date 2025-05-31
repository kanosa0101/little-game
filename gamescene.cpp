//gamescene.cpp
#include "gamescene.h"
#include <QKeyEvent>
#include <QGraphicsLineItem>
#include <QApplication>
#include <QFile>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_character(nullptr) {
    // 设置场景
    setupScene();

    // 创建角色
    m_character = new Character();
    addItem(m_character);
    m_character->setPos(100, 300);
    m_character->setZValue(1000);

    // 加载动画
    loadAnimations();

    // 游戏循环
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &GameScene::gameUpdate);
    //m_gameTimer->start(1000 / 90); // ~60 FPS

    connect(this, &GameScene::returnToMainMenu, [this]() {
        m_character->setPos(100, 300); // 重置角色位置
        m_gameTimer->stop(); // 暂停游戏循环
    });
}

void GameScene::setupScene() {
    // 获取主屏幕尺寸（全屏）
    QRectF screenRect = QApplication::primaryScreen()->geometry();
    qreal screenWidth = screenRect.width();
    qreal screenHeight = screenRect.height();

    // 设置场景边界（逻辑尺寸，可自定义）
    m_sceneLeft = 0;
    m_sceneRight = screenWidth;   // 或固定值如 1920
    m_sceneTop = 0;
    m_sceneBottom = screenHeight; // 或固定值如 1080

    setBackgroundBrush(Qt::lightGray);

    QPixmap grassBlock(":background/Images/ground.png");
    QPixmap testPix(":background/Images/ground.png");
    if(testPix.isNull()) {
        qDebug() << "ERROR: Failed to load default ground image!";
    } else {
        qDebug() << "Default image loaded, size:" << testPix.size();
    }

    // 添加草方块地面
    for (int x = -512; x < 8192; x += 128) {
        QGraphicsPixmapItem *block = new QGraphicsPixmapItem(grassBlock);
        block->setPos(x, 450);
        addItem(block);
        collidableItems.append(block); // 存储为 QGraphicsItem*
    }

    // 设置背景音乐
    setupBackgroundMusic();
}

void GameScene::loadAnimations() {
    // 测试资源加载
    QPixmap testPix(":roles/Images/Right - Walking_000.png");
    if(testPix.isNull()) {
        qDebug() << "ERROR: Failed to load default character image!";
    } else {
        qDebug() << "Default image loaded, size:" << testPix.size();
    }

    // 加载向右行走动画 (18帧)
    QList<QPixmap> walkRightFrames;
    for(int i = 0; i < 18; ++i) {
        QString path = QString(":roles/Images/Right - Walking_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap frame(path);
        if(frame.isNull()) {
            qDebug() << "Failed to load right walk frame:" << path;
        }
        walkRightFrames.append(frame);
    }

    // 加载向左行走动画 (18帧)
    QList<QPixmap> walkLeftFrames;
    for(int i = 0; i < 18; ++i) {
        QString path = QString(":roles/Images/Left - Walking_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap frame(path);
        if(frame.isNull()) {
            qDebug() << "Failed to load left walk frame:" << path;
        }
        walkLeftFrames.append(frame);
    }

    // 加载向右攻击动画 (9帧)
    QList<QPixmap> slashRightFrames;
    for(int i = 0; i < 8; ++i) {
        QString path = QString(":/roles/Images/Right - Slashing_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap frame(path);
        if(frame.isNull()) {
            qDebug() << "Failed to load right slash frame:" << path;
        }
        slashRightFrames.append(frame);
    }

    // 加载向左攻击动画 (9帧)
    QList<QPixmap> slashLeftFrames;
    for(int i = 0; i < 8; ++i) {
        QString path = QString(":/roles/Images/Left - Slashing_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap frame(path);
        if(frame.isNull()) {
            qDebug() << "Failed to load left slash frame:" << path;
        }
        slashLeftFrames.append(frame);
    }

    // 设置动画
    m_character->loadWalkRightAnimation(walkRightFrames);
    m_character->loadWalkLeftAnimation(walkLeftFrames);
    m_character->loadSlashRightAnimation(slashRightFrames);
    m_character->loadSlashLeftAnimation(slashLeftFrames);

    // 设置静止和跳跃动画
    m_character->loadIdleRightAnimation(QPixmap(":roles/Images/Right - Walking_000.png"));
    m_character->loadIdleLeftAnimation(QPixmap(":roles/Images/Left - Walking_000.png"));
    m_character->loadJumpAnimation(QPixmap(":roles/Images/Right - Walking_000.png"));
}

void GameScene::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Escape) {
        emit returnToMainMenu();
        return;
    }

    if(!event->isAutoRepeat() && m_gameRunning) {  // 添加游戏运行检查
        m_pressedKeys.insert(event->key());
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event) {
    if(!event->isAutoRepeat()) {
        m_pressedKeys.remove(event->key());
    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::gameUpdate() {
    // 处理输入
    bool moving = false;

    if(m_pressedKeys.contains(Qt::Key_Left) || m_pressedKeys.contains(Qt::Key_A)) {
        m_character->moveLeft();
        moving = true;
    }
    if(m_pressedKeys.contains(Qt::Key_Right) || m_pressedKeys.contains(Qt::Key_D)) {
        m_character->moveRight();
        moving = true;
    }

    if(!moving) {
        m_character->stopMoving();
    }

    if(m_pressedKeys.contains(Qt::Key_Space) || m_pressedKeys.contains(Qt::Key_W) || m_pressedKeys.contains(Qt::Key_Up)) {
        m_character->jump();
    }

    if(m_pressedKeys.contains(Qt::Key_J) && !m_attackKeyPressed){
        m_attackKeyPressed = true;
        if(m_character->getDirection()){
            m_character->slashRight();
        }
        else{
            m_character->slashLeft();
        }
    }
    else if(!m_pressedKeys.contains(Qt::Key_J)) {
        m_attackKeyPressed = false;  // 重置标记当J键释放
    }

    // 更新角色
    m_character->update();

    // 简单的相机跟随
    setSceneRect(m_character->x() - 200, 0, 800, 600);
}


void GameScene::setupBackgroundMusic() {
    backgroundMusic = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    backgroundMusic->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.3);  // 30% 音量（注意：Qt6 用 0.0~1.0）

    // 调试资源文件
    QFile file(":bgm/Music/backgroundmusic.mp3");
    qDebug() << "音乐文件是否存在:" << file.exists() << "大小:" << file.size();

    QUrl musicUrl = QUrl("qrc:bgm/Music/backgroundmusic.mp3");  // 显式使用 qrc:/
    backgroundMusic->setSource(musicUrl);

    connect(backgroundMusic, &QMediaPlayer::errorOccurred, [](auto error, auto msg) {
        qDebug() << "音乐错误:" << error << msg;
    });

    connect(backgroundMusic, &QMediaPlayer::mediaStatusChanged, [](auto status) {
        qDebug() << "媒体状态变化:" << status;  // 查看加载过程
    });

    backgroundMusic->setLoops(QMediaPlayer::Infinite);
    backgroundMusic->play();
}

void GameScene::startGame() {
    m_gameRunning = true;
    m_gameTimer->start(1000 / 90);
    if (backgroundMusic) {
        backgroundMusic->play();
    }
}

void GameScene::resetGame() {
    // 停止所有运动
    m_gameTimer->stop();
    m_pressedKeys.clear();
    m_attackKeyPressed = false;

    // 重置角色
    if (m_character) {
        m_character->setPos(100, 300);
        m_character->resetState();
    }

    // 重置背景音乐
    if (backgroundMusic) {
        backgroundMusic->stop();
        backgroundMusic->setPosition(0); // 回到开头
    }

    // 重置相机
    setSceneRect(m_character->x() - 400, 0, 800, 600);
}

void GameScene::pauseGame() {
    m_gameRunning = false;
    m_gameTimer->stop();
    m_pressedKeys.clear();  // 清除按键状态
}
