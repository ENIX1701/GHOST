# Functionality

GHOST is designed to simulate a modular post-exploitation agent. Its capabilities are divided into distinct modules which can be toggled at compile-time.

> [!NOTE]
> All *Modules* are mapped by name to [MITRE ATT&CK](https://attack.mitre.org/) tactics. For more detailed information go there :3

Below is a breakdown of the currently implemented modules and their specific methods.

---

## Persistence

The **Persistence** module ensures the implant restarts automatically after the host system reboots or the user logs in. The module attempts to install enabled persistence methods sequentially until *at least* one succeeds (in the future this would, ideally, be configurable).

### 1. Run control (RC files)

**Flag**: `PERSIST_RUNCONTROL`  
**Target**: Linux shell configuration files  
**Mitre ATT&CK mapping**: [T1546.004](https://attack.mitre.org/techniques/T1546/004/)  

This method appends a background execution command (like `~/.ghost/GHOST &`) to the user's shell initialization files.

> [!NOTE]
> Targeted files are: `.bashrc`, `.bash_profile`, `.profile`, `.zshrc`

This causes the implant to activate each time the user logs on (and, in turn, survives reboots!).

### 2. Systemd service

**Flag**: `PERSIST_SERVICE`  
**Target**: Systemd user manager  
**Mitre ATT&CK mapping**: [T1053.006](https://attack.mitre.org/techniques/T1053/006/)  

This method creates and enables a user-level Systemd service.

> [!IMPORTANT]
> The service is located at `~/.config/systemd/user/ghost.service`, set to `Restart=always` and hooks into `default.target`. It also does not require root privileges, since it uses `systemctl --user` :3

### 3. Cron job

**Flag**: `PERSIST_CRON`  
**Target**: User crontab  
**Mitre ATT&CK mapping**: [T1053.003](https://attack.mitre.org/techniques/T1053/003/)  

This method adds a scheduled task to the current user's crontab.

> [!NOTE]
> The trigger is `@reboot`.

---

## Impact

The **Impact** module focuses on simulating the most notorious (and highly preventable!) threat actors, such as [ransomware](https://en.wikipedia.org/wiki/Ransomware) and [wiper](https://en.wikipedia.org/wiki/Wiper_(malware)) attacks.

> [!CAUTION]
> These methods *can* (and more often than not *will*) modify user data. They can cause harm to the devices, where they're deployed (trust me, had to reinstall WSL a few times...), so you have to **make absolutely sure** that you and your clients understand and accept the consequences. 

> [!CAUTION]
> Before running any **Impact** functionality you should backup all data stored on targeted devices.

### 1. Encryption

**Flag**: `IMPACT_ENCRYPT`  
**Mitre ATT&CK mapping**: [T1486](https://attack.mitre.org/techniques/T1486/)  
**Supported cipher flags**: [`XOR`](https://en.wikipedia.org/wiki/XOR_cipher), [`AES`](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) (stub), [`CHACHA`](https://en.wikipedia.org/wiki/Salsa20) (stub)  

Recursively iterates through user's *home* directory (returned by `SystemUtils::GetUserHome()`) and encrypts files that user has write access to. It then applies the selected encryption algorithm with key set in [`Config.hpp`](../include/core/Config.hpp) by overwriting the files in place.

> [!NOTE]
> This method supports multiple ciphers via the `IEncryptionStrategy` interface. The algorightm is selected at compile-time via the `ENCRYPTION_ALGO` flag.

### 2. Wiper

**Flag**: `IMPACT_WIPE`  
**Mitre ATT&CK mapping**: [T1561.001](https://attack.mitre.org/techniques/T1561/001/)  

Simulates a wiper. It wipes your files. Everything's getting destroyed. No going back.

> [!CAUTION]
> This should be only deployed in fully controlled and/or simulated environments to test resilience. Highly destructive.

---

## Exfiltration

The **Exfiltration** module simulates the "unauthorized" transfer of data from a compromised host to the C2 server (SHADOW in this case).

### 1. HTTP post

**Flag**: `EXFIL_HTTP`  
**Mitre ATT&CK mapping**: [T1041](https://attack.mitre.org/techniques/T1041/)  

Exfiltrates data via standard HTTP POST requests. Uses `Comms` wrapper for the implementation. Format of sent data is dependent on the context.

### 2. DNS tunneling (stub) [BACKLOG]

**Flag**: `EXFIL_DNS`  
**Mitre ATT&CK mapping**: [T1048.003](https://attack.mitre.org/techniques/T1048/003/)  

> [!IMPORTANT]
> This is a stub. It would be really cool if it got implemented, but until it is, nothing's set in stone. It may or may not get purged in the future.

## Command and control

While not a standalone module, the core logic handles standard C2 operations via the **SHADOW** protocol.

### Beacons

GHOSTs send heartbeats at configurable intervals (with possible jitter). This can be used to check blue team detection tools and metrics. 

### Commands

Implants accept the following tasks:

| Command       | Arguments     | Description                                                                   |
|---------------|---------------|-------------------------------------------------------------------------------|
| `EXEC`        |`[command]`    | Executes a shell command via `popen` and returns its output.                  |
| `IMPACT`      |`None`         | Manually triggers the configured [IMPACT](#impact) module.                    |
| `STOP_HAUNT`  |`None`         | Kills the GHOST. This includes cleanup and self-destruction of the binary.    |
