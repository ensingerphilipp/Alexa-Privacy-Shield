enum state{
  normal,
  boot,
  notification,
  listening,
  mute,
  setup_mode,
  recieving_call,
  active_call,
  do_not_disturb,  //also used for WiFi issues during setup mode
  guard
};

enum led_color{
  off,
  orange,
  yellow,
  purple,
  blue,
  cyan,
  red,
  green,
  white,
  none
};

class LED
{
private:
  char colorValue1,colorValue2,colorValue3;
public:
    void setValue1(char value);
    void setValue2(char value);
    void setValue3(char value);
    char getValue1();
    char getValue2();
    char getValue3();
    void printVals();
    led_color getColor();
};