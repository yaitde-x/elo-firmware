#include "global.h"

#ifdef ELO_HX711

#include "hx711_hal.h"

#define DOUT D3
#define CLK D2

HX711 scale(DOUT, CLK);

//float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup
double lastReading = -1000.0; // force it to send a reading change the first time
CommsProvider *hx711_comms;
StateProvider *hx711_stateProvider;

void hx711_setup(StateProvider *s, CommsProvider *commsProvider)
{
  hx711_comms = commsProvider;
  hx711_stateProvider = s;

  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  //long zero_factor = scale.read_average(); //Get a baseline reading
  //Serial.print("Zero factor: ");           //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  //Serial.println(zero_factor);

  //hx711_stateProvider->setCal1(-7050);

  hx711_loop();
}

void hx711_loop()
{
  
  hx711_stateProvider->setCal1(-7050);

  if (hx711_stateProvider->getVar5() != 0)
  {
    hx711_stateProvider->setVar5(0);
    scale.tare();
  }
  else
  {

    float cf = hx711_stateProvider->getCal1();

    scale.set_scale(cf); //Adjust to this calibration factor

    double currentReading = scale.get_units();

    if (scaleReadingHasChanged(currentReading, lastReading))
    {

      Serial.print("Transmit reading: ");
      Serial.print(currentReading, 1);
      Serial.print(" lbs"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
      Serial.print(" calibration_factor: ");
      Serial.print(cf);
      Serial.println();

      lastReading = currentReading;
      hx711_stateProvider->setVar1((int)(currentReading * 100));
      hx711_comms->broadcastState();
    }
  }
}

int scaleReadingHasChanged(double currentReading, double lastReading)
{
  return (currentReading <= (lastReading - .1) || currentReading >= (lastReading + .1));
}

#endif