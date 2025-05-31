#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "animation.h"

class Character : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    enum State { Idle, Walking, Jumping, Falling, Slashing };
    enum Direction { Left, Right };

    Character(QGraphicsItem *parent = nullptr);

    //移动函数
    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    void update();

    //攻击函数
    void slashLeft();
    void slashRight();

    //移动参数
    void setGravity(float gravity);
    void setMoveSpeed(float speed);
    void setJumpForce(float force);

    //移动动画
    void loadWalkRightAnimation(const QList<QPixmap> &frames);
    void loadWalkLeftAnimation(const QList<QPixmap> &frames);
    void loadIdleRightAnimation(const QPixmap &frame);
    void loadIdleLeftAnimation(const QPixmap &frame);
    void loadJumpAnimation(const QPixmap &frame);

    //攻击动画
    void loadSlashLeftAnimation(const QList<QPixmap> &frames);
    void loadSlashRightAnimation(const QList<QPixmap> &frames);

    State currentState() const { return m_currentState; }
    Direction currentDirection() const { return m_currentDirection; }

    bool getDirection() {return m_facingRight; }

    void resetState() {m_currentState = Idle; }

private:

    //移动参数
    float m_velocityX;
    float m_velocityY;
    float m_gravity;
    float m_moveSpeed;
    float m_jumpForce;

    bool m_isOnGround;
    bool m_facingRight; // true=右, false=左
    bool m_isSlashing;

    int m_slashCooldown;

    //状态
    State m_currentState;
    Direction m_currentDirection;

    //帧
    Animation m_walkRightAnim;
    Animation m_walkLeftAnim;
    Animation m_slashRightAnim;
    Animation m_slashLeftAnim;

    QPixmap m_idleFrame;
    QPixmap m_idleLeft;
    QPixmap m_idleRight;
    QPixmap m_jumpFrame;

    int m_attackFrameCounter = 0;  // 跟踪攻击动画的帧计数
    const int ATTACK_TOTAL_FRAMES = 8;  // 攻击动画总帧数

    void updateAnimation();
    void checkGroundCollision();
};

#endif // CHARACTER_H
