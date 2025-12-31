/////////////////////////////////////////////////////////////////

#pragma once
#ifndef STATE_H
#define STATE_H

/////////////////////////////////////////////////////////////////

#include "Arduino.h"

/////////////////////////////////////////////////////////////////

#ifndef SIMPLEFSM_CALLBACKS_WITH_CONTEXT
#define SIMPLEFSM_CALLBACKS_WITH_CONTEXT 0
#endif

#if SIMPLEFSM_CALLBACKS_WITH_CONTEXT
typedef void (*CallbackFunction)(void *);
typedef bool (*GuardCondition)(void *);
#else
typedef void (*CallbackFunction)();
typedef bool (*GuardCondition)();
#endif

/////////////////////////////////////////////////////////////////

class State {
  friend class SimpleFSM;

 public:
  State();
  State(String name, CallbackFunction on_enter, CallbackFunction on_state = NULL, CallbackFunction on_exit = NULL, bool is_final = false);

  void setup(String name, CallbackFunction on_enter, CallbackFunction on_state = NULL, CallbackFunction on_exit = NULL, bool is_final = false);
  void setName(String name);
  void setOnEnterHandler(CallbackFunction f);
  void setOnStateHandler(CallbackFunction f);
  void setOnExitHandler(CallbackFunction f);
  void setAsFinal(bool final = true);

  int getID() const;
  bool isFinal() const;
  String getName() const;

 protected:
  int id;
  static int _next_id;

  String name = "";
  CallbackFunction on_enter = NULL;
  CallbackFunction on_state = NULL;
  CallbackFunction on_exit = NULL;
  bool is_final = false;
};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
