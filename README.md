# MECH1010 Drone Controller Project

This project was developed as part of the **MECH1010 Mechatronics & Programming** module during my first year at the University of Leeds. The goal was to design and program a simple control system for a simulated helicopter (or drone arm) using an Arduino Uno.

## Project Summary

The system uses sensor feedback (e.g., potentiometer and angle sensor) to adjust motor speed via a proportional controller. It aims to bring the drone arm to a target angle and shut down safely once within tolerance.

### Objectives:
- Read angle data from sensors
- Implement a control algorithm to reach a target position
- Display system status via LEDs
- Log and output telemetry data

## Files

- `drone_controller.ino` — The full Arduino control code
- `flight_data.csv` — Logged sensor data from a test run
- `flight_demo.mp4` — Footage of the test in action

## Results

During the test, the system successfully brought the drone arm within the target range and shut down safely. Logged data shows stable convergence and minimal overshoot.

![Demo Screenshot](screenshot.png) <!-- optional if you want to add an image frame from video -->

## Tech Stack

- Arduino Uno
- Potentiometer & angle sensor
- MATLAB (for analysis)
- CSV logging

## Author

**William Cook [@willcook415]**  
Mechanical Engineering Student  
University of Leeds

---

> This project demonstrates core skills in embedded systems, control theory, and data analysis — built from scratch as a first-year engineering project.
