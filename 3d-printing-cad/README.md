# 3D Printing & CAD Design (Fusion 360 & Cura)

This directory contains CAD models, slicing files, and design reports for 3D printing assignments completed during the Erasmus+ BIP program at the University of Novi Sad.

## Project: CR2032 Battery Holder

A functional, secure, and easy-to-print storage container for CR2032 coin cell batteries featuring a slide-on locking lid.

### Key Specifications & Features
- **Target Component:** 1x CR2032 Battery ($20\text{ mm} \times 3.2\text{ mm}$).
- **Mechanism:** Slide-on lid with integrated alignment rails; friction fit requiring no screws or glue.
- **Tolerances & Clearances:**
  - *Static Fit (Battery Cavity):* $0.3\text{ mm}$ functional clearance $+ 0.25\text{ mm}$ anticipated PLA thermal expansion ($0.55\text{ mm}$ total gap).
  - *Moving Fit (Lid/Base Interface):* $0.4\text{ mm}$ sliding clearance $+ 0.25\text{ mm}$ expansion ($0.65\text{ mm}$ total gap) for smooth gliding without binding.
- **Wall Thickness:** $1.2\text{ mm}$ minimum for structural rigidity and fast printing.

### 3D Printing Parameters (Ultimaker Cura)
- **Printer:** Creality Ender-5
- **Material:** PLA
- **Layer Height:** $0.2\text{ mm}$
- **Print Speed:** $100\text{ mm/s}$
- **Infill:** $15\%$ (Grid pattern)
- **Supports:** None required (optimized for bridge-free overhangs).
- **Print Time:** $\approx 45\text{ minutes}$ for base and lid.

### Files Included
- `CR2032.f3d`: Original parametric Fusion 360 design file.
- `holder.3mf`: Slicing-ready model of the base.
- `lid.3mf`: Slicing-ready model of the locking lid.
- `CR2032_Report.md`: Full engineering design report.
