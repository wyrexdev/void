# For Installing Skia (Arch Linux)

# 1. System update and dependencies
sudo pacman -Syu
sudo pacman -S --needed base-devel git python clang ninja cmake freetype2 fontconfig mesa

# 2. Create directory and clone repository
mkdir -p ~/opt
cd ~/opt
git clone https://github.com/google/skia
cd skia

# 3. Sync dependencies
python3 tools/git-sync-deps

# 4. Install bazelisk (optional, for some build configurations)
sudo pacman -S bazelisk

# 5. Configure build with gn
gn gen out/Release --args='
    is_official_build=true
    is_debug=false
    cc="clang"
    cxx="clang++"
    skia_use_fontations=false
    skia_enable_fontmgr_fontconfig=true
'

# 6. Build Skia
ninja -C out/Release