# Hound App - Wii U (Simple Version)

This is a **minimal version** of Hound App for Wii U that only requires:
- **SDL2** (for rendering and input)
- **wut** (Wii U toolchain)

No SDL2_image, SDL2_ttf, or SDL2_net dependencies!

## Why This Version?

The full version requires many dependencies (webp, freetype, harfbuzz) that are hard to install. This simplified version:
- Uses only SDL2 for rendering (colored rectangles)
- Has basic UI navigation
- Works with Wii U GamePad
- No external dependencies beyond SDL2

## Building

### 1. Install devkitPro
```bash
wget https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x install-devkitpro-pacman
sudo ./install-devkitpro-pacman
```

### 2. Install Wii U packages
```bash
sudo dkp-pacman -S wiiu-dev
```

### 3. Set environment
```bash
echo 'export DEVKITPRO=/opt/devkitpro' >> ~/.bashrc
echo 'export WUT_ROOT=$DEVKITPRO/wut' >> ~/.bashrc
echo 'export PATH=$DEVKITPRO/devkitPPC/bin:$DEVKITPRO/wut/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

### 4. Build
```bash
cd wiiu-simple
./build.sh
```

## Output

The build creates: `output/hound_app_wiiu.rpx`

## Installation

Copy to your Wii U SD card:
```bash
cp output/hound_app_wiiu.rpx /media/YourSDCard/wiiu/apps/hound_app/
cp assets/meta.xml /media/YourSDCard/wiiu/apps/hound_app/
```

## Controls

| Wii U Controller | Action |
|------------------|--------|
| D-Pad Up/Down | Navigate menu |
| A Button | Select |
| B Button | Go back |

## Features

- [x] Basic SDL2 application framework
- [x] Wii U controller input
- [x] Simple UI with colored rectangles
- [x] Menu navigation
- [x] Easy to build (only 2 dependencies)
- [ ] Networking (removed for simplicity)
- [ ] Images (removed for simplicity)
- [ ] Custom fonts (removed for simplicity)

## Adding Features Later

Once this builds, you can add:
1. Networking with SDL2_net
2. Images with SDL2_image
3. Fonts with SDL2_ttf

But start with this minimal version first!
