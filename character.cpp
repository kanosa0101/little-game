//character.cpp
#include "character.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

Character::Character(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent),
    m_velocityX(0), m_velocityY(0),
    m_gravity(0.5f), m_moveSpeed(5.0f), m_jumpForce(12.0f),
    m_isOnGround(true),m_facingRight(true),
    m_isSlashing(false), m_slashCooldown(0),
    m_currentState(Idle) {
    setPixmap(QPixmap(":roles/Images/Right - Walking_000.png"));
    setTransformOriginPoint(boundingRect().center());
}

void Character::loadWalkRightAnimation(const QList<QPixmap> &frames) {
    m_walkRightAnim = Animation();
    for(const QPixmap &frame : frames) {
        m_walkRightAnim.addFrame(frame);
    }
    m_walkRightAnim.setFrameRate(18);
}

void Character::loadWalkLeftAnimation(const QList<QPixmap> &frames) {
    m_walkLeftAnim = Animation();
    for(const QPixmap &frame : frames) {
        m_walkLeftAnim.addFrame(frame);
    }
    m_walkLeftAnim.setFrameRate(18);
}

void Character::loadSlashRightAnimation(const QList<QPixmap> &frames) {
    m_slashRightAnim = Animation();
    for(const QPixmap &frame : frames) {
        m_slashRightAnim.addFrame(frame);
    }
    m_slashRightAnim.setFrameRate(9);
}

void Character::loadSlashLeftAnimation(const QList<QPixmap> &frames) {
    m_slashLeftAnim = Animation();
    for(const QPixmap &frame : frames) {
        m_slashLeftAnim.addFrame(frame);
    }
    m_slashLeftAnim.setFrameRate(9);
}


void Character::loadIdleRightAnimation(const QPixmap &pixmap) {
    m_idleRight = pixmap;
}

void Character::loadIdleLeftAnimation(const QPixmap &pixmap) {
    m_idleLeft = pixmap;
}

void Character::loadJumpAnimation(const QPixmap &frame) {
    m_jumpFrame = frame;
}

void Character::moveLeft() {
    m_facingRight = false;
    m_velocityX = -m_moveSpeed;
}

void Character::moveRight() {
    m_facingRight = true;
    m_velocityX = m_moveSpeed;
}

void Character::slashLeft() {
    if(!m_isSlashing) {
        m_facingRight = false;
        m_currentState = Slashing;
        m_isSlashing = true;
        m_attackFrameCounter = 0;  // 重置攻击帧计数
        m_slashLeftAnim.reset();  // 重置动画
    }
}

void Character::slashRight() {
    if(!m_isSlashing) {
        m_facingRight = true;
        m_currentState = Slashing;
        m_isSlashing = true;
        m_attackFrameCounter = 0;  // 重置攻击帧计数
        m_slashRightAnim.reset();  // 重置动画
    }
}

void Character::stopMoving() {
    m_velocityX = 0;
    if(m_facingRight) {
        // 使用面向右的静止帧
        setPixmap(m_idleRight);
    } else {
        // 使用面向左的静止帧
        setPixmap(m_idleLeft);
    }
}

void Character::jump() {
    if(m_isOnGround) {
        m_velocityY = -m_jumpForce;
        m_isOnGround = false;
        m_currentState = Jumping;
    }
}

void Character::update() {
    // 应用物理
    m_velocityY += m_gravity;
    m_velocityY = qMin(m_velocityY, 15.0f); // 限制下落速度

    qreal newX = x() + m_velocityX;
    if(newX < -256){
        newX = -256;
    }
    else if(newX > 7936){
        newX = 7936;
    }
    qreal newY = y() + m_velocityY;

    // 更新位置
    setPos(newX, newY);

    // 碰撞检测
    checkGroundCollision();

    // 更新状态 - 攻击状态优先
    if(m_isSlashing){
        // 如果正在攻击，保持攻击状态直到完成
        m_currentState = Slashing;
    } else {
        // 其他状态的判断
        if(!m_isOnGround) {
            m_currentState = (m_velocityY < 0) ? Jumping : Falling;
        } else if(qAbs(m_velocityX) > 0.1f) {
            m_currentState = Walking;
        } else {
            m_currentState = Idle;
        }
    }

    // 更新动画
    updateAnimation();

    // 减速
    m_velocityX *= 0.9f;
    if(qAbs(m_velocityX) < 0.1f) m_velocityX = 0;
}

void Character::checkGroundCollision() {
    // 角色底部中心检测点
    QPointF bottomCenter(
        x() + boundingRect().width() / 2,
        y() + boundingRect().height()
        );

    // 初始化状态
    m_isOnGround = false;

    // 检测与底部中心点相交的所有物体
    QList<QGraphicsItem*> collidingItems = scene()->items(bottomCenter);

    for (QGraphicsItem *item : collidingItems) {
        // 跳过自己（防止检测到自己）
        if (item == this) continue;

        // 获取物体的顶部 y 坐标
        qreal itemTop = item->y();

        // 如果角色底部 >= 物体顶部，且正在下落
        if (bottomCenter.y() - 20 >= itemTop && m_velocityY > 0) {
            setPos(x(), itemTop - boundingRect().height() + 20); // 对齐到物体顶部
            m_velocityY = 0;
            m_isOnGround = true;
            break;
        }
    }
}

void Character::updateAnimation() {
    switch(m_currentState) {
    case Walking:
        if(m_velocityX > 0) {
            m_walkRightAnim.update();
            setPixmap(m_walkRightAnim.currentFrame());
        } else {
            m_walkLeftAnim.update();
            setPixmap(m_walkLeftAnim.currentFrame());
        }
        break;
    case Jumping:
    case Falling:
        if(m_facingRight) {
            setPixmap(m_idleRight);
        } else {
            setPixmap(m_idleLeft);
        }
        break;
    case Slashing:
        if(m_facingRight) {
            // 保存当前帧索引
            int oldFrame = m_slashRightAnim.getCurrentFrameIndex();
            m_slashRightAnim.update();
            setPixmap(m_slashRightAnim.currentFrame());

            // 只有当动画真正切换到下一帧时才增加计数器
            int newFrame = m_slashRightAnim.getCurrentFrameIndex();
            if(newFrame != oldFrame || (newFrame == 0 && m_attackFrameCounter > 0)) {
                m_attackFrameCounter++;
            }

            // 检查动画是否完成（已播放完所有帧）
            if(m_attackFrameCounter >= m_slashRightAnim.getTotalFrames()) {
                m_isSlashing = false;
                m_currentState = Idle;
                m_attackFrameCounter = 0;
            }
        } else {
            // 保存当前帧索引
            int oldFrame = m_slashLeftAnim.getCurrentFrameIndex();
            m_slashLeftAnim.update();
            setPixmap(m_slashLeftAnim.currentFrame());

            // 只有当动画真正切换到下一帧时才增加计数器
            int newFrame = m_slashLeftAnim.getCurrentFrameIndex();
            if(newFrame != oldFrame || (newFrame == 0 && m_attackFrameCounter > 0)) {
                m_attackFrameCounter++;
            }

            // 检查动画是否完成
            if(m_attackFrameCounter >= m_slashLeftAnim.getTotalFrames()) {
                m_isSlashing = false;
                m_currentState = Idle;
                m_attackFrameCounter = 0;
            }
        }
        break;
    case Idle:
    default:
        if(m_facingRight) {
            setPixmap(m_idleRight);
        } else {
            setPixmap(m_idleLeft);
        }
        break;
    }
}

void Character::setGravity(float gravity) { m_gravity = gravity; }
void Character::setMoveSpeed(float speed) { m_moveSpeed = speed; }
void Character::setJumpForce(float force) { m_jumpForce = force; }
