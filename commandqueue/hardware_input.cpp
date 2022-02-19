#include "hardware_input.h"

void Key::scan(void)
{
  //Serial.println("Calling Key::scan");
    _previous_btn_state = _btn_state;
    _btn_state = (digitalRead(_Pin) == _activeState);
    if (_btn_state != _previous_btn_state)
    {
      //Serial.println("state!=last");
      //Serial.println(_eventqueue.size());
      if (_btn_state)
      {
        //Serial.println("PRESS");
        _eventqueue.push_back(HardwareEventDigital(_hardwareid,_KeyID,HardwareEventType::PRESS));
       // Serial.println(_eventqueue.size());
      }
      else
      {
        //Serial.println("RELEASE");
        _eventqueue.push_back(HardwareEventDigital(_hardwareid,_KeyID,HardwareEventType::RELEASE));
        //Serial.println(_eventqueue.size());
      }
    }
}

void Key::setup(void)
{
  //Serial.print("Setup Pin: ");
  //Serial.println(_Pin);
  pinMode(_Pin, _activeState ? INPUT_PULLDOWN : INPUT_PULLUP);
}

void Row::scan(void)
{

    std::for_each(_row_keys.begin(), _row_keys.end(), [](Key &thiskey) { thiskey.scan();});

}

void Row::setup(void)
{

    std::for_each(_row_keys.begin(), _row_keys.end(), [](Key &thiskey) { thiskey.setup();});

}

void Keypad::scan(void)
{
  int rowid = 0;
    std::for_each(_rows.begin(), _rows.end(), [this, &rowid](Row &thisrow) { 
      pinMode(_RowPins[rowid], OUTPUT ); // flip direction
      digitalWrite(_RowPins[rowid], _activeState ? HIGH : LOW);
      thisrow.scan();
      pinMode(_RowPins[rowid], _activeState ? INPUT_PULLDOWN : INPUT_PULLUP); // same orientation 
      rowid++;
      });
}

void Keypad::setup(void)
{
    int rowid = 0;
    std::for_each(_rows.begin(), _rows.end(), [this, &rowid](Row &thisrow) { 
      thisrow.setup();
      pinMode(_RowPins[rowid], _activeState ? INPUT_PULLDOWN : INPUT_PULLUP); // same orientation 
      rowid++;
      });
}
