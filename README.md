<div align="center">
  <h1 style="color:#00FF41; font-family: 'Courier New', monospace; margin: 20px 0 5px; letter-spacing: 1px;">
    G O N D A
  </h1>
  <p style="color:#00FFFF; font-family: 'Courier New', monospace; margin: 0; font-size: 14px;">
    <em>A terminal-based file explorer with C++</em>
  </p>
</div>

<br>

<div style="background:#0D0D0D; padding:20px; border-radius:12px; border:2px solid #00FF41; box-shadow:0 0 20px rgba(0,255,65,0.3); font-family:'Courier New',monospace; max-width:900px; margin:0 auto;">
  
  <!-- Terminal Header -->
  <div style="background:#1A1A1A; padding:8px 15px; border-radius:8px 8px 0 0; color:#00FF41; font-weight:bold; display:flex; justify-content:space-between; align-items:center;">
    <span>C:\Users\Guest&gt; gonda.exe</span>
    <span style="font-size:12px; color:#666;">● ■ ▲</span>
  </div>

  <!-- Terminal Body -->
  <div style="background:#000000; color:#FFFFFF; padding:15px; border-radius:0 0 8px 8px; line-height:1.4; font-size:14px; overflow:hidden;">
    
    <!-- Tabs -->
    <div style="color:#00FF41; margin-bottom:8px;">
      <span style="background:#00FF41; color:#000; padding:2px 8px; border-radius:4px;">[1]</span>
      <span style="color:#888; margin-left:8px;">[2] [3] [4]</span>
      <span style="float:right; color:#666;">+</span>
    </div>

    <!-- Path Bar -->
    <div style="background:#001F3F; color:#00FFFF; padding:4px 10px; border-radius:4px; margin-bottom:10px; font-size:13px;">
      Path: C:\Windows\System32
    </div>

    <!-- Title -->
    <div style="color:#00FF41; font-weight:bold; margin-bottom:8px;">
      THIS PC - DRIVES
    </div>

    <!-- Table Header -->
    <div style="display:flex; color:#AAAAAA; font-size:12px; border-bottom:1px solid #333; padding-bottom:4px; margin-bottom:6px;">
      <div style="width:40px; text-align:center;">D</div>
      <div style="width:100px;">Name</div>
      <div style="flex:1; margin-left:20px;">Label</div>
      <div style="width:140px; text-align:right;">Used / Total</div>
    </div>

    <!-- Drive Rows -->
    <div style="color:#FFFFFF;">
      <div style="display:flex; align-items:center; padding:3px 0; background:#001F00; margin-bottom:2px; border-radius:4px;">
        <div style="width:40px; text-align:center; color:#00FF41;">D</div>
        <div style="width:100px; color:#00FFFF;">C:\</div>
        <div style="flex:1; margin-left:20px; color:#FFFFFF;">Windows (C:)</div>
        <div style="width:140px; text-align:right; color:#888;">45.2 GB / 237 GB</div>
      </div>
      <div style="display:flex; align-items:center; padding:3px 0;">
        <div style="width:40px; text-align:center; color:#00FF41;">D</div>
        <div style="width:100px; color:#00FFFF;">D:\</div>
        <div style="flex:1; margin-left:20px; color:#FFFFFF;">Data (D:)</div>
        <div style="width:140px; text-align:right; color:#888;">120 GB / 1.0 TB</div>
      </div>
      <div style="display:flex; align-items:center; padding:3px 0;">
        <div style="width:40px; text-align:center; color:#00FF41;">D</div>
        <div style="width:100px; color:#00FFFF;">E:\</div>
        <div style="flex:1; margin-left:20px; color:#FFFFFF;">Backup (E:)</div>
        <div style="width:140px; text-align:right; color:#888;">89 GB / 500 GB</div>
      </div>
    </div>

    <!-- Status Bar -->
    <div style="margin-top:15px; color:#666; font-size:12px; text-align:center;">
      [↑↓:Navigate] [Enter:Open] [←:Back] [Del:Delete] [+:New Tab] [F5:Refresh] [U:Help] [Esc:Exit]
    </div>
  </div>
</div>

<br>

## Features

<div style="background:#001F00; color:#00FF41; padding:12px; border-left:4px solid #00FF41; border-radius:8px; font-family:'Courier New',monospace; margin:15px 0;">
  <strong>Multi-tab support</strong> (up to 9 tabs, default 4)<br>
  <strong>"This PC" view</strong> with drive labels and space usage<br>
  <strong>Directory navigation</strong> with history and back support<br>
  <strong>File operations</strong>: Open • Delete (Y/N) • Refresh<br>
  <strong>Responsive UI</strong> • Auto column sizing<br>
  <strong>USB drive detection</strong> • <strong>File size formatting</strong> (B → TB)<br>
  <strong>Color-coded interface</strong> using Windows console attributes<br>
  <strong>Self-installer</strong> → <code style="color:#FFFF00;">C:\Windows\System32\gonda.exe</code>
</div>

---

## How to Use

```bash
gonda.exe
```

> First run:  
> <span style="color:#FFFF00;">Install to System32? (Y/N)</span>  
> • <code>Y</code> → Run globally with `gonda`  
> • <code>N</code> → Portable mode

---

### Keyboard Shortcuts

<table style="width:100%; border-collapse:collapse; font-family:'Courier New',monospace; background:#000; color:#00FF41; border:1px solid #00FF41; border-radius:8px; overflow:hidden;">
  <tr style="background:#001F00;">
    <th style="padding:8px; border-bottom:1px solid #00FF41; text-align:left;">Key</th>
    <th style="padding:8px; border-bottom:1px solid #00FF41; text-align:left;">Action</th>
  </tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">↑ ↓</td><td style="padding:8px; border-bottom:1px solid #333;">Navigate</td></tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">Enter / →</td><td style="padding:8px; border-bottom:1px solid #333;">Open</td></tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">← / Backspace</td><td style="padding:8px; border-bottom:1px solid #333;">Go back</td></tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">1-9</td><td style="padding:8px; border-bottom:1px solid #333;">Switch tab</td></tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">+</td><td style="padding:8px; border-bottom:1px solid #333;">New tab</td></tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">Del</td><td style="padding:8px; border-bottom:1px solid #333;">Delete (Y/N)</td></tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">F5 / R</td><td style="padding:8px; border-bottom:1px solid #333;">Refresh</td></tr>
  <tr><td style="padding:8px; border-bottom:1px solid #333;">U</td><td style="padding:8px; border-bottom:1px solid #333;">Help</td></tr>
  <tr><td style="padding:8px;">Esc</td><td style="padding:8px;">Exit</td></tr>
</table>

---

## Technical Notes

- **Windows API**: `FindFirstFileW`, `GetLogicalDrives`, `ShellExecuteW`
- **Unicode support** for paths and filenames
- **Auto layout** based on console width
- **Live drive refresh** every 2 seconds
- **Zero dependencies** — single `.exe`

---

## Build from Source

```bash
g++ -O2 -static main.cpp -o gonda.exe -lshlwapi
```

> Requires **MinGW** or **MSVC + Windows SDK**

---

## Security

<div style="background:#300000; color:#FF5555; padding:12px; border:2px solid #FF0000; border-radius:8px; font-family:'Courier New',monospace;">
  <strong>Found a vulnerability?</strong><br>
  <strong>DO NOT</strong> open a public issue.<br>
  Follow our <a href="SECURITY.md" style="color:#FFFF00; text-decoration:underline;">Security Policy</a>
</div>

> [Report a Vulnerability](SECURITY.md)

---

<div align="center" style="margin:30px 0; color:#00FFFF; font-family:'Courier New',monospace;">
  <strong>Compact • Fast • Terminal-Powered</strong><br>
  <em>Made for power users who love the command line.</em>
</div>
