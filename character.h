#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "animation.h"

class Character : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    enum State { Idle, Walking, Jumping, Falling };

    Character(QGraphicsItem *parent = nullptr);

    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    void update();

    void setGravity(float gravity);
    void setMoveSpeed(float speed);
    void setJumpForce(float force);

    void loadWalkRightAnimation(const QList<QPixmap> &frames);
    void loadWalkLeftAnimation(const QList<QPixmap> &frames);
    void loadIdleRightAnimation(const QPixmap &frame);
    void loadIdleLeftAnimation(const QPixmap &frame);
    void loadJumpAnimation(const QPixmap &frame);

    State currentState() const { return m_currentState; }

private:
    float m_velocityX;
    float m_velocityY;
    float m_gravity;
    float m_moveSpeed;
    float m_jumpForce;
    bool m_isOnGround;
    bool m_facingRight; // true=右, false=左

    State m_currentState;
    Animation m_walkRightAnim;
    Animation m_walkLeftAnim;
    QPixmap m_idleFrame;
    QPixmap m_idleLeft;
    QPixmap m_idleRight;
    QPixmap m_jumpFrame;

    void updateAnimation();
    void checkGroundCollision();
};

#endif // CHARACTER_H
