# Void Browser – Architecture Overview

This documentation has been prepared to explain **why the architectural structures designed within Void Browser were designed this way**.  
This document is not an API or code reference; it has been written to record the reasoning behind architectural decisions.

---

## 0. Scope and Approach

Void Browser does not aim to replicate the extensive human resources and mature architectures of today's modern web browsers.

This project is an experimental web browser, developed by a single developer, with the goal of testing security-focused architectural ideas.

---

## 1.0 Security

In the Void Browser architecture, security comes before functionality.  
Web content is inherently considered untrustworthy, and sensitive data must be strictly isolated from this content.

---

### 1.1 Why Sandbox?

Today, many malicious programs aim to steal user credentials by targeting browser data after gaining access to the system.

Cookies contain session keys, authentication information, OAuth tokens, and critical data that directly represents the user.  
Therefore, cookies are considered highly sensitive assets.

To mitigate this risk, Void Browser completely separates cookie management from the main GUI / renderer process.

When the browser starts:
- A separate process is created for cookie management
- This process runs in a sandbox environment
- The GUI/renderer process cannot directly access cookie data

All cookie read and write operations are performed exclusively within the sandbox. Communication between the GUI and the sandbox is provided through a controlled mechanism.

---

### 1.2 Inter-Process Communication (IPC)

Communication between the sandbox and the GUI is performed using the Inter-Process Communication (IPC) mechanism.

IPC is used in the Void Browser architecture solely to cross security boundaries. This mechanism prevents the GUI process from directly accessing sensitive data within the sandbox.

All data exchanges via IPC must be:
- Limited
- Defined
- Controlled

This approach ensures that even if the GUI or renderer process is compromised, cookie data is not directly exposed.

This approach prevents the direct exposure of cookie data even if the GUI or renderer process is compromised.

---

## 2. Architectural Principle

The use of IPC and sandboxing in the Void Browser architecture is chosen solely for security reasons, not for convenience or performance.

This architecture is built on the principles of minimum attack surface and maximum isolation.