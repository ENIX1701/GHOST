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

## PROBLEM

Outlined in devlog: **tl;dr** is that Windows is treating this primitive implant as malware, which makes it difficult to run (you need to disable Defender or add an exclusion, it's not something I'd want the user to do). This forces me to rethink the core concept of the GHOST component.

**Long version:**
I've anticipated that GHOST will cause some problems. Its main role is to de-facto simulate an infection on a system, which, combined with how senstive EDR systems are nowadays, trips the behavioral safeguards put in place. What *I* think is happening in this particular instance is: the code uses low-level sockets (`winsock2.h` and `ws2tcpip.h` headers in Windows to be precise), which potentially triggers AV/EDR hooks. What would need to be done is as follows: all the code would need to be highly obfuscated and packed during or right after compilation. On top of that, the code itself would need to bypass EDR user-mode hooks, which means no high-level APIs AND using clever calling mechanisms (like `syscall` *JMP* to evade direct syscalls). These are, for the time being, WAY out of scope of this project. I have not planned on writing state-of-art malware simulation implants, I just wanted something to reliably test the other components.

So, going forward: GHOST will be a **LINUX-ONLY** implant. But, because it was meant to be multi-platform (and it isn't), it will implement broader functionality. This means that it will, for now, try to: establish persistence on a system, beacon and wait for commands, execute them and send back data. It's a bit more advanced than what I was planning on with the multi-platform version, but since it's focused on a single OS, it'll be interesting to test out. I would also love to make this configurable and parametrizable (so that you could potentially use CHARON to generate a GHOST with a particular persistence option, with no beaconing, and with only data-exfiltration mechanism, for example :)). I believe this will more than make up for the lack of Windows version. This is what I'll be pivoting GHOST to. Expect a lot of refactoring in the near future...

## SCRATCHPAD

The README's getting dirty, but it'll be redone once the fully-featured MVP is ready.

What will be changed in the linux version:
- [ ] refactor code to use linux-native mechanisms. no more macros!! (that's a lie, but macros will be used to parametrize mechanisms during compilation -> future CHARON builder integration)
- [ ] persistence through... (look this up)

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