#include <stdint.h>
#include <vector>
#include <utility>
#include <algorithm>
#include "event.h"
#include "command_queues.h"
//#include <functional>

typedef bool (*detect_event_cb_t)(HardwareEvent event, HardwareEvent criteria);
typedef void (*run_event_cb_t)(extended_keycodes_t key);

class Trigger;

typedef std::vector<Trigger> Triggers_t;

class Trigger {
  public:
    Trigger(HardwareEvent crit, detect_event_cb_t dl, run_event_cb_t cmd ) 
              : _criteria{crit}, _detectlogic{ dl }, _command_to_run {cmd} {};
    void detectEvent(HardwareEvent event);

    Triggers_t sub_triggers;
    
  protected:
    HardwareEvent _criteria;
    detect_event_cb_t _detectlogic;
    run_event_cb_t _command_to_run;
    
    
};

 bool typecheck(HardwareEvent event, HardwareEvent criteria);
 bool idcheck(HardwareEvent event, HardwareEvent criteria);
 bool typeidcheck(HardwareEvent event, HardwareEvent criteria);
