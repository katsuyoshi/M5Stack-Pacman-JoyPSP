#include "JoyM5Atom.h"
#include <sstream>

#define SSID      "Pac-Man"
#define PASSWORD  "JOYM5ATOM"
#define PORT_NO 8888


JoyM5Atom::JoyM5Atom()
{
  _state = JOYM5ATOM_STATE_INITIAL;
  _changed = false;
  _data = '5';
}

bool JoyM5Atom::is_right()
{
  if (is_connected() == false) return false;

  switch(_data) {
  case '9':
  case '6':
  case '3':
    return true;
  default:
    return false;
  }
}

bool JoyM5Atom::is_left()
{
  if (is_connected() == false) return false;

  switch(_data) {
  case '7':
  case '4':
  case '1':
    return true;
  default:
    return false;
  }
}

bool JoyM5Atom::is_up()
{
  if (is_connected() == false) return false;

  switch(_data) {
  case '7':
  case '8':
  case '9':
    return true;
  default:
    return false;
  }
}

bool JoyM5Atom::is_down()
{
  if (is_connected() == false) return false;

  switch(_data) {
  case '1':
  case '2':
  case '3':
    return true;
  default:
    return false;
  }
}


void JoyM5Atom::begin()
{
  terminate();
  Serial.print("WiFi begin");
}

void JoyM5Atom::update()
{
  _changed = false;
  switch (_state)
  {
  case JOYM5ATOM_STATE_INITIAL:
    state_initial();
    break;
  case JOYM5ATOM_STATE_CONNECTING:
    state_connecting();
    break;
  case JOYM5ATOM_STATE_LISTEN:
    state_listen();
    break;
  case JOYM5ATOM_STATE_CONNECTED:
    state_connected();
    break;
  case JOYM5ATOM_STATE_CLOSED:
    state_closed();
    break;
  }
}

void JoyM5Atom::set_state(int state)
{
  _state = state;
}

void JoyM5Atom::state_initial()
{
  WiFi.softAP(SSID, PASSWORD);
  set_state(JOYM5ATOM_STATE_CONNECTING);
}

void JoyM5Atom::state_connecting()
{
  Serial.println("WiFi connected");
  if (_server == false)
  {
    _server = WiFiServer(PORT_NO, 1);
    _server.begin();
  }
  set_state(JOYM5ATOM_STATE_LISTEN);
  Serial.println(WiFi.softAPIP());
}

void JoyM5Atom::state_listen()
{
  _client = _server.available();
  if (_client)
  {
    set_state(JOYM5ATOM_STATE_CONNECTED);
    Serial.println("WiFi listening");
  }
}

void JoyM5Atom::state_connected()
{
  if (_client.connected())
  {
    if (_client.available())
    {
      _data = _client.read();
      Serial.print(String(_data).c_str());
      _changed = true;
    }
  }
  else
  {
    set_state(JOYM5ATOM_STATE_CLOSED);
  }
}

void JoyM5Atom::state_closed()
{
  terminate();
  set_state(JOYM5ATOM_STATE_CONNECTING);
  Serial.println("WiFi closed");
}

void JoyM5Atom::terminate()
{
  set_state(JOYM5ATOM_STATE_INITIAL);
  if (_client)
  {
    _client.stop();
    _client.~WiFiClient();
  }
  if (_server)
  {
    _server.stopAll();
    _server.~WiFiServer();
  }
  WiFi.disconnect();
}
