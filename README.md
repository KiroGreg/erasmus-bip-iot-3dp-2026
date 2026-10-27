# Erasmus+ Blended Intensive Programme (BIP): Internet of Things and 3D Printing

**University of Novi Sad (UNS) | Faculty of Sciences**  
**Program Dates:** May 18 - 22, 2026  
**Author:** Kyryll Hryhorenko  

---

## Program Overview

This repository contains coursework, design reports, and practical assignments developed as part of the third edition of the Erasmus+ Blended Intensive Programme (BIP) titled **"Internet of Things and 3D Printing"** (EUGLOH 2.0 & Friends), hosted by the University of Novi Sad. 

The program followed a blended format consisting of five weeks of online collaborative coursework followed by a one-week intensive onsite program in Novi Sad. It brought together students from across European partner institutions (including EUGLOH universities such as Alcala, Szeged, Porto, Hamburg, and Paris-Saclay, as well as partner institutions like Maribor and Alba Iulia) alongside UNS students. Activities included lectures, hands-on workshops, case studies, and industrial site visits (such as Schneider Electric LLC).

---

## Repository Structure

```
.
├── iot-arduino/              # C++ and Arduino embedded systems assignments
│   ├── Exercise1_SensorReading.cpp
│   ├── Exercise2_SensorReadingAdvanced.cpp
│   ├── MovingAverageTracker.cpp
│   └── README.md
├── 3d-printing-cad/          # CAD designs, slicing models, and engineering reports
│   ├── CR2032.f3d
│   ├── CR2032_Report.md
│   ├── holder.3mf
│   ├── lid.3mf
│   └── README.md
└── ergonomics-lighting/      # Ergonomics and workspace lighting analysis
    ├── DesignAssignment1_LightAndShade.md
    ├── Kyryll1.png
    ├── Kyryll2.png
    └── README.md
```

---

## Summary of Modules

### 1. IoT & Embedded Systems (`iot-arduino/`)
- **Robust Sensor Reading Routines:** Development of reliable data acquisition routines handling unreliable sensors in C++ and Arduino environments. Features multi-sample collection, trimmed mean outlier filtering, range consistency checks, linear scaling to temperature ranges, and IEEE-754 special value error propagation (`-INFINITY` and `+INFINITY`).
- **Moving Average Data Management:** Window-based sensor tracking utilizing fixed-size arrays with $O(1)$ running sum maintenance for high efficiency on resource-constrained microcontrollers.

### 2. 3D Printing & CAD Design (`3d-printing-cad/`)
- **CR2032 Battery Holder:** Parametric CAD design in Autodesk Fusion 360 featuring a slide-on locking lid and integrated rails.
- **Tolerance Analysis:** Engineered with precise clearances ($0.3\text{ mm}$ static cavity fit and $0.4\text{ mm}$ moving lid interface) to compensate for standard PLA thermal expansion ($0.25\text{ mm}$).
- **Slicing Optimization:** Configured for Ultimaker Cura on a Creality Ender-5 ($0.2\text{ mm}$ layer height, $100\text{ mm/s}$ print speed, $15\%$ grid infill, support-free overhangs).

### 3. Ergonomics & Workspace Lighting (`ergonomics-lighting/`)
- **Light and Shade Analysis:** Comparative photographic evaluation (`Kyryll1.png` vs. `Kyryll2.png`) analyzing how lighting conditions impact visual comfort, eye strain, and productivity. Investigates the transition from unregulated direct glare to diffused natural light and supplementary task lighting.

---

## References & Official Links

- [University of Novi Sad - BIP IoT & 3DP Announcement](https://www.uns.ac.rs/index.php/en/vesti-eng/8856-trece-izdanje-bip-iot-3dp)
- EUGLOH 2.0 Academic Framework
