#pragma once

namespace CeltEngine
{
    enum class EventType : short
    {
        None = 0,
        WindowResize, WindowClose, WindowFocus, WindowLostFocus, WindowMoved,
        FrameBufferResize
    };

    enum class EventCategory : byte
    {
        None = 0,
        Application, Input, Keyboard, Mouse, Window
    };

    class Event
    {
    public:
        EventType Type = EventType::None;
        EventCategory Category = EventCategory::None;
        bool Handled = false;

        virtual ~Event() = default;
    };

    class FrameBufferResizeEvent : public Event
    {
    public:
        FrameBufferResizeEvent(int width, int height)
        {
            Type = EventType::FrameBufferResize;
            Category = EventCategory::None;
            m_Width = width;
            m_Height = height;
        }

        int GetNewWidth() const { return m_Width; }
        int GetNewHeight() const { return m_Height; }
    private:
        int m_Width, m_Height;
    };

    class WindowEvent : public Event
    {
    protected:
        WindowEvent()
        {
            Category = EventCategory::Window;
        }
    };

    class WindowResizeEvent : public WindowEvent
    {
    public:
        WindowResizeEvent(int width, int height) : WindowEvent()
        {
            Type = EventType::WindowResize;
            m_Width = width;
            m_Height = height;
        }

        int GetNewWidth() const { return m_Width; }
        int GetNewHeight() const { return m_Height; }
    private:
        int m_Width, m_Height;
    };

    class WindowMovedEvent : public WindowEvent
    {
    public:
        WindowMovedEvent(int x, int y) : WindowEvent()
        {
            Type = EventType::WindowMoved;
            m_X = x;
            m_Y = y;
        }

        int GetNewPosX() const { return m_X; }
        int GetNewPosY() const { return m_Y; }
    private:
        int m_X, m_Y;
    };

    class WindowCloseEvent : public WindowEvent
    {
    public:
        WindowCloseEvent() : WindowEvent()
        {
            Type = EventType::WindowClose;
        }
    };

    class WindowFocusEvent : public WindowEvent
    {
    public:
        WindowFocusEvent() : WindowEvent()
        {
            Type = EventType::WindowFocus;
        }
    };

    class WindowLostFocusEvent : public WindowEvent
    {
    public:
        WindowLostFocusEvent() : WindowEvent()
        {
            Type = EventType::WindowLostFocus;
        }
    };
}
