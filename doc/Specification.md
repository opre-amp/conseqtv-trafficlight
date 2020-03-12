# Specification

This file will detail the goals we outlined for ourselves for the homework assignment of the _Application Development Environments_ subject.

## Abstract Outline

The goal of the project is to provide a backend solution (and a client program interfacing with it) for a logically reconfigurable traffic light that has the ability to detect incoming traffic, and can switch on/off the bulbs in the traffic light. By _logically reconfigurable_ we mean that the program reacting to input by changing the output is entirely reconfigurable on-the-fly to allow changes without manual on-site intervention. To comply with the timing requirements of the problem at hand, we will rely on a separate processing unit on the SoC of the board we are using which is not under the scheduler of the operating system, while maintaining connectivity through the _regular_ cores running Linux.

## Architecture

The solution is divided up into two main components: the backend and the client program. The client is trivially constructed, can be run on any suitable consumer operating system such as Microsoft Windows, Gnu/Linux, *BSD or MacOS. It communicates with the backend via REST calls on top of a secure TLS layer. See Section __Communication__ for more information.

The backend is more complicated: it features a multi-core ARM SoC such as the bcm2710, which has direct access to a GPIO block. As seen in Section __Implementation__, we are to use a 4-core Raspberry Pi 3B+ for this purpose, with 3 of the 4 cores running an embedded version of Linux and the 4th running the reconfigurable program mentioned in the outline. The two components communicate through hardware mailboxes which is handled by a kernel module acting as a device driver from the Linux side. This is also detailed in Section __Communication__.

## Communication

This section details the channels of communications used among the different components of this system.

### REST

The backend provides the following services:

* The client may retrieve any measurements published by the reconfigurable program, such as the frequency of the passing traffic.
* The client may send a Police Interrupt, which (when handled by the program) will put the lights into a blinking yellow state.
* The client may stop the currently running program.
* The client may send additional data which can be handled by the program.
* The client may request a self-check for the state of any of the LEDs.

For all of the above, the client must have a valid token which is to be issued and revoked manually.

### Device Driver

The kernel module provides a character device for the communication with the separate program. This communication is done through two mailboxes, one for sending data (TX) and one for receiving data (RX), and two memory regions (memTX, memRX). The mailboxes signal how long the newly put data is at the corresponding memory location. The TX mailbox is special: if it contains an address which is considered to be _executable_, the program will cease to run and will instead jump there. 