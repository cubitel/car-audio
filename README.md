Car Audio
=========

USB audio amplifier that turns android-based tablet into
full-featured automotive head unit.

#### Interfaces
- USB device interface for tablet
- USB host interface
- 4x45W digital class D automotive audio amplifier
- 2 CAN interfaces
- 100Base-T1 automotive ethernet interface
- Analog input for steering wheel keys
- FM radio available as separate shield

Amplifier acts as USB multifunctional device with ethernet and audio interfaces.

USB ethernet interface is used for configuration and control.
Interface has static IP address 192.168.91.11/24.

Hardware
--------

KiCad pcb files located in _pcb_ directory.
They include main amplifier board and FM tuner shield.

Software
--------

There is bootloader stub and main firmware.

#### Requirements

To install build tools in ubuntu:
```
sudo apt-get install gcc-arm-none-eabi dfu-util openssl protobuf-compiler python-protobuf
```
