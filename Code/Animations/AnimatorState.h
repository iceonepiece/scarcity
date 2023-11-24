#pragma once

#include <iostream>
#include <vector>
#include <imgui/imgui.h>
#include "SpriteAnimation.h"
#include "Animations/AnimationClip.h"
#include "Animations/AnimationSerializer.h"
#include "Animations/AnimatorTransition.h"

class AnimatorController;

class AnimatorState
{
public:
    AnimatorState(const std::string& name = "New State");
    ~AnimatorState();

    void SetController(AnimatorController* controller) { m_controller = controller; }
    AnimatorController* GetController() { return m_controller; }

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

    std::string GetName() { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    ImVec2& GetPosition() { return m_position; }
    bool& GetSelected() { return m_selected; }


    std::string m_name;
    ImVec2 m_position;
    bool m_selected = false;
    AnimationClip* m_motion;
    float m_speed;

    void AddOutgoingTransition(AnimatorTransition* transition);
    void AddIncomingTransition(AnimatorTransition* transition);

    bool RemoveOutgoingTransition(AnimatorTransition* transition);
    bool RemoveIncomingTransition(AnimatorTransition* transition);

    bool RemoveTransition(AnimatorState* toState);

    std::vector<AnimatorTransition*>& GetOutgoingTransitions() { return m_outgoingTransitions; }
    std::vector<AnimatorTransition*>& GetIncomingTransitions() { return m_incomingTransitions; }

private:
    AnimatorController* m_controller;
    //SpriteAnimation m_spriteAnimation;
    std::vector<AnimatorTransition*> m_outgoingTransitions;
    std::vector<AnimatorTransition*> m_incomingTransitions;

    bool m_done = false;

    friend class AnimationSerializer;
};
