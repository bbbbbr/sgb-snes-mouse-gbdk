# Changes vs upstream
Made a couple changes as follows to suit my use cases better.

Modified the SGB Mouse handling:
- Use relative deltas from the hardware instead of absolute SNES x,y screen positions
- Include mouse connected status with button data
- Invert values so the bits are active high on the GB side
- Turn off mouse cursor update and display on the SNES side (so GB can display cursor instead)
- Status bit indicating if SGB menu is open (activated when pressing Mouse or GamePad L+R together) so GB can optionally ignore mouse interaction
- Suppress entering menu with the mouse L + R buttons (Can be turned off by commenting out the `wMenuHeldTimoutMouse` line in `mouseInner.s`)

Modified tools:
- Change output to C const arrays
- Include SGB command packet header and trailing zero padding in the arrays so that SGB packets are fully formed in ROM

Example program:
- Changed from an RGBDS asm example to a GBDK-2020 C example

Possible other changes:
- Including mouse acceleration state
- A mechanism to request changing acceleration (there appear to be some control vars)

# Upstream Repo Readme:
--------------------------
# SGB Testbed

This project is used to generate my own hacks for SGB.
