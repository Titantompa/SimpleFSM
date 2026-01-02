/////////////////////////////////////////////////////////////////
/*
    This emulates four blinken lights using four instances of 
    the same state state machine.
    Each light has it's own timer, making it turn on and off
    every 1 to 4 seconds, respectively.
    While the light is "on", the corresponding light's number 
    is printed to Serial.
*/

/////////////////////////////////////////////////////////////////

#define SIMPLEFSM_CALLBACKS_WITH_CONTEXT 1

#include "SimpleFSM.h"

#undef SIMPLEFSM_CALLBACKS_WITH_CONTEXT

/////////////////////////////////////////////////////////////////

class ContextualSimpleFSM : public SimpleFSM
{
public:
  int light_id;
};

/////////////////////////////////////////////////////////////////

ContextualSimpleFSM fsm[4] = { ContextualSimpleFSM(), ContextualSimpleFSM(), ContextualSimpleFSM(), ContextualSimpleFSM() };

/////////////////////////////////////////////////////////////////

void light_on(struct ContextualSimpleFSM* ctx) {
  Serial.printf("Light %d: Entering State: ON\n", ctx->light_id);
}
 
void light_off(struct ContextualSimpleFSM* ctx) {
  Serial.printf("Light %d: Entering State: OFF\n", ctx->light_id);
}

void exit_light_on(struct ContextualSimpleFSM* ctx) {
  Serial.printf("\nLight %d: Leaving State: ON ", ctx->light_id);
}
 
void exit_light_off(struct ContextualSimpleFSM* ctx) {
  Serial.printf("\nLight %d: Leaving State: OFF", ctx->light_id);
}

void on_to_off(struct ContextualSimpleFSM* ctx) {
  Serial.printf("Light %d: ON -> OFF\n", ctx->light_id);
}

void off_to_on(struct ContextualSimpleFSM* ctx) {
  Serial.printf("Light %d: OFF -> ON\n", ctx->light_id);  
}

void ongoing(struct ContextualSimpleFSM* ctx) {
  Serial.printf("%d", ctx->light_id);  
}

/////////////////////////////////////////////////////////////////

State s[] = {
  State("on",  (CallbackFunction) light_on, (CallbackFunction) ongoing, (CallbackFunction) exit_light_on),
  State("off",  (CallbackFunction) light_off, nullptr, (CallbackFunction) exit_light_off)
};

enum triggers {
  light_switch_flipped = 1  
};

Transition transitions[] = {
  Transition(&s[0], &s[1], light_switch_flipped, (CallbackFunction) on_to_off),
  Transition(&s[1], &s[0], light_switch_flipped, (CallbackFunction) off_to_on)
};

int num_transitions = sizeof(transitions) / sizeof(Transition);

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(300);
  }
  Serial.println();
  Serial.println();
  Serial.println("SimpleFSM - Contextual (Light Switches)\n");
    
  for(int i = 0; i< 4; i++)
  {
    fsm[i].add(transitions, num_transitions);
    fsm[i].setInitialState(&s[1]);
    fsm[i].light_id = i + 1;
  }
}

/////////////////////////////////////////////////////////////////

void loop()
{
  for (int i = 0; i < 4; i++)
  {
    fsm[i].run();
    // flip the switch every 1-4 seconds depending on light id
    if (fsm[i].lastTransitioned() > (fsm[i].light_id * 1000))
    {
      fsm[i].trigger(light_switch_flipped);
    }
  }
}
/////////////////////////////////////////////////////////////////