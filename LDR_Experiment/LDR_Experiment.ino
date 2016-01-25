/*
 * LDR Experiment
 * Author: Corbin Murrow
 * Date: 25 January 2016
 * Version: 1.2
 * 
 * This tool is meant to be run on the ATmega 328 while connected to PC via 
 * USB cable while running PuTTY to monitor the COM port. This allows for 
 * logging of data output to a file for other processing. Data will be 
 * output in a comma separated format to be logged as a CSV and then parsed
 * with Excel.
 * 
 * ======= VERSION HISTORY =======
 * Version 1.2: Updated code to use new BOEbot library - CM - 25 January 2016
 * Version 1.1: Added standard deviation calculation - CM - 13 October 2015
 * Version 1.0: Initial commint - CM - 12 October 2015
 */

#include <BOEbot.h>
#include <Servo.h>

const int distances = 16;

int leftAvg[distances];
int rightAvg[distances];
int leftStDev[distances];
int rightStDev[distances];

void setup() 
{
  initialize();
  
  Serial.println("Photoresistor Experiment\n");
}

void loop() 
{
  int startDistance = 0;
  int stopDistance = 8*12;
  int stepDistance = 6;

  
  int index = 0;

  for (startDistance; startDistance<stopDistance; startDistance+=stepDistance)
  {
    Serial.print("Move system to ");
    Serial.print(startDistance);
    Serial.println(" inches.");
    wait();

    const int samples = 100;
    int leftValues[samples]; 
    int rightValues[samples];

    for (int i = 0; i < samples; i++)
    {
      leftValues[i] = getLeftLight();
      rightValues[i] = getRightLight();
    }

    leftAvg[index] = average(leftValues, samples);
    rightAvg[index] = average(rightValues, samples);
    leftStDev[index] = standardDeviation(leftAvg[index], leftValues, samples);
    rightStDev[index] = standardDeviation(rightAvg[index], rightValues, samples);
    index++;
  }

  displayandhalt();
}

int standardDeviation(int avg, int* array, int length)
{
  int sumDiffSquared;
  int variance;
  
  for (int i = 0; i < length; i++)
  {
    sumDiffSquared += sq(array[i]-avg);
  }

  variance = (int)((float)sumDiffSquared/(float)length);

  return (int)sqrt(variance);
}

int average(int* array, int length)
{
  float sum = 0.0;

  for (int i = 0; i < length; i++)
  {
    sum += (float)array[i];
  }

  return (int)(sum/(float)length); 
}

void wait()
{
  Serial.println("Press any key to continue...");
  while(Serial.available() == 0) {};
  Serial.read();
  return;
}
void displayandhalt() 
{ 
  Serial.println("Distance,LeftAvg,RightAvg,LeftStDev,RightStDev");

  for (int i = 0; i < distances; i++)
  {
    Serial.print(6*i);
    Serial.print(",");
    Serial.print(leftAvg[i]);
    Serial.print(",");
    Serial.print(rightAvg[i]);
    Serial.print(",");
    Serial.print(leftStDev[i]);
    Serial.print(",");
    Serial.println(rightStDev[i]);
  }

  Serial.end();
  
  while(1); 
}

