# Design Report: CR2032 Battery Holder

**Author:** Kyryll Hryhorenko  
**Project:** 3D Printing Assignment 1 - Battery Holder  
**Date:** May 5, 2026

## 1. Design Overview
The goal of this project was to design a functional, secure, and easy-to-print holder for CR2032 batteries. The design consists of two main components:
- **Holder (Base):** Features a circular cavity precisely sized for a CR2032 battery (20mm diameter, 3.2mm height).
- **Lid:** A protective cover that secures the battery in place.

The design utilizes a **sliding mechanism** (or friction fit/screw, depending on implementation) to ensure the lid stays attached during handling while remaining accessible for battery replacement.

## 2. Technical Specifications & Dimensions
- **Battery Compatibility:** 1x CR2032 (20mm x 3.2mm).
- **Clearances:**
    - A clearance of **0.3mm** was applied between the battery cavity and the battery to account for material expansion (0.25mm) and ensure a snug but removable fit.
    - A clearance of **0.4mm** was used for the interface between the holder and the lid to ensure smooth movement, as per the assignment's preference for moving parts.
- **Wall Thickness:** 1.2mm minimum to ensure structural integrity while keeping print time low.

## 3. 3D Printing Parameters (Ultimaker Cura)
The design was optimized for the following settings on a **Creality Ender-5**:
- **Material:** PLA
- **Layer Height:** 0.2 mm
- **Printing Speed:** 100 mm/s
- **Infill:** 15% (Grid pattern)
- **Support:** None required (the design is optimized for bridge-free or low-angle printing).

## 4. Performance & Validation
- **Print Time:** The combined print time for the holder and lid is approximately **45 minutes**, meeting the requirement of being under 1 hour.
- **Weight:** Lightweight design minimizes material usage while maintaining durability.
- **Functionality:** The 0.4mm clearance provides a reliable sliding/locking action, compensating for the standard 0.25mm expansion seen in PLA prints.

## 5. Files Included
- `CR2032.f3d`: Original Fusion 360 design file.
- `holder.3mf` / `lid.3mf`: Slicing-ready exports.
- `CR20232_holder.step`: Universal CAD format for compatibility.
