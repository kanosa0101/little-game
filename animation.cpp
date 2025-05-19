//animation.cpp
#include "animation.h"

Animation::Animation()
    : m_currentFrame(0), m_frameCounter(0), m_frameDelay(5) {}

void Animation::addFrame(const QPixmap &frame) {
    m_frames.append(frame);
}

void Animation::setFrameRate(int fps) {
    m_frameDelay = qMax(1, 60 / fps); // 确保至少1帧延迟
}

QPixmap Animation::currentFrame() const {
    return m_frames.isEmpty() ? QPixmap() : m_frames.at(m_currentFrame);
}

void Animation::update() {
    if(m_frames.size() <= 1) return;

    if(++m_frameCounter >= m_frameDelay) {
        m_frameCounter = 0;
        m_currentFrame = (m_currentFrame + 1) % m_frames.size();
    }
}

void Animation::reset() {
    m_currentFrame = 0;
    m_frameCounter = 0;
}

bool Animation::isEmpty() const {
    return m_frames.isEmpty();
}
