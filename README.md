# Kyo-Project

The fully autonomous air purifier

## Key Features:

* Autonomous Navigation using IR sensors for real-time obstacle avoidance.
* Air Quality Detection using PM2.5 sensor to detect pollution levels.
* Real-Time Feedback via 16x2 LCD display showing AQI values.
* Smart Fan Control that activates when AQI surpasses safe thresholds.
* Mobility supported by dual DC motors and H-bridge motor driver.
* 3D Printed PLA Chassis with modular electronics design.

https://www.behance.net/gallery/209441083/Kyo-Subtle-intelligence

## _Schematic of the electronics circuit_-

<img src="https://github.com/user-attachments/assets/b38d3941-a19e-4444-8f2c-b08928bfd345" width="1000">


<br>
<br>


<table>
  <thead>
    <tr>
      <th>Component</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Battery</td>
      <td>Multi-cell power source</td>
    </tr>
    <tr>
      <td>L298N</td>
      <td>Dual H-Bridge Motor Driver</td>
    </tr>
    <tr>
      <td>PM2.5 Sensor</td>
      <td>Analog air quality sensor (e.g., MQ135)</td>
    </tr>
    <tr>
      <td>IR Sensors</td>
      <td>Used for obstacle detection</td>
    </tr>
    <tr>
      <td>Fan</td>
      <td>Controlled by AQI threshold</td>
    </tr>
    <tr>
      <td>LCD (HY1602E)</td>
      <td>Displays real-time AQI data</td>
    </tr>
    <tr>
      <td>Arduino Board</td>
      <td>Core logic</td>
    </tr>
  </tbody>
</table>

## _Code Breakdown_-

The Arduino sketch does the following:

- *Reads AQI* from analog pin and maps it to a *0–500 AQI scale*.
- *Displays AQI* on LCD using *I2C protocol*.
- *Activates fan* if AQI *exceeds 250*.
- *IR Sensors* on both sides detect obstacles:
  - *No obstacle* → *go forward*
  - *Left blocked* → *turn right*
  - *Right blocked* → *turn left*
  - *Both blocked* → *reverse briefly, then turn*
- *Motor control* is done via *PWM* on *L298N inputs*.


## _Flow of control_-

The robot follows a loop-based control system, executing the following steps:

### **A. Start**
The microcontroller powers on and begins execution from the `setup()` function. This step marks the beginning of the autonomous loop.

### **B. Initialize all components**
- The LCD, fan control pin, IR sensors, PM2.5 sensor (analog input), and motor driver pins are initialized.
- Ensures that all components are set to the correct mode (INPUT/OUTPUT).
- Prepares devices for the main loop, such as setting up I2C communication for the LCD and defining pin modes.

### **C. Read AQI from sensor**
- Reads analog voltage from the PM2.5 sensor (e.g., MQ135).
- This value is mapped to an AQI scale (0–500), making it human-readable.
- The reading frequency is typically every few milliseconds to allow real-time responsiveness.

### **D. Display AQI on LCD**
- The processed AQI value is sent to the LCD.
- Provides real-time visibility of air quality for debugging or user awareness.

### **E. AQI > 250?**
- The system checks whether AQI crosses a dangerous threshold (e.g., 250).
- If yes, the environment is considered polluted enough to activate the air-purifying fan.

### **F. Turn on Fan**
- The digital pin connected to the fan is set HIGH.
- Ensures air purification is active only when needed to save energy.

### **G. Check IR sensors**
- The robot uses two IR sensors for basic obstacle detection.
- The state of each sensor (LOW = clear, HIGH = obstacle) is read.
- Based on the combination of these inputs, movement decisions are made.

### **H. Move Forward**
- If both IR sensors detect no obstacle (clear path), both motors are driven forward.
- This is the default movement behavior during clear navigation.

### **I. Turn Right**
- If the left sensor is blocked but the right is clear, the robot turns right.
- Achieved by stopping the left motor and running the right motor, or vice versa depending on wiring.

### **J. Turn Left**
- If the right sensor is blocked but the left is clear, the robot turns left.
- Helps the robot avoid obstacles smartly without abrupt stops.

### **K. Reverse briefly, then turn**
- If both sensors detect obstacles (e.g., at a dead-end), the robot reverses for a short duration and then randomly chooses a left or right turn.
- This prevents the robot from getting stuck and enables dynamic pathfinding.

After each movement decision, the loop returns to **C**, ensuring continuous sensing, display, and navigation logic.


## _Concept of Kyo_-

Kyo is a fully autonomous mobile air purifier that intelligently monitors and purifies air across different rooms by combining environmental sensing, mobility, and user-centric logic. The system can operate in multiple modes based on user presence and AQI levels.

### **System Overview**

The overall system is divided into three major blocks:

- **Kyo Q (Intelligence)** – the central logic system that controls decision-making and navigation
- **Mobility Module** – allows movement using motor drivers, IR sensors, and a chassis
- **Air Purification Unit** – uses a HEPA filter, PM2.5 sensor, and a high-speed BLDC fan for active purification

---

### **1. Start-Up and Operating Modes**

From the first flowchart:

- **Turn On**: The device powers up either via direct user input or scheduled operation.
- **Two main operational modes**:
  - **Remote Mode**: Keeps Kyo stationed in a single room to constantly monitor and purify air within that space.
  - **Room Tracker Follow Mode**: Enables Kyo to autonomously identify and travel to rooms with the highest AQI levels.

---

### **2. Remote Mode**

- Ideal for localized purification.
- The user manually assigns the device to a room.
- Kyo reads AQI, displays it, and activates the fan when pollution crosses threshold.
- Stays in the room to maintain low AQI.
- Monitors whether the user’s phone stays in the area; if not, it switches to autonomous mode.

---

### **3. Tracker Follow Mode**

- After scanning a smartphone or compatible device, Kyo:
  - **Connects to it** to associate room context.
  - **Notifies** user to close doors/windows.
  - **Follows the Highest AQI Room** using comparative PM2.5 data from multiple rooms (possibly via a network of sensors or moving scan).
- Once a room is clean, it proceeds to the **Next Target**, continuing the purification cycle.
- When all rooms are within safe AQI limits, it **Turns Off**.

---

### **4. Hardware and Internal Wiring**

From the second flowchart:

- **Power Supply Unit (>10,000mAh)** fuels the entire system.
- **Kyo Q** interfaces with the **Main PCB**, which connects to:
  - **Arduino Mega**: The central microcontroller handling logic and sensor inputs.
  - **LCD Module (16×2 with I2C)**: Displays AQI in real time.
  - **Mobility**: Includes dual BO Motors (150 RPM), IR sensors for navigation, and chassis.
  - **Air Purifier Module**: HEPA Filter, PM2.5 Sensor, and a high-speed BLDC Motor.
- All subsystems are controlled via Arduino, which processes sensor data and motor/fan commands accordingly.

---

### **5. Autonomy and Navigation**

- **IR Sensors** detect obstacles during movement.
- Navigation logic:
  - Clear path → Move forward
  - One side blocked → Turn opposite
  - Both blocked → Reverse and reorient
- Kyo continuously evaluates which room has the highest AQI and travels there if not already present.

---

### **Conclusion**

Kyo bridges intelligent sensing, adaptive mobility, and air purification in one unified device. It’s ideal for homes, offices, or closed environments where AQI levels need to be actively and intelligently managed.


<br>

<img src="https://github.com/user-attachments/assets/4c4129c4-715b-4f75-a97f-206e911ad178" width="400">
<img src="https://github.com/user-attachments/assets/40cd9ed8-5279-45a9-8d41-a09e98159bbd" width="400">

<br>
<br>

## _Concept Diagrams_-


<br>

<img src="https://github.com/user-attachments/assets/ae455929-956e-4e3c-8450-eca98c88de69" width="400">
<img src="https://github.com/user-attachments/assets/f154d193-c1ce-4727-ad53-93e6d7c36c41" width="400">

<img src="https://github.com/user-attachments/assets/d2e96d6b-cfd9-41f6-8e42-b25ce174299d" width="400">

<img src="https://github.com/user-attachments/assets/7ab15765-8292-42a6-bb75-18bfaf4667fd" width="400">

<br>
<br>

## _Renders of Kyo_-

<br>

<img src="https://github.com/user-attachments/assets/9c669e1b-c9a5-4264-8c81-1540a2b8e4ef" width="400">
<img src="https://github.com/user-attachments/assets/7422d51d-9265-4682-b277-e8e7b4254ba1" width="400">
<img src="https://github.com/user-attachments/assets/5799f6e3-1673-4e27-b7cf-b4c514cc3a95" width="400">
<img src="https://github.com/user-attachments/assets/c8ebe99d-e922-49fc-a13e-5ba26d88711c" width="400">

<br>
<br>

## _Making Process_-

The creation of Kyo involved simple tools, basic electronics, and a lot of hands-on work. Here's how it all came together:

### **1. Planning & Sketching**
- The idea started with solving a common problem — indoor air pollution.
- Rough sketches helped figure out how the purifier would move, sense air quality, and clean the space.

### **2. 3D Printed Chassis**
- The outer body of Kyo was designed on a computer using 3D modeling software.
- It was printed using PLA plastic, which is strong and easy to work with.
- The design had enough space to fit all the components like motors, fan, sensors, and wires.

### **3. Electronics on Breadboard**
- All the electronics were first built and tested on a breadboard.
- This made it easy to change connections and test different ideas before finalizing anything.
- Key components included:
  - Arduino Mega for control
  - IR sensors for navigation
  - PM2.5 sensor to measure air quality
  - L298N motor driver to run the wheels
  - LCD to display AQI data

### **4. Writing the Code**
- The code was written using Arduino IDE.
- It reads air quality data, controls the fan, displays AQI on the screen, and guides movement based on obstacle detection.

### **5. Assembling Everything**
- After testing, all the components were placed inside the 3D printed body.
- Motors were fixed to the wheels, the LCD screen was mounted on top, and sensors were positioned for accurate detection.

### **6. Testing & Improvement**
- Kyo was tested in different rooms to check how well it moved and cleaned the air.
- The air quality sensor was calibrated, and the movement logic was adjusted to avoid obstacles better.

<br>

<img src="https://github.com/user-attachments/assets/60eb4193-5347-4914-8be2-9397e328fef7" width="400">
<img src="https://github.com/user-attachments/assets/f341908d-75c1-4a68-9d48-abd84df9b30c" width="400">
<img src="https://github.com/user-attachments/assets/66eb6075-4e82-4674-a0b5-0763caad933e" width="400">
<img src="https://github.com/user-attachments/assets/cbba0f7d-2bb3-48a5-b7eb-fc3a9336f55b" width="400">

<br>
<br>

## _Finished Product_- 

Kyo is a fully functional, autonomous air purifier that intelligently navigates indoor spaces while actively monitoring and improving air quality. With its compact 3D printed body, smooth mobility, and real-time AQI display, Kyo combines practical function with a clean, minimal design. The final prototype successfully brings together all its components — from sensors and motors to display and filtration — into a single, cohesive unit.

<br>

<img src="https://github.com/user-attachments/assets/3d0a7b9a-20f6-442d-8b04-c4a4d5682ec8" width="400">
<img src="https://github.com/user-attachments/assets/33510fea-6ce3-416a-9d6f-cea9a5fbae08" width="400">
<img src="https://github.com/user-attachments/assets/1bff3742-f905-409e-a6aa-7ee0f4c5a23b" width="400">
<img src="https://github.com/user-attachments/assets/a1d79dcc-de8a-4984-ab9c-b690f9c55701" width="400">



