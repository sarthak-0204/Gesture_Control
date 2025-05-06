## **Gesture-Controlled Robotic Car using ESP32 and Hand Tracking**


## **Project Overview**

This project demonstrates a robotic vehicle controlled by human hand gestures using a webcam and a Python program that recognizes gestures through **MediaPipe** and **OpenCV**. The commands are sent wirelessly to an **ESP32 microcontroller** via HTTP, which then drives the motors accordingly. It simulates a contactless interface for controlling mobility aids or small robotic platforms.

---

## **How It Works**

1. **Hand Gesture Detection**:

   * A Python script captures real-time video using OpenCV.
   * Google’s MediaPipe library detects hand landmarks (fingers, thumb, etc.).
   * Specific gestures are recognized:

     * **Index Finger** → Move Forward
     * **Middle Finger** → Move Backward
     * **Fist/Open Palm/No Detection** → Stop
     * **Thumbs Up** → Right turn
     * **Index + Pinky finger** → Left turn

2. **Command Transmission**:

   * Based on the recognized gesture, the Python script sends an HTTP GET request to the ESP32’s IP address (e.g., `http://192.168.4.1/forward`).

3. **ESP32 Wi-Fi Mode**:

   * ESP32 is configured as a Wi-Fi Access Point.
   * It listens for HTTP commands and drives the motors using PWM control.

4. **Motor Control**:

   * Motor direction pins (IN1–IN4) and speed control via PWM on ENA/ENB.
   * Turning is achieved by running motors in opposite or differential speeds.

---

## **Hardware Components**

* ESP32 DevKit V1
* L298N Dual H-Bridge Motor Driver
* Four DC Motors
* Power Supply (Battery/USB)
* Jumper Wires
* Robot chassis (or wheelchair frame)
* Webcam (for gesture detection)

---

## **Software Used**

* **Arduino IDE** (for ESP32 firmware)
* **Python 3.x**
* **OpenCV** (for camera processing)
* **MediaPipe** (for hand tracking)
* **Requests** (to send HTTP requests)

---

## **Use Cases**

1. **Assistive Technology**

   * Enables physically impaired individuals to control a wheelchair using gestures.

2. **Contactless Control in Medical or Industrial Settings**

   * Operate robots in sterile or hazardous environments without physical contact.

3. **Smart Home Integration**

   * Control home appliances or mobility aids using hand gestures.

4. **Education and Research**

   * Demonstrates core concepts in computer vision, embedded systems, and robotics.

5. **DIY Robotics Projects**

   * Useful for hobbyists building advanced gesture-controlled robots.

---

## **Advantages**

* Low-cost solution using open-source tools
* Contactless and intuitive interaction
* Modular design – can be upgraded with speech, obstacle detection, etc.
* Portable and customizable for various applications


