# Hound App - Wii U Port

This is the Wii U version of the Hound Media Server app, built using the [wut](https://github.com/devkitPro/wut) toolchain and SDL2.

## Prerequisites

To build this project, you need:

1. **devkitPro** with Wii U support
   - Download from: https://devkitpro.org/
   - Install using the devkitPro pacman

2. **wut** (Wii U Toolchain)
   - Installed via devkitPro pacman: `sudo dkp-pacman -S wiiu-wut`

3. **SDL2 for Wii U**
   - Install via: `sudo dkp-pacman -S wiiu-sdl2 wiiu-sdl2_image wiiu-sdl2_ttf wiiu-sdl2_net`

4. **Environment Variables**
   ```bash
   export DEVKITPRO=/path/to/devkitpro
   export WUT_ROOT=$DEVKITPRO/wut
   ```

## Building

### Linux/macOS

```bash
# Set up environment
source $DEVKITPRO/wut/etc/wut.env

# Build the project
cd wiiu
./build.sh
```

### Windows

Use the devkitPro msys2 terminal:

```bash
# Set up environment (usually done automatically)
source $DEVKITPRO/wut/etc/wut.env

# Build the project
cd wiiu
./build.sh
```

## Output

The build process will create:
- `wiiu/output/hound_app_wiiu.rpx` - The Wii U executable

## Installation

To install on your Wii U:

1. Copy the `.rpx` file to your Wii U's SD card
2. Place it in `sd:/wiiu/apps/hound_app/`
3. Create a `meta.xml` file in the same directory:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<app version="1">
  <name>Hound App</name>
  <coder>Hound Media Server</coder>
  <version>1.0.0</version>
  <release_date>20241023</release_date>
  <short_description>Stream media from your Hound server</short_description>
  <long_description>Hound App for Wii U allows you to stream media from your Hound Media Server to your Wii U console.</long_description>
  <ahb_access needed="true"/>
</app>
```

4. Launch from the Homebrew Launcher

## Controls

| Wii U Controller | Action |
|------------------|--------|
| D-Pad Up/Down | Navigate menu |
| A Button | Select/Activate |
| B Button | Back/Go to previous screen |
| Left Stick | Navigate (alternative) |
| Right Stick | Scroll (where applicable) |
| + Button | Show settings |
| - Button | Show info |

## Features

- [x] Basic SDL2 application framework
- [x] Wii U controller input support
- [x] UI navigation system
- [x] Network connectivity
- [x] Hound API client
- [ ] Video playback (requires MPV or custom decoder)
- [ ] Audio playback
- [ ] Full Hound API integration
- [ ] Collections support
- [ ] Search functionality
- [ ] Settings menu

## Architecture

The Wii U version uses a different architecture from the React Native version:

1. **Core**: C++ with SDL2 for rendering and input
2. **Networking**: SDL2_net for HTTP requests
3. **UI**: Custom UI system with SDL2 rendering
4. **Video**: Placeholder for video playback (would need custom decoder)

### Directory Structure

```
wiiu/
├── include/          # Header files
│   ├── app.h         # Main application
│   ├── ui.h          # UI manager
│   ├── controller.h  # Controller input
│   ├── network.h     # Network operations
│   ├── hound_client.h # Hound API client
│   └── video_player.h # Video playback
├── src/              # Source files
│   ├── main.cpp      # Entry point
│   ├── app.cpp       # Application implementation
│   ├── ui.cpp        # UI implementation
│   ├── controller.cpp # Controller implementation
│   ├── network.cpp   # Network implementation
│   ├── hound_client.cpp # Hound client implementation
│   └── video_player.cpp # Video player implementation
├── assets/           # Game assets
├── CMakeLists.txt    # CMake build configuration
├── build.sh          # Build script
└── README.md         # This file
```

## Limitations

1. **No React Native**: The Wii U doesn't support React Native, so this is a native C++ application
2. **Video Playback**: Requires custom video decoder implementation
3. **Memory**: Wii U has limited memory (2GB), so large media libraries may need pagination
4. **Performance**: The Wii U's PowerPC CPU may limit performance for complex UIs

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on Wii U (if possible) or using [decaf-emu](https://github.com/decaf-emu/decaf-emu)
5. Submit a pull request

## License

This project is licensed under the same license as the main Hound App project.
