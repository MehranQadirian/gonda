# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 1.x     | :white_check_mark: |
| < 1.0   | :x:                |

> **Current version**: `1.0`

---

## Reporting a Vulnerability

We take the security of **Gonda File Manager** seriously. If you discover a security vulnerability, please report it responsibly.

### How to Report

1. **Do NOT open a public GitHub issue.**
2. Send a detailed report to:  
   **Email**: `mehranghadirian@proton.me`

### What to Include

- Type of issue (e.g., privilege escalation, file deletion bypass, path traversal)
- Full steps to reproduce (including screenshots or video if possible)
- Affected component (e.g., `InstallProgram()`, `DeleteSelected()`, `GetDirectoryItems`)
- Potential impact (e.g., data loss, unauthorized access)
- Your environment (Windows version, execution context: admin/user)

### Response Time

- You will receive an acknowledgment within **48 hours**.
- We aim to provide a fix or mitigation within **7–14 days** for critical issues.
- You will be credited in the release notes (unless you prefer anonymity).

---

## Security Considerations in This Project

This application includes **sensitive operations**:

- **System-level file operations** (`DeleteFileW`, `RemoveDirectoryW`)
- **Copying executable to `System32`** (`InstallProgram()`)
- **Direct disk access** via `GetLogicalDrives`, `GetDiskFreeSpaceExW`
- **Shell execution** via `ShellExecuteW`

### Known Security Notes

| Risk | Description | Mitigation |
|------|-----------|------------|
| Requires Admin for Install | Installation copies `gonda.exe` to `C:\Windows\System32` | Clearly documented; user prompted |
| File Deletion Without Recycle Bin | `DeleteFileW` permanently deletes files | Confirmed with Y/N prompt |
| No sandboxing | Runs with user privileges | User should run in least-privilege context |
| Path Traversal (limited) | Uses `\\*` pattern but validates `..` skip | Could be bypassed with symbolic links |

---

## Responsible Disclosure

We follow responsible disclosure principles:

- We will **not take legal action** against researchers acting in good faith.
- We request **no public disclosure** until a fix is released.
- We will **coordinate a disclosure timeline** with you.

---

## Scope

Only vulnerabilities in the **official source code** in this repository are in scope.

Out-of-scope:
- Social engineering
- Physical attacks
- Denial-of-service via resource exhaustion
- Issues in third-party libraries not bundled

---

## Thank You

Thank you for helping keep **Gonda File Manager** and its users safe!
