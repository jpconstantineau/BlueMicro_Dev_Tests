#include "trigger.h"



void Trigger::detectEvent(HardwareEvent event)
{
  if(_detectlogic(event,_criteria)){_command_to_run(event.key);}
}

 bool typecheck(HardwareEvent event, HardwareEvent criteria)
 { return (event.type == criteria.type);};

  bool idcheck(HardwareEvent event, HardwareEvent criteria)
 { return (event.source == criteria.source)&&(event.component == criteria.component);};

 bool typeidcheck(HardwareEvent event, HardwareEvent criteria)
 { return (event.type == criteria.type)&&(event.source == criteria.source)&&(event.component == criteria.component);};
