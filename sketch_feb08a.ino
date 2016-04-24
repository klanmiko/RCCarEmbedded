#define START_BYTE 0x24
#define STEER_FLAG 0x54
#define SPEED_FLAG 0x22
#define END_FLAG 0x65
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
YunClient client;
int x = 0;
byte packet[4];
byte buffera = 0;
boolean reading = false;
char steer, speedval;
void setup()
{
  Serial.begin(9600);
  Bridge.begin();
  Serial.println("starting");
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, INPUT);
  analogWrite(3, 0);
    analogWrite(5, 0);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
}
void loop()
{
  if (!client.connected())
  {
    Serial.println("not connected");
    analogWrite(3, 0);
    analogWrite(5, 0);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    client.connect("192.168.240.142", 6551);
    delay(1000);
  }
  else if (client.connected()) {
    if(checkifdisconnected())
    {
      return;
    }
    while (buffera != START_BYTE)
    {
      if(!client.connected())
      {
        client.stop();
        Serial.println("stopping");
        return;
      }
      buffera = client.read();
    }
    buffera = client.read();
    while (buffera != STEER_FLAG)
    {
       if(!client.connected())
      {
        client.stop();
        Serial.println("stopping");
        return;
      }
      buffera = client.read();
    }
    buffera = client.read();
    packet[0] = buffera;
    buffera = client.read();
    packet[1] = buffera;
    buffera = client.read();
    while (buffera != SPEED_FLAG)
    {
       if(!client.connected())
      {
        client.stop();
        Serial.println("stopping");
        return;
      }
      buffera = client.read();
    }
    buffera = client.read();
    packet[2] = buffera;
    buffera = client.read();
    packet[3] = buffera;
    steer = (char)packet[1];
    speedval = (char)packet[3];
    if (packet[0] == 0x00)
    {
      digitalWrite(2, LOW);
      Serial.println("backward");
    }
    else if (packet[0] == 0x01)
    {
      digitalWrite(2, HIGH);
      Serial.println("foward");
    }
    if(steer==100)
    {
      steer = 99;
    }
    analogWrite(3, ((int)steer) * 255 / 100);
    //analogWrite(3, 200);
    if (packet[2] == 0x00)
      digitalWrite(4, HIGH);
    else if (packet[2] == 0x01)
      digitalWrite(4, LOW);
    if(speedval==100)
    {
      speedval = 99;
    }
    analogWrite(5, ((int)speedval) * 255 / 100);
    client.flush();
  }
}
bool checkifdisconnected()
{
  if(client.peek()==-1)
  {
    Serial.println("stopping");
    client.stop();
    return true;
  }
  return false;
}



