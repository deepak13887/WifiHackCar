# WifiHackCar
This repository contains simple client side node js application which runs on laptop and RC car program which runs on the car.
The idea is to control the RC car from client app running on laptop over a WiFi network.
The Wifi accesspoint is setup on the RC car using the NodeMCU's wifi module and running it in server mode.
The laptop is then made to connect to the RC car's wifi network.
The node js application running on the laptop is then able to issue commands to the car over this Wifi network.

Thanks to the following link which shows step by step assembly of the RC car using nodemcu:
https://srituhobby.com/how-to-make-a-wifi-controlled-car-using-nodemcu-and-blynk-app-step-by-step-instructions/