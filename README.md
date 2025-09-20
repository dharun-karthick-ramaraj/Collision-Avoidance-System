# Collision Avoidance System for Vehicles (Arduino UNO)

An embedded system that prevents or reduces the severity of collisions by reading three ultrasonic sensors and dynamically steering/propelling a two‑motor car using a servo for front steering and an H‑bridge for drive control.

> **Demo video:** [YouTube](https://www.youtube.com/watch?v=mm960fzvDU0) · **Drive backup:** [Google Drive](https://drive.google.com/file/d/17zMttCqrMqSAIwWTW7Nm1D2kWgdgxk7h/view?usp=sharing)

---

## ✨ Overview

The car continuously samples **three ultrasonic sensors** (left, front, right). Based on distance readings, it:

* goes **forward** when the path is clear,
* **turns right** or **left** to bypass obstacles,
* or **reverses + U‑turns** when all directions are blocked.

A **servo** controls steering (≈0°/90°/180°). Two DC motors (via an H‑bridge) control forward/reverse.

---

## 🧠 Control Logic (20 cm threshold)

**Decision table**

| Left | Front | Right | Action                                       |
| ---- | ----- | ----- | -------------------------------------------- |
| >20  | >20   | >20   | Go **forward**                               |
| any  | >20   | any   | Go **forward**                               |
| <20  | ≤20   | >20   | **Turn right** then forward                  |
| >20  | ≤20   | <20   | **Turn left** then forward                   |
| >20  | ≤20   | >20   | **Turn right** then forward                  |
| ≤20  | ≤20   | ≤20   | **Stop → reverse → U‑turn**                  |
| any  | ≤20   | any   | **Stop → reverse**, then evaluate left/right |

**Flowchart**

```mermaid
flowchart TD
  A[Start Loop] --> B[Read Front]
  B -->|front > 20cm| C[Forward]
  B -->|front ≤ 20cm| D[Stop -> Reverse]
  D --> E[Read Left & Right]
  E -->|left < 20 & right > 20| F[Steer Right -> Forward]
  E -->|left > 20 & right < 20| G[Steer Left -> Forward]
  E -->|left > 20 & right > 20| H[Prefer Right -> Forward]
  E -->|else (both ≤ 20)| I[Stop -> Reverse -> U-turn]
  C --> A
  F --> A
  G --> A
  H --> A
  I --> A
```

---

## 🔌 Hardware

* **Arduino UNO R3** (ATmega328P)
* **3 × ultrasonic sensors** (front, left, right)
* **Servo (steering)** on pin **D9**
* **Motor driver** (dual‑channel H‑bridge) for 2 DC motors
* **Power**: USB‑B for UNO/servo, **9 V** battery for motor gearbox

> The project used a simulator during development; the physical build follows the same wiring and logic.

---

## 📎 Pinout

| Function            | Pin(s)                         | Notes                         |
| ------------------- | ------------------------------ | ----------------------------- |
| Servo (steering)    | **D9**                         | `Servo` library, center ≈ 90° |
| Front ultrasonic    | **Trig D11**, **Echo D10**     | Distance measurement          |
| Left ultrasonic     | **Trig D13**, **Echo D12**     | Distance measurement          |
| Right ultrasonic    | **Trig A0**, **Echo A1**       | Distance measurement          |
| Motor PWM (ENA/ENB) | **D6**, **D5**                 | `analogWrite` controls speed  |
| Motor direction     | **D8**, **D4**, **D7**, **D3** | H‑bridge direction lines      |

**Distance threshold:** `20 cm`

---

## 🧩 Software

* **Arduino IDE** (or PlatformIO)
* **Libraries:** `Servo.h` (bundled with Arduino IDE)

**Sketch outline (mirrors the implementation):**

```cpp
#include <Servo.h>
Servo steer;
const int THRESH = 20; // cm

// pins per table above...

long readDistanceCM(int trig, int echo) {
  // trigger pulse + pulseIn(...) → return cm
}

void setup() {
  steer.attach(9, 500, 2500);
  // pinMode(...) per mapping
  // Serial.begin(9600);
}

void forward(uint8_t speed=255) {
  // set steer to 90; set motor direction forward; analogWrite speed
}

void stopAll() { /* zero PWMs; all dirs LOW */ }
void reverse(uint8_t speed=127, int ms=1000) { /* reverse motion */ }
void steerRightThenGo() { steer.write(0); /* drive */ }
void steerLeftThenGo()  { steer.write(180); /* drive */ }
void uturn() { /* differential drive to rotate ~180° */ }

void loop() {
  steer.write(90);
  int front = readDistanceCM(11, 10);
  if (front > THRESH) { forward(); return; }

  // blocked: stop + reverse
  stopAll(); delay(2000);
  reverse(127, 2000);

  int left  = readDistanceCM(13, 12);
  int right = readDistanceCM(A0, A1);

  if (left < THRESH && right > THRESH) { steerRightThenGo(); }
  else if (left > THRESH && right < THRESH) { steerLeftThenGo(); }
  else if (left > THRESH && right > THRESH) { steerRightThenGo(); }
  else {
    // fully blocked → stop, reverse, U‑turn
    stopAll(); delay(1000);
    reverse(127, 1000);
    uturn();
  }
}
```

---

## 🛠️ Build & Run

1. **Wire the hardware** per the pinout table.
2. Open the sketch in **Arduino IDE**.
3. Select **Board**: Arduino UNO, correct **Port**.
4. **Upload**.
5. Place the car on a test track with obstacles; power the motors (9 V).
6. Observe behavior across the decision‑table scenarios.

---

## ⚙️ Calibration Tips

* Tune **servo angles** (`0/90/180`) for your steering geometry.
* Adjust **PWM speeds** for smooth turns/U‑turns.
* If ultrasonic reads are noisy, add median filtering or sample multiple times.
* Start with **THRESH = 25–30 cm** for safer braking, then reduce toward **20 cm**.

---

## 📊 Results

* Straight drive when all three sensors are clear.
* Right/left detours when the corresponding flank is open.
* Full block: **stop → reverse → U‑turn**, then re‑evaluate.

Screenshots, measurement notes, and test cases are documented in the final report.


---

## 👥 Team

* Shashwat Chaudhary — 19BCE0056
* Varun Veerbjadra — 19BCB0121
* Dharun Karthick — 19BCE2316
* Ankit Mahto — 19BCE2601
* Priti Sri S — 19BCE2433

---

## 📚 References

* Demonstration video: [https://www.youtube.com/watch?v=mm960fzvDU0](https://www.youtube.com/watch?v=mm960fzvDU0)
* Drive backup: [https://drive.google.com/file/d/17zMttCqrMqSAIwWTW7Nm1D2kWgdgxk7h/view?usp=sharing](https://drive.google.com/file/d/17zMttCqrMqSAIwWTW7Nm1D2kWgdgxk7h/view?usp=sharing)
* Additional literature and diagrams in the project report.
