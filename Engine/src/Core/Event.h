#pragma once

namespace CeltEngine
{
    enum class EventType
    {
        None = 0,
    };

    class Event
    {
    public:
        EventType Type = EventType::None;
        bool Handled = false;

        virtual ~Event() = default;
    };
}
