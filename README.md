<div align="center">

<!-- Terminal Preview using Code Block -->
```
╔══════════════════════════════════════════════════════════════════════╗
║ ● C:\Users\Guest> gonda.exe                                 − □ ×    ║
╠══════════════════════════════════════════════════════════════════════╣
║  [1]  [2]  [3]  [4]                                                  ║
╠══════════════════════════════════════════════════════════════════════╣
║ Path: C:\Windows\System32                                            ║
╠══════════════════════════════════════════════════════════════════════╣
║                                                                      ║
║  THIS PC - DRIVES                                                    ║
║                                                                      ║
║  ┌──┬────────────┬──────────────────────┬────────────────────────┐   ║
║  │T │ Name       │ Label                │ Used / Total           │   ║
║  ├──┼────────────┼──────────────────────┼────────────────────────┤   ║
║  │D │ C:\        │ Windows (C:)         │ 45.2 GB / 237 GB       │   ║
║  │D │ D:\        │ Data (D:)            │ 120 GB / 1.0 TB        │   ║
║  │D │ E:\        │ Backup (E:)          │ 89 GB / 500 GB         │   ║
║  └──┴────────────┴──────────────────────┴────────────────────────┘   ║
║                                                                      ║
╠══════════════════════════════════════════════════════════════════════╣
║ Enter:Open | Del:Delete | F5:Refresh | Esc:Exit                      ║
╚══════════════════════════════════════════════════════════════════════╝
```

# 🗂️ GONDA FILE MANAGER

**Lightweight Console-Based File Manager for Windows**

<img src="https://img.shields.io/badge/Platform-Windows-blue?style=for-the-badge&logo=windows" alt="Platform"> <img src="https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=cplusplus" alt="Language"> <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License"> <img src="https://img.shields.io/badge/Size-Single%20EXE-orange?style=for-the-badge" alt="Size">

</div>

---

## ⚡ Features

**Gonda** brings power and speed to Windows Command Prompt with a feature-rich interface designed for developers and power users:

- **Multi-Tab Interface** — Manage up to 9 tabs simultaneously (default 4 tabs), switch instantly with number keys
- **This PC View** — Visual overview of all drives with volume labels, used/total space, and real-time USB detection
- **Smart Navigation** — Full history support with back/forward navigation, breadcrumb paths, and instant directory jumping
- **File Operations** — Open files with default applications, delete with confirmation prompts, refresh views on demand
- **Responsive Layout** — Automatically adjusts column widths based on console size for optimal readability
- **Drive Intelligence** — Detects all drive types including removable USB devices, formats sizes from bytes to terabytes
- **Color-Coded UI** — Uses Windows console color attributes for intuitive visual feedback and category distinction
- **Zero Dependencies** — Single portable executable with no external libraries or runtime requirements
- **System Integration** — Optional self-installer copies binary to System32 for global command-line access
- **Unicode Support** — Handles international filenames and paths with full UTF-16 compatibility

---

## 🚀 How to Use

### Quick Start

1. **Download** the latest `gonda.exe` from releases
2. **Run** from Command Prompt or PowerShell:

```cmd
gonda.exe
```

3. **First Launch Setup** — Gonda will prompt for installation:
   - Press `Y` to install to `C:\Windows\System32` (**requires Administrator**)
   - Press `N` to run portably from current directory

After installation, launch from anywhere:

```cmd
gonda
```

### Navigation

- Use **arrow keys** to navigate through files and folders
- Press **Enter** or **→** to open selected item
- Press **←** or **Backspace** to go back to parent directory
- Switch tabs with **number keys 1-9**
- Create new tab with **+** key

---

## ⌨️ Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `↑` `↓` | Navigate up/down through items |
| `Enter` / `→` | Open selected file or folder |
| `←` / `Backspace` | Go back to parent directory |
| `1-9` | Switch to corresponding tab |
| `+` | Create new tab |
| `Del` | Delete selected item (with confirmation) |
| `F5` / `R` | Refresh current view |
| `U` | Show help and keyboard shortcuts |
| `Esc` | Exit application |

---

## 🔧 Technical Notes

Gonda is built with performance and reliability in mind, leveraging native Windows APIs for maximum efficiency:

- **Windows API Integration** — Uses `FindFirstFileW`, `GetLogicalDrives`, `ShellExecuteW`, and other native functions for direct system access
- **Unicode-First Design** — Full support for international characters in paths and filenames via UTF-16 encoding
- **Adaptive UI** — Automatically detects console dimensions and adjusts layout in real-time
- **Live Drive Monitoring** — Refreshes drive information every 2 seconds when viewing "This PC" to detect USB insertions/removals
- **Zero External Dependencies** — Completely self-contained single executable with static linking
- **Minimal Memory Footprint** — Efficient memory management with dynamic allocation only when needed
- **Error Handling** — Graceful handling of permission errors, invalid paths, and system-level exceptions

---

## 🛠️ Build from Source

Compile Gonda yourself with MinGW or MSVC:

```bash
g++ -O2 -static main.cpp -o gonda.exe -lshlwapi
```

**Requirements:**
- MinGW-w64 or Microsoft Visual C++ compiler
- Windows SDK (included with Visual Studio or MinGW)
- C++11 or later

**Compiler Flags Explained:**
- `-O2` — Level 2 optimization for speed
- `-static` — Static linking (no DLL dependencies)
- `-lshlwapi` — Link against Shell Lightweight API

**Build Output:**
- Single portable executable: `gonda.exe`
- Typical size: ~150-200 KB (depending on compiler)

---

## 🔒 Security

We take security seriously. If you discover a vulnerability, please **do not open a public issue**.

Instead, follow our [**Security Policy**](SECURITY.md) to report it responsibly.

> [**Report a Vulnerability →**](SECURITY.md)

All reports are reviewed within 48 hours and handled with confidentiality.

---

---

<div align="center">

### ⚡ Compact. Fast. Powerful. ⚡

*A fully functional file manager for power users who live in the terminal.*

Made with ♥ for the Windows command line

</div>
