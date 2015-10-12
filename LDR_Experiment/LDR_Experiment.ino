/*
 * LDR Experiment
 * Author: Corbin Murrow
 * Date: 12 October 2015
 * Version: 1.0
 * 
 * This tool is meant to be run on the ATmega 328 while connected via the COM port 
 * with PuTTY. This allows for logging of data output for other processing. Data
 * will be supplied in a comma separated format to be logged as a CSV and then parsed
 * with Excel.
 * 
 * ======= VERSION HISTORY =======
 * Version 1.0: Initial commint - CM - 12 October 2015
 */

#define leftLDR 5
#define rightLDR 3

const int distances = 16;

int left[distances];
int right[distances];

void setup() 
{
  Serial.begin(115200);
  Serial.println("Photoresistor Experiment\n");

  pinMode(leftLDR, INPUT);
  pinMode(rightLDR, INPUT);
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
    int leftvalues[samples]; 
    int rightvalues[samples];

    for (int i = 0; i < samples; i++)
    {
      leftvalues[i] = analogRead(leftLDR);
      rightvalues[i] = analogRead(rightLDR);
    }

    left[index] = average(leftvalues, samples);
    right[index] = average(rightvalues, samples);
    index++;
  }

  displayandhalt();
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
  Serial.println("Distance,Left,Right");

  for (int i = 0; i < distances; i++)
  {
    Serial.print(6*i);
    Serial.print(",");
    Serial.print(left[i]);
    Serial.print(",");
    Serial.println(right[i]);
  }

  Serial.end();
  
  while(1); 
}

