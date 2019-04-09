// #include <math.h>
const int numSamples = 200;  // More the better, but slower
const int loopDeplay = 5;    // Stability
const int thinkingFreq = 20; // 0 - numSamples (less is more)
const float threshold = 100.0;
const int pinAudio = A5;
const int pinRelay = 2;

int samples[numSamples] = {0};
int samplesIndex = 0;
long total = 0;
float average = 0.0;
bool recentlyTriggeredRelay = true;
int recentlyTriggeredRelayHowManySamplesAgo = numSamples * 2;
int pinRelayState = 0;

void setup()
{
  // Serial.begin(115200);
  pinMode(pinAudio, INPUT);
  pinMode(pinRelay, OUTPUT);
  // Empty samples array
  for (int i = 0; i < numSamples; i++)
  {
    samples[i] = 0;
  }
  delay(500);
}

void loop()
{
  int audio = analogRead(pinAudio);

  // Add audio reading to samples array
  samples[samplesIndex++] = audio; // 0 - 1024

  if (samplesIndex >= numSamples)
  {
    samplesIndex = 0;
  }

  // Lol. Yeah, make super sure we've had a new full set of values because
  // relay triggering really fucks with the analog reading
  if (recentlyTriggeredRelayHowManySamplesAgo > 0)
  {
    recentlyTriggeredRelayHowManySamplesAgo--;
  }
  else
  {
    recentlyTriggeredRelay = false;
  }

  // showArray(samples, numSamples);

  // Debugging block
  // Serial.print(audio);
  // for (int i = 0; i < (5 - ((String)audio).length()); i++)
  // {
  //   // for (int i = 0; i < (5 - (floor (log10 (abs (audio)))) + 1); i++) {
  //   Serial.print(" ");
  // }
  // Serial.print(standardDeviation(samples, numSamples));
  // // Serial.print("  ");
  // // Serial.print(total);
  // // Serial.print("  ");
  // // Serial.print(samplesIndex);
  // Serial.print("  ");
  // Serial.print(mean(samples, numSamples));
  // Serial.print("\n");

  // Occasionally, figure out if audio is playing
  if (!recentlyTriggeredRelay && samplesIndex >= thinkingFreq && samplesIndex % thinkingFreq == 0)
  {
    // The amount by which the audio signal is varying
    // If it's varying lots then there's a signal, if barely at all.. s'dead
    // int pinRelayStateNew = (standardDeviation(samples, numSamples) > threshold && !everyValueIsTiny(samples, numSamples)) ? 1 : 0;
    int pinRelayStateNew = (mean(samples, numSamples) > threshold) ? 1 : 0;
    if (pinRelayStateNew != pinRelayState)
    {
      pinRelayState = pinRelayStateNew;
      digitalWrite(pinRelay, pinRelayState);
      recentlyTriggeredRelay = true;
      recentlyTriggeredRelayHowManySamplesAgo = numSamples * 2;
    }
  }

  delay(loopDeplay);
}

float everyValueIsTiny(int data[], int dataLength)
{
  bool tiny = true;
  for (int i = 0; i < dataLength; ++i)
  {
    if (data[i] >= 10)
    {
      tiny = false;
    }
  }
  return tiny;
}

float mean(int data[], int dataLength)
{
  long total = 0;
  for (int i = 0; i < dataLength; ++i)
  {
    total += data[i];
  }
  return total / dataLength;
}

float standardDeviation(int data[], int dataLength)
{
  float sum = 0.0, mean, standardDeviation = 0.0;
  for (int i = 0; i < dataLength; ++i)
  {
    sum += data[i];
  }
  mean = sum / dataLength;
  for (int i = 0; i < dataLength; ++i)
  {
    standardDeviation += pow(data[i] - mean, 2);
  }
  return sqrt(standardDeviation / dataLength);
}

void showArray(int data[], int dataLength)
{
  for (int i = 0; i < dataLength; ++i)
  {
    Serial.print(data[i]);
    Serial.print(' ');
  }
  Serial.println(' ');
}
