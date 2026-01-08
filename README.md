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

## How to run

> TODO

## TODO

- [ ] MVP (core beacon loop; connect -> get task -> execute -> sleep)
- [ ] network -> basic network connectivity (consider raw sockets or HTTP)
- [ ] persistence -> either add a run key or a .lnk in startup
- [ ] stealth -> jitter for network comms, maybe some source obfuscation or (if very bored) polymorphism?

## Legal

> **Disclaimer:** This software is for educational purposes and authorized red team engagements only. The authors are not responsible for misuse.