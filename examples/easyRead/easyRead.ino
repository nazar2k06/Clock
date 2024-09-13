#include <Clock.h>

#define MY_UNIX 100000
#define MY_GMT 0
Clock clk;

void setup() {
  TimeT time;
  clk.printDate(MY_GMT);
  delay(2000);
  clk.printDate(MY_GMT);
  
  Serial.println("\nUnix to zero");
  clk.setUnix(0);
  time = clk.getTime(MY_GMT);
  
  clk.printDate(MY_GMT);
  delay(2000);
  clk.printDate(time);

  Serial.println(String("\nUnix to ") + MY_UNIX);
  clk.setUnix(MY_UNIX);
}

void loop() {
  Serial.println(String("\nStatus:") + clk.status());
  Serial.println(String("Unix:") + clk.getUnix());
  Serial.println(String(clk.hour(MY_GMT)) + ":" + clk.minute(MY_GMT) + ":" + clk.second(MY_GMT));
  
  delay(2000);
}