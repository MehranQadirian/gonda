
# Gonda File Manager

A lightweight, console-based file manager for Windows, written in C++. It provides a multi-tab interface similar to modern file explorers, with navigation, file operations, and drive information — all within the Windows Command Prompt.

## Features

- **Multi-tab support** (up to 9 tabs, default 4)
- **This PC view** showing all drives with labels, used/total space
- **Directory navigation** with history and back/forward support
- **File and folder operations**:
  - Open files/folders
  - Delete (with confirmation)
  - Refresh view (`F5` or `R`)
- **Responsive UI** with dynamic column sizing based on console width
- **Drive detection** including removable USB drives
- **File size formatting** (B, KB, MB, GB, TB)
- **Color-coded interface** using Windows console attributes
- **Self-installer** to copy `gonda.exe` to System32 for global access

## How to Use

1. **Download** the compiled `gonda.exe`
2. **Run** the executable in Command Prompt:
   ```
   gonda.exe
   ```
3. On first run, it will ask to install itself to `C:\Windows\System32` (requires **Administrator privileges**).  
   - Press `Y` to install and run from anywhere using just `gonda`  
   - Press `N` to run portably (only from current directory)

### Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `↑` `↓` | Navigate up/down |
| `Enter` / `→` | Open selected item |
| `←` / `Backspace` | Go back |
| `1-9` | Switch to tab |
| `+` | Create new tab |
| `Del` | Delete selected (confirmation required) |
| `F5` / `R` | Refresh current view |
| `U` | Show help |
| `Esc` | Exit |

## Technical Notes

- Uses Windows API (`FindFirstFileW`, `GetLogicalDrives`, `ShellExecuteW`, etc.)
- Supports Unicode paths and filenames
- Automatically detects console size and adjusts layout
- Refreshes drive info every 2 seconds when in "This PC" view
- No external dependencies — single executable

## Build from Source (Optional)

```bash
g++ -O2 -static main.cpp -o gonda.exe -lshlwapi
```

Requires MinGW or MSVC with Windows SDK.

---
Compact, fast, and fully functional file manager for power users in the terminal.
