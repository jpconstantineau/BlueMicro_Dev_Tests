#include "actor.h"
#include <Arduino.h>
void Actor::notify(void)
{
  std::for_each(observers.cbegin(), observers.cend(),
      [](Actor observer){ 
        observer.update();
        });
}

void Actor::update(void)
{
 Serial.println("Actor::update");
}

void Actor::registerObserver( Actor& observer)
{
  observers.push_back(observer);
}

void Actor::unregisterObserver( Actor& observertoremove)
{
  std::remove_if(observers.begin(), observers.end(), [&observertoremove](Actor& observerfromlist){return (observerfromlist == observertoremove);});
}

  bool Actor::operator==( Actor &other)  {
    return true;
  }
  
