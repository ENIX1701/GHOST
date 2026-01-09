# GHOST

GHOST is an implant for AETHER deployed to victim Linux and Windows hosts.

## Overview
**Role:** Run on target machine and execute commands remotely  
**Platforms:** Windows, Linux  
**Idea:** Beacons poll the server every X seconds (possibly jittered to [obfuscate](https://attack.mitre.org/techniques/T1001/) the traffic).  

## Capabilities:
- Establish persistence
- Execute shell commands
- Data exfiltration
- Download (and execute) additional payloads
- (maybe) Inject shellcode

## Architecture

GHOST is a standalone implant that can be run on Windows and Linux systems. I've decided that, since I have some networking basics, I'll try to implement some very basic raw HTTP over sockets. It's not as difficult as it sounds, but there are a few things that need to be taken into consideration:
1. TCP vs UDP sockets -> most sensible solution would be to use TCP for any communication, where we expect a response (like registering into the SHADOW) and UDP when sending (so for data exfiltration) !!! THIS WAS COMPLETELY WRONG, sticking to TCP, as the server is a RESTful API
2. Sockets are handled differently on Windows and Linux. This means that I'll need to use macros to define which libraries get compiled with which version. This is quite easy (and non-trivial at the same time!), but it's important to keep track of what's needed where
3. JSON parsing will be done "manually", so it's important to implement necessary sanitization (to not "blow up" the implant, that'd be awkward)
4. Main Ghost class is somewhat abstract. It would be possible (and even a good practice, but firstly I want to see it work) to create an interace and then a separate implementation, so that it's possible to swap out different components (food for thought for later parametrization)
5. Overall this is very low level in comparison to JavaScript or Python way of doing it, but it's a priceless learning experience

## How to run

> TODO

## TODO

- [ ] MVP (core beacon loop; connect -> get task -> execute -> sleep)
- [ ] network -> basic network connectivity (consider raw sockets or HTTP)
- [ ] persistence -> either add a run key or a .lnk in startup
- [ ] stealth -> jitter for network comms, maybe some source obfuscation or (if very bored) polymorphism?

## Legal

> **Disclaimer:** This software is for educational purposes and authorized red team engagements only. The authors are not responsible for misuse.