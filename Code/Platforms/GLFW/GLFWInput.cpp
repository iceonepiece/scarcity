#include "GLFWInput.h"

std::vector<KeyCode> DEFAULT_KEY_CODES = {
    Key::Escape,
    Key::Space,
    Key::Enter,
    Key::Left,
    Key::Right,
    Key::Up,
    Key::Down,
    Key::LeftControl,
    Key::RightControl,
	Key::A,
    Key::B,
    Key::C,
    Key::D,
    Key::E,
    Key::F,
    Key::G,
    Key::H,
    Key::I,
    Key::J,
    Key::K,
    Key::L,
    Key::M,
    Key::N,
    Key::O,
    Key::P,
    Key::Q,
    Key::R,
    Key::S,
    Key::T,
    Key::U,
    Key::V,
    Key::W,
    Key::X,
    Key::Y,
    Key::Z
};

std::vector<MouseCode> DEFAULT_MOUSE_CODES = {
    Mouse::ButtonLeft,
    Mouse::ButtonRight,
    Mouse::ButtonMiddle
};

GLFWInput::GLFWInput(GLFWwindow& window)
    : Input(DEFAULT_KEY_CODES, DEFAULT_MOUSE_CODES)
	, m_window(window)
{
}

void GLFWInput::Poll()
{
    for (KeyCode key : m_keyCodes)
    {
        UpdateKeyState(key, glfwGetKey(&m_window, key));
    }

    for (MouseCode code : m_mouseCodes)
    {
        UpdateMouseState(code, glfwGetMouseButton(&m_window, code));
    }
}