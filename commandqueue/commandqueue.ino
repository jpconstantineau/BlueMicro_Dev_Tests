#include "command_queues.h"
#include "hardware_input.h"
#include "trigger.h"
#include <bluemicro_hid.h>
#include "hid_keycodes.h"
commandlist_t commandList; 
commandqueue_t setupQueue;
commandqueue_t commandQueue;
eventqueue_t eventQueue;

Triggers_t triggers;

pinlist_t rows = {14, 15, 16, 17, 18};         
pinlist_t columns = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; 

pinlist_t pins = { 3, 4, 5, 6, 7, 8, 9, 10,15,16,17,18,19,20,21,22}; 

pin_gpio_t speakerpin = 23;
pin_gpio_t ledpin = 25;
pin_gpio_t rgbpin = 28;

//pin_gpio_t ledpin = 13;
//pin_gpio_t pin = 7; 
//pinlist_t pins = {7, 8}; 
bool activeState = false;


HardwareInput test(eventQueue,0);

//Key a_button(eventQueue, 0,0, {pin}, activeState);
//Button b_button(eventQueue, 0, pin, activeState);
Keys matrix1(eventQueue,0,pins,activeState);
//Keypad matrix2(eventQueue,0,rows,columns,activeState);

void setup() {  
  // setup all commands
  SETUPCOMMAND(commandList, COMMANDID(0) , bluemicro_hid.begin());
  SETUPCOMMAND(commandList, COMMANDID(1) , Serial.begin(115200));
  SETUPCOMMAND(commandList, COMMANDID(2) , Serial.println("Starting up"));
  SETUPCOMMAND(commandList, COMMANDID(4) , matrix1.setup());
  SETUPCOMMAND(commandList, COMMANDID(5) , matrix1.scan());

  SETUPCOMMAND(commandList, COMMANDID(6) , bluemicro_hid.processQueues(CONNECTION_MODE_AUTO));


  ADDCOMMAND(setupQueue, COMMANDID(0));
  ADDCOMMAND(setupQueue, COMMANDID(1));
  ADDCOMMAND(setupQueue, COMMANDID(2));
  ADDCOMMAND(setupQueue, COMMANDID(4));
  ADDCOMMAND(commandQueue, COMMANDID(5));
  
  SETUPCOMMAND(commandList, COMMANDID(10), pinMode(ledpin, OUTPUT));
  SETUPCOMMAND(commandList, COMMANDID(11), digitalWrite(ledpin, LOW));
  SETUPCOMMAND(commandList, COMMANDID(12), digitalWrite(ledpin, HIGH));

  ADDCOMMAND(setupQueue, COMMANDID(10));
  
  SETUPCOMMAND(commandList, COMMANDID(20), pinMode(speakerpin, OUTPUT));
  SETUPCOMMAND(commandList, COMMANDID(21), noTone(speakerpin) );
  SETUPCOMMAND(commandList, COMMANDID(22), tone(speakerpin,440));
  ADDCOMMAND(setupQueue, COMMANDID(20));

 
  SETUPCOMMAND(commandList, COMMANDID(90), std::for_each(eventQueue.begin(), eventQueue.end(),[](HardwareEvent event){std::for_each(triggers.begin(), triggers.end(),[event](Trigger trig){trig.detectEvent(event);});}););
  SETUPCOMMAND(commandList, COMMANDID(98), eventQueue.clear());
  SETUPCOMMAND(commandList, COMMANDID(99), delay(10));

  ADDCOMMAND(commandQueue, COMMANDID(90));
  ADDCOMMAND(commandQueue, COMMANDID(98));
  ADDCOMMAND(commandQueue, COMMANDID(6));
  ADDCOMMAND(commandQueue, COMMANDID(99));
  
  SORTCOMMANDS(commandList);

  extended_keycodes_t tmpkey = 0;

    run_event_cb_t  press_run_led = +[ledpin](extended_keycodes_t key){digitalWrite(ledpin, HIGH);};
    run_event_cb_t  release_run_led = +[ledpin](extended_keycodes_t key){digitalWrite(ledpin, LOW);};
  
    triggers.push_back(Trigger(
    HardwareEvent(0,0,HardwareEventType::PRESS),
    typecheck, 
    press_run_led));

    triggers.push_back(Trigger(
    HardwareEvent(0,0,HardwareEventType::RELEASE),
    typecheck, 
    release_run_led)); 


 keymap_t keymap = {
        KC_7,    KC_8,    KC_9,    KC_KP_SLASH,
        KC_4,    KC_5,    KC_6,    KC_KP_ASTERISK,
        KC_1,    KC_2,    KC_3,    KC_KP_MINUS,
        LAYER_1, KC_0,    KC_DOT,  KC_KP_PLUS 
        };

int keyid = 0;
  std::for_each(keymap.cbegin(), keymap.cend(), [&keyid, &bluemicro_hid,ledpin](extended_keycodes_t key){

    run_event_cb_t  press_run = [&bluemicro_hid] (extended_keycodes_t keyf){bluemicro_hid.keyboardPress(keyf);};
    run_event_cb_t  release_run = [&bluemicro_hid](extended_keycodes_t keyf){bluemicro_hid.keyboardRelease();};
    
    triggers.push_back(Trigger(
    HardwareEvent(0,keyid,HardwareEventType::PRESS),
    typeidcheck, press_run ));

    triggers.push_back(Trigger(
    HardwareEvent(0,keyid,HardwareEventType::RELEASE),
    typeidcheck, release_run ));
    keyid++;
    });

  RUNCOMMANDS(setupQueue, commandList); 
}

void loop() {
  RUNCOMMANDS(commandQueue, commandList);  
}
