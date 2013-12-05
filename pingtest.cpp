/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example RF Radio Ping Pair
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two different nodes,
 * connect the role_pin to ground on one.  The ping node sends the current time to the pong node,
 * which responds by sending the value back.  The ping node can then see how long the whole cycle
 * took.
 */
#include <cstdlib>
#include <iostream>
#include "Mirf.h"
//#include "Mirf.cpp"

//#include "../RF24.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

//RF24 radio(9,10);
//Nrf24l radio("/dev/spidev0.0",8000000);  //spi device, speed and CSN,only CSN is NEEDED in RPI


// sets the role of this unit in hardware.  Connect to GND to be the 'pong' receiver
// Leave open to be the 'ping' transmitter
const int role_pin = 7;

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
//const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
//0x636C696531LL=="clie1", 0x7365727631LL=="serv1"
//uint8_t pipes[2] = {1,2};
//const uint64_t pipes[2] = {0x1,0x2};
void delay(int ms) {
  usleep(ms*1000); //convert to microseconds
  return;
}

void setup(void)
{
  Mirf = Nrf24l("/dev/spidev0.0",500000);
  Mirf.cePin=25;
  Mirf.csnPin=8;
  Mirf.init();
  Mirf.setRADDR((uint8_t *)"serv1");
  Mirf.payload = sizeof(float);
  Mirf.config();
  printf("%d",Mirf.read_register(RF_DR));
  printf("setup done.\n\r");
}

void loop(void)
{
  float* temp;
  uint8_t data[Mirf.payload];
  unsigned long time;
  unsigned long count;
  //printf("Setting Tx address.\n\r");
  Mirf.setTADDR((uint8_t*)"clie1");
  printf("Sending request...\n\r");
  Mirf.send(data);
  delay(50);
  //printf("Waiting for send to finish...\n\r");
  while(Mirf.isSending()){
  }
  time = __millis();
  count = 0;
  //printf("Waiting for response...\n\r");
  while(!Mirf.dataReady())
  {
    //printf("%d.",Mirf.getStatus());
    count++;
    if(count%50==0) printf(".");
    delay(10);
    if(__millis()-time>2000)
    {
      printf("Time out, skip to next cycle...\n\r");
      return;
    }
  }
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData(data);
    temp = (float*) data;
    printf("%.2f",*temp);
    printf("F at ");
    printf("%u",time/1000);
    printf(". Ping: ");
    printf("%u",__millis()-time);
    printf("ms.\n\r");
    //delay(50000);
    delay(1000);
  }
}

int main(int argc, char** argv)
{
  setup();
  while(1)
    loop();
  return 0;
}


// vim:cin:ai:sts=2 sw=2 ft=cpp
