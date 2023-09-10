#pragma once

#include <iostream>
#include "SpriteAnimation.h"

class AnimatorController;

class AnimatorState
{
public:
    AnimatorState();
    AnimatorState(const AnimatorState& other)
        : m_spriteAnimation(other.m_spriteAnimation)
        , m_done(other.m_done)
    {
        std::cout << "AnimatorState Copy Constructor\n";
    }

    AnimatorState(SpriteAnimation spriteAnimation);
        
    SpriteAnimation& GetSpriteAnimation()
    {
        return m_spriteAnimation;
    }

    void Process(AnimatorController& fsm);

    inline bool IsDone() const { return m_done; }

private:
    SpriteAnimation m_spriteAnimation;
    bool m_done = false;
};
