# README for `graphical-environment-linux`

## Description
This project contains configuration files and scripts for setting up a minimalist graphical environment on Linux, utilizing `dwm`, `slstatus`, and `dmenu` along with system scripts for various tasks.

## Included Files
1. `config.dwm.h` - Configuration for `dwm` (dynamic window manager).
2. `config.slstatus.h` - Configuration for `slstatus` (status panel).
3. `config.dmenu.h` - Configuration for `dmenu` (dynamic menu).
4. `.xprofile` - Script that runs at the start of an X session.
5. `sys_scripts/` - Collection of scripts for managing system tasks.

## Installation Instructions
Each file or script must be installed or copied into the appropriate directories in your system. Specifically:

1. **dwm**:
   - Install `dwm` from your distribution's repository or compile from source.
   - Rename `config.dwm.h` to `config.h` and copy it to the `dwm` configuration directory, then recompile `dwm`.

2. **slstatus**:
   - Similarly, install and configure `slstatus`, renaming `config.slstatus.h` to `config.h`.

3. **dmenu**:
   - Install `dmenu` and replace the default configuration file by renaming `config.dmenu.h` to `config.h`.

4. **.xprofile**:
   - Place the `.xprofile` file in your home directory.

5. **sys_scripts**:
   - The scripts should be placed in the subfolder `~/.config/sys_scripts/`.
   - Grant the scripts execution rights with the command `chmod +x`.

After installing all components, restart your X session to activate the changes.

## Notes
- All settings and scripts assume a basic understanding of Linux and the X Window System.
- Always create backups before modifying system files.

