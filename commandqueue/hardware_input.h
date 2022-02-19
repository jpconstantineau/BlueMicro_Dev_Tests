#include <stdint.h>
#include <Arduino.h>
#include "event.h"

typedef uint32_t pin_gpio_t;
typedef std::vector < pin_gpio_t > pinlist_t;
typedef std::vector < bool > keystates_t;

class HardwareInput {
    public:
      HardwareInput(eventqueue_t& queue, HardwareEventSource_t hardwareid=0)
      :  _eventqueue{ queue }, _hardwareid{ hardwareid } {};
   
      virtual void scan(void) {};
      virtual void setup(void) {};

    protected:
      eventqueue_t& _eventqueue;
      HardwareEventSource_t _hardwareid;
};


class Key : public  HardwareInput {
    public:

        Key(eventqueue_t& queue, HardwareEventSource_t hardwareid=0, uint32_t keyid=0, uint32_t ulPin =0 , bool activeState =0) 
            : HardwareInput(queue,hardwareid),
              _KeyID{ keyid }, _Pin{ ulPin }, _activeState{ activeState } {};

        void scan(void);
        void setup(void);

    protected:
      uint32_t  _KeyID;
      uint32_t  _Pin;
      bool      _activeState;
      bool      _btn_state;
      bool      _previous_btn_state;
};


class Button : public  Key {
    public:
        Button(eventqueue_t& queue, HardwareEventSource_t hardwareid =0,  uint32_t ulPin =0, bool activeState =0) 
            : Key(queue,hardwareid,0,ulPin,activeState) {};
};

typedef std::vector<Key> row_of_keys_t;

class Row : public  HardwareInput {
    public:
        Row(eventqueue_t& queue, HardwareEventSource_t hardwareid, uint32_t rowid, pinlist_t pinlist, bool activeState) 
            : HardwareInput(queue,hardwareid),
              _RowID{ rowid }, _Pins{ pinlist }, _activeState{ activeState } {
                int j = 0;
                int s = _Pins.size();
                std::for_each(_Pins.begin(), _Pins.end(), [this, &j, s](pin_gpio_t thispin) {
                      _row_keys.push_back(Key(_eventqueue,_hardwareid,_RowID*s+j, thispin ,_activeState));
                       j++;
                    });
                };

        void scan(void);
        void setup(void);
    protected:
      uint32_t  _RowID;
      pinlist_t  _Pins;
      bool      _activeState;
      row_of_keys_t _row_keys;
}; 

class Keys : public  Row {
  public:
  Keys(eventqueue_t& queue, HardwareEventSource_t hardwareid, pinlist_t pinlist, bool activeState) 
            : Row(queue,hardwareid,0,pinlist,activeState) {};
};

typedef std::vector<Row> group_of_rows_t;

class Keypad : public HardwareInput {
      public:
        Keypad(eventqueue_t& queue, HardwareEventSource_t hardwareid, pinlist_t rowpinlist, pinlist_t columnpinlist, bool activeState) 
            : HardwareInput(queue,hardwareid),
              _RowPins{ rowpinlist }, _ColumnPins{ columnpinlist }, _activeState{ activeState } {
                int rowid = 0;
                std::for_each(_RowPins.cbegin(), _RowPins.cend(), [this, &rowid](pin_gpio_t thisrowpin) {
                      _rows.emplace_back(_eventqueue,_hardwareid,rowid,_ColumnPins ,_activeState);
                       rowid++;
                    });
                };

        void scan(void);
        void setup(void);
    protected:
      pinlist_t  _RowPins;
      pinlist_t  _ColumnPins;
      bool      _activeState;
      group_of_rows_t _rows;     
};
