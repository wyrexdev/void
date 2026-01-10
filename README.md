<div align = center>

<div style='margin-bottom: 30px; margin-top: 20px;'>
<img src='https://cdn.vobrow.com/VB.png' style="width: 750px;">
</div>

<img src='https://img.shields.io/badge/Workflow-Void-blue?style=for-the-badge&logo=git'>
<img src='https://img.shields.io/github/issues/wyrexdev/void?style=for-the-badge&logo=github'>
<img src='https://img.shields.io/github/issues-pr/wyrexdev/void?style=for-the-badge&logo=github'>
<img src='https://img.shields.io/github/languages/top/wyrexdev/void?style=for-the-badge'>
<img src='https://img.shields.io/github/license/wyrexdev/void?style=for-the-badge'>
<img src='https://img.shields.io/badge/discord-wyrex-red?style=for-the-badge&logo=discord'>

<div style="margin-top: 10px;">
<p style="font-family: 'Trebuchet MS', sans-serif;">
Void is an experimental browser architecture built from the ground up.
<br>
HTML parsing, rendering, and execution layers are fully custom.
<br>
The graphics pipeline is built on Skia and OpenGL, with processes isolated via IPC.
<br>
Styling and behavior break from traditional hierarchies.
<br>
The system is designed with control and observability as first-class concerns.
</p>
</div>

--- 
</div>

<h2 style="font-family: 'Trebuchet MS', sans-serif;">Features</h2>
<div style="font-family: 'Trebuchet MS', sans-serif;">• IPC communication prevents malicious software that attempts to manipulate the browser by running JIT codes on an internal machine.</div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• Thanks to IPC Communication, Cookies data is processed and kept confidential in such a way that normal users are denied permission to read and write them.</div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• When CSS is rendered, JS does not command CSS; rather, CSS commands JS—a reverse architecture.</div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• Has Self 3D Render Pipeline - 2D Skia Render Pipeline Used.</div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• Has Self JIT.</div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• Cross-platform preparation using QT6 in GUI.</div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• Has Self HTML CSS Tokenizer and Parser.</div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• Has Self ResourceManager for URL's fetching.</div>

---

<h2 style="font-family: 'Trebuchet MS', sans-serif;">Installation</h2>

<h3 style="font-family: 'Trebuchet MS', sans-serif;">1.1 - Required</h3>

<div style="font-family: 'Trebuchet MS', sans-serif;">• Google Skia </div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• Freetype2 </div>
<div style="font-family: 'Trebuchet MS', sans-serif;">• SQLite3 </div>

<h3 style="font-family: 'Trebuchet MS', sans-serif;">1.2 - Skia Installation</h3>

---

<h4 style="font-family: 'Trebuchet MS', sans-serif;">1.2.1 - System update and dependencies</h4>

```
sudo pacman -Syu sudo pacman -S --needed base-devel git python clang ninja cmake freetype2 fontconfig mesa
```

<h4 style="font-family: 'Trebuchet MS', sans-serif;">1.2.2 - Create directory and clone repository</h4>

```
mkdir -p ~/opt
cd ~/opt
git clone https://github.com/google/skia 
cd skia
```

<h4 style="font-family: 'Trebuchet MS', sans-serif;">1.2.3 - Sync dependencies</h4>

```
python3 tools/git-sync-deps
```

<h4 style="font-family: 'Trebuchet MS', sans-serif;">1.2.4 - Install bazelisk (optional, for some missing configurations)</h4>

```
sudo pacman -S bazelisk
```

<h4 style="font-family: 'Trebuchet MS', sans-serif;">1.2.5 - Configure build with gn</h4>

```
gn gen out/Release --args='
is_official_build=true 
is_debug=false 
cc="clang" 
cxx="clang++"
skia_use_fontations=false 
skia_enable_fontmgr_fontconfig=true'
```

<h4 style="font-family: 'Trebuchet MS', sans-serif;">1.2.6 - Build Skia</h4>

```
ninja -C out/Release
```

---

<h3 style="font-family: 'Trebuchet MS', sans-serif;">1.3 - Git Clone</h3>

```
git clone https://github.com/wyrexdev/void.git
```

---

<h3 style="font-family: 'Trebuchet MS', sans-serif;">1.4 - Run Project</h3>

```
bash RUN
```

---

<!---<div align = center>
<h2 style="font-family: 'Trebuchet MS', sans-serif;">Gallery</h2>
</div>--->
