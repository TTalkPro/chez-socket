//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_BASE_HANDLER_H
#define CZSOCKET_BASE_HANDLER_H

#include <memory>
class reactor;

class base_handler : public  std::enable_shared_from_this<base_handler>
{
    friend class reactor;

public:
    base_handler(): _reactor(nullptr), _events(0), _pending_events(0)
    {
    };
    void watch(int events)
    {
        _pending_events |= events;
    };
    void unwatch(int events)
    {
        _pending_events &= ~events;
    };
    virtual ~base_handler();

    int events() const { return _events; };
    int pending_events() const { return _pending_events; };
    void apply_pending_events() { _events = _pending_events; };

protected:
    std::shared_ptr<reactor> attached_reactor() const { return _reactor; };
    void attach_reactor(const std::shared_ptr<reactor>& r) { _reactor = r; };
    void detach_reactor() { _reactor = nullptr; };

private:
    std::shared_ptr<reactor> _reactor;
    int _pending_events;
    int _events;
};

#endif //CZSOCKET_BASE_HANDLER_H
