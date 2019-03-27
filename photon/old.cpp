//
//  main.cpp
//  t6-dropscript
//
//  Created by Nuha on 3/25/19.
//  Copyright © 2019 Nuha. All rights reserved.
//

#include <iostream>

// Nuha Ahmed
// Created 2/27/19

/* Purpose: DAQ script for the Particle Photon
 Collect raw accelerometer data from the ADXL377 sensor using
 analog reads and converting it into common units (g's) and prints the
 result to the Serial Monitor
 */



// Make sure these two variables are correct for your setup
int scale = 200; //200 (±200g) for ADXL377
boolean micro_is_5V = false; // Set to true if using a 5V microcontroller such as the Arduino Uno, false if using a 3.3V microcontroller, this affects the interpretation of the sensor data
float magnitude = 0; // defined variables
float xtermb, xterma;
float ytermb, yterma;
float ztermb, zterma;
float magxyzb, magxyza;
float magdiff, actualmagdiff;

#include "math.h" // to use special math functions
#include "live.h" // server code

void setup()
{
    // call Server.setup() ??????
    
    // Initialize serial communication at 115200 baud
    Serial.begin(115200);
    // call Server.send() ??????
}

// Read, scale, and print accelerometer data
void loop()
{
    /*** RAW ****/
    // Get raw accelerometer data for each axis; "before" data
    int rawXb = analogRead(A0);
    int rawYb = analogRead(A1);
    int rawZb = analogRead(A2);
    // Scale accelerometer ADC readings into common units
    // Scale map depends on if using a 5V or 3.3V microcontroller
    float scaledXb, scaledYb, scaledZb, scaledmagnitudeb; // Scaled values for each axis
    
    delay(1);// Allows a reference data to be obtained
    
    // Get raw accelerometer data for each axis after delay to compare data; "after" data
    int rawXa = analogRead(A0);
    int rawYa = analogRead(A1);
    int rawZa = analogRead(A2);
    // Scale accelerometer ADC readings into common units
    // Scale map depends on if using a 5V or 3.3V microcontroller
    float scaledXa, scaledYa, scaledZa,scaledmagnitudea; // Scaled values for each axis
    
    /*** SCALING ****/
    if (micro_is_5V) // Microcontroller runs off 5V
    {
        scaledXb = map(rawXb, 0, 675, -scale, scale); // 3.3/5 * 1023 =~ 675
        scaledYb = map(rawYb, 0, 675, -scale, scale);
        scaledZb = map(rawZb, 0, 675, -scale, scale);
    }
    else // Microcontroller runs off 3.3V (For this particular experiment)
    {
        scaledXb = map(rawXb, 0, 4095, -scale, scale); // Before Hit Comparsion Data
        scaledYb = map(rawYb, 0, 4095, -scale, scale);
        scaledZb = map(rawZb, 0, 4095, -scale, scale);
        
        // This is to find magnitude Before Hit
        xtermb= (scaledXb*scaledXb);
        ytermb= (scaledYb*scaledYb);
        ztermb= (scaledZb*scaledZb);
        magxyzb= (xtermb+ytermb+ztermb);
        scaledmagnitudeb= sqrt(magxyzb);
        
        scaledXa = map(rawXa, 0, 4095, -scale, scale); // After Hit Comparsion Data
        scaledYa = map(rawYa, 0, 4095, -scale, scale);
        scaledZa = map(rawZa, 0, 4095, -scale, scale);
        
        // This is to find magnitude After Hit
        xterma= (scaledXa*scaledXa);
        yterma= (scaledYa*scaledYa);
        zterma= (scaledZa*scaledZa);
        magxyza= (xterma+yterma+zterma);
        scaledmagnitudea= sqrt(magxyza);
        
    }
    
    // Print out scaled X,Y,Z accelerometer readings Before Hit
    Serial.print("X Vector Before: "); Serial.print(scaledXb); Serial.println(" g");
    Serial.print("Y Vector Before: "); Serial.print(scaledYb); Serial.println(" g");
    Serial.print("Z Vector Before: "); Serial.print(scaledZb); Serial.println(" g");
    Serial.println("*****************************");
    Serial.print("Magnitude Before: "); Serial.print(scaledmagnitudeb); Serial.println(" g"); // Calculate Before Magnitude
    Serial.println("*****************************");
    Serial.println("*****************************");
    
    
    // Print out scaled X,Y,Z accelerometer readings After Hit
    Serial.print("X Vector After: "); Serial.print(scaledXa); Serial.println(" g");
    Serial.print("Y Vector After: "); Serial.print(scaledYa); Serial.println(" g");
    Serial.print("Z Vector After: "); Serial.print(scaledZa); Serial.println(" g");
    Serial.println("*****************************");
    Serial.print("Magnitude After: "); Serial.print(scaledmagnitudea); Serial.println(" g"); // Calculated After Magnitude
    Serial.println("*****************************");
    Serial.println("*****************************");
    
    magdiff=(scaledmagnitudeb-scaledmagnitudea); // This takes difference in two readings
    actualmagdiff=abs(magdiff);
    if ( magdiff < 0 ) // Loop gives absolute of reading, this is final magnitude
    {
        actualmagdiff = -magdiff;
    }
    else
    {
        actualmagdiff = magdiff;
    }
    
    // Prints out Final or Actual Mantiude difference
    Serial.print("Actual Magnitude: "); Serial.print(actualmagdiff, 5); Serial.println(" g"); //calculated magnitude
    Serial.println("*****************************");
    Serial.println("*****************************");
    
    
    Particle.publish("maker_girbil", String(actualmagdiff)); // Publishes data
    
}

