#ifndef STATEMACHINE_H
#define STATEMACHINE_H 

#define StateMachineChoice void*
#define Choice(a) reinterpret_cast<void*>(a)
#define CallChoice(a) reinterpret_cast<__choice>(a)()
typedef void* (*__choice)();

#endif
