const int pinRelay = 2;
bool pinRelayState = false;

void setup()
{
  pinMode(pinRelay, OUTPUT);
  delay(500);
}

void loop()
{
  digitalWrite(pinRelay, pinRelayState);
  delay(3000);
  pinRelayState = !pinRelayState;
}