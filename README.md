# Changes vs upstream
Made a couple changes as follows to suit my use cases better.

Modified the SGB Mouse handling:
- Use relative deltas from the hardware instead of absolute SNES x,y screen positions
- Include mouse connected status with button data
- Invert values so the bits are active high on the GB side

Modified tools:
- Change output to C const arrays
- Include SGB command packet header and trailing zero padding in the arrays so that SGB packets are fully formed in ROM

Example:
- Changed from an RGBDS asm example to a GBDK-2020 C example


# SGB Testbed

This uses <https://github.com/ISSOtm/gb-starter-kit> as a template.

This project is used to generate my own hacks for SGB.

Bulding: Use RGBDS < 0.6.0
