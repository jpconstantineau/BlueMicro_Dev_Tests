#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <vector>
#include <utility>
#include <algorithm>
#include "command_queues.h"
typedef uint16_t HardwareEventComponent_t;
typedef uint16_t HardwareEventSource_t;



enum class HardwareEventType {
    NONE = 0,
    PRESS = 1,
    RELEASE = 2,
    TAP = 3,
    DOUBLETAP = 4,
    TRIPLETAP = 5,
    HOLD = 6,
};

class HardwareEvent {

    public:
       HardwareEvent(HardwareEventSource_t src, HardwareEventComponent_t cmp, HardwareEventType t)
       : type{t}, source{src}, component{cmp} {key=0;};

       
       HardwareEventType type;
       HardwareEventSource_t source;
       HardwareEventComponent_t component;
       extended_keycodes_t key; 
          
};


class HardwareEventDigital : public HardwareEvent {

    public:
       HardwareEventDigital(HardwareEventSource_t s, HardwareEventComponent_t cmp, HardwareEventType t)
        : HardwareEvent(s, cmp, t) {};

          
};

class HardwareEventAnalog : public HardwareEvent {

    public:
       HardwareEventAnalog(HardwareEventSource_t s, HardwareEventComponent_t cmp, HardwareEventType t)
          : HardwareEvent(s, cmp, t) {};
    
};

typedef std::vector < HardwareEvent > eventqueue_t;

#endif /* EVENT_H */
