#include <stdint.h>
#include <vector>
#include <utility>
#include <algorithm>

class Actor;

typedef std::vector<Actor> ActorList_t;
typedef uint16_t message_t; 

class Actor {
  public:

  message_t getstate();
  void notify(void);
  void update(void);
  void registerObserver( Actor& observer);
  void unregisterObserver( Actor& observer);
bool  operator==( Actor &other); 
  protected:

    ActorList_t observers;
    //ActorList_t childs;
    
    
};
