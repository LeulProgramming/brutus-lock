# BRUTUS

## What is Brutus?
Brutus is a way to ensure that one's property is safe and sound. It is a smart bike lock attachment that allows bikers to **elevate their confidence in the safety** of their mode of transportation with the ability to remotely monitor and secure their bikes.

When someone is messing with the bike, the vibration sensor detects movement, and the bike owner is alerted via push notifications and can then monitor their bike from a live video feed. This way, the owner can easily identify/describe a intruder if anything off the bike is stolen.

One of hour biggest challenges was aquiring working hardware as all of the cellular ESP32s we ordered were malfunctioning. Therefore, we opted to build the product that connects to the app via Wifi. Future versions will deal with challenges faced in version one.

## About the Brutus Lock

Features:
* Vibration detection
  * Once vibration is detected for 3 seconds, the bike owner is a alerted that someone is trying to steal their bike.
* Live feed
* Able to attach to majority of bike locks
* Wifi connectivity
* Weather proof
* App to control lock

Technology we used:
* Software
  * Arduino      - to take in inputs from vibration sensors
  * React Native - for the mobile application
  * Firebase     - to store images/videos from the camera and vibration data and store login information
* Hardware
  * ESP32
  * OV7670 Camera
  * Hiletgo Vibration Sensor
 
* Future Goals
  * Add cellular capabilites
  * Add GPS capabilites
  * Create a new version of the product that is more general or used for something else (i.e. door locks, bags, etc.)
  * Other features


## Team Members
* Tech Team
  * Leul Dagnachew
  * Zoya Farooqui
  * Nafee Karim
  * Jimmy Liang
  * Amogh Maroju
* Product Team
  * Reeba Abraham
  * Kenneth Chen
  * Richard Yang
* Design Team
  * Britney Lam
