#ifndef KEY_PRESSED_EVENT_H
#define KEY_PRESSED_EVENT_H

#include "../event_bus/event.h"
#include <SDL.h>

class KeyPressedEvent : public Event
{
public:
    SDL_Keycode key_pressed;

    // Construtor com lista de inicialização
    KeyPressedEvent(SDL_Keycode key_pressed) : key_pressed(key_pressed) {}
};
#endif
