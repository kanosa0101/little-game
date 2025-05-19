#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QSet>
#include <QGraphicsRectItem>
#include "character.h"

class GameScene : public QGraphicsScene {
    Q_OBJECT
public:
    GameScene(QObject *parent = nullptr);
    QList<QGraphicsRectItem*> platforms; // 存储所有平台
    QList<QGraphicsItem*> collidableItems;

    qreal m_sceneLeft;
    qreal m_sceneRight;
    qreal m_sceneTop;
    qreal m_sceneBottom;

signals:
    void returnToMainMenu();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void gameUpdate();

private:
    Character *m_character;
    QTimer *m_gameTimer;
    QSet<int> m_pressedKeys;

    void setupScene();
    void loadAnimations();
};

#endif // GAMESCENE_H
