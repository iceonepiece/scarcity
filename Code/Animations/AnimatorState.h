#pragma once

#include <iostream>
#include <vector>
#include "SpriteAnimation.h"
#include "Animations/AnimationClip.h"
#include "Animations/AnimationSerializer.h"
#include "Animations/AnimatorTransition.h"

class AnimatorController;

class AnimatorState
{
public:
    AnimatorState();

    /*
    AnimatorState(const AnimatorState& other)
        : m_spriteAnimation(other.m_spriteAnimation)
        , m_done(other.m_done)
    {
        std::cout << "AnimatorState Copy Constructor\n";
    }
    */

    AnimatorState& operator=(const AnimatorState& other);
    AnimatorState(SpriteAnimation spriteAnimation);

    /*
    SpriteAnimation GetSpriteAnimation() const
    {
        return m_spriteAnimation;
    }
    */

    void Process(AnimatorController& fsm);

    inline bool IsDone() const { return m_done; }

private:
    //SpriteAnimation m_spriteAnimation;

    std::string m_name;
    AnimationClip* m_motion;
    float m_speed;
    std::vector<AnimatorTransition> m_transitions;

    bool m_done = false;

    friend class AnimationSerializer;
};
