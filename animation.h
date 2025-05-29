#ifndef ANIMATION_H
#define ANIMATION_H

#include <QList>
#include <QPixmap>

class Animation {
public:
    Animation();

    void addFrame(const QPixmap &frame);
    void setFrameRate(int fps);
    QPixmap currentFrame() const;
    void update();
    void reset();
    bool isEmpty() const;

    int getCurrentFrameIndex() const { return m_currentFrame; }
    int getTotalFrames() const { return m_frames.size(); }

private:
    QList<QPixmap> m_frames;
    int m_currentFrame;
    int m_frameCounter;
    int m_frameDelay;

};

#endif // ANIMATION_H
