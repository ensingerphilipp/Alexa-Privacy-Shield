enum assistant_state{
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