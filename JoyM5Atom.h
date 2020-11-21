#ifndef joy_m5atom_h
#define joy_m5atom_h

#include <WiFi.h>
#include <WiFiAP.h>

#define JOYM5ATOM_STATE_INITIAL       0
#define JOYM5ATOM_STATE_CONNECTING    1
#define JOYM5ATOM_STATE_LISTEN        2
#define JOYM5ATOM_STATE_CONNECTED     3
#define JOYM5ATOM_STATE_CLOSED        4

class JoyM5Atom
{
  public:
    JoyM5Atom();
    void begin();
    void update();
    bool is_right();
    bool is_left();
    bool is_up();
    bool is_down();
    bool is_connected() { return _state == JOYM5ATOM_STATE_CONNECTED; }
    bool is_channged() { return _changed; }

  private:
    WiFiServer _server;
    WiFiClient _client;
    int _state;
    bool _changed;
    char _data;

    void set_state(int state);
    void state_initial();
    void state_connecting();
    void state_listen();
    void state_connected();
    void state_closed();
    bool check_connection();
    void terminate();

};

#endif
