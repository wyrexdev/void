# Void Browser

![Void Browser](readme/130925-2.png)

Void Browser is a modern, high-performance web browser built with Qt6 and C++20. It features a custom rendering engine with both OpenGL and Vulkan support, advanced HTML parsing capabilities, and a sleek dark theme interface focused on performance and user experience.

## Features

### Core Browser Features
- **Dual Rendering Engine**: OpenGL 4.5 and Vulkan rendering support for maximum performance
- **Advanced HTML Parser**: Custom HTML/CSS parsing engine with tokenization and DOM tree building
- **Network Layer**: libcurl-based HTTP client with SSL/TLS support
- **Tab Management**: Advanced tab system with history tracking and visual indicators
- **Search Integration**: Built-in search functionality with Google integration

### User Interface
- **Modern Dark Theme**: Carefully designed dark color scheme with customizable theming
- **Modular Widget System**: Custom widget hierarchy with hover effects and event handling
- **SVG Icons**: Scalable vector icons for crisp display at any resolution
- **Custom Typography**: Nunito and Nano fonts for modern typography
- **Responsive Layout**: Adaptive sidebar system with left navigation and right quick access

### Technical Features
- **Cross-Platform**: Built with Qt6 for Windows, Linux, and macOS compatibility
- **C++20 Standard**: Modern C++ features for optimal performance
- **Memory Management**: Efficient resource management with Qt's object system
- **OpenGL Integration**: Hardware-accelerated rendering with OpenGL 4.5 compatibility profile
- **Vulkan Support**: Next-generation graphics API support for future-proofing

## Screenshots

The application features:
- Navigation bar with tab management controls
- Left sidebar with home and settings options
- Right sidebar with quick access to web applications
- Central browser area with search functionality
- Modern typography using custom fonts (Nunito and Nano)

## Technical Details

### Architecture
- **Framework**: Qt6 with C++20 standard
- **Rendering**: Dual-engine support (OpenGL 4.5 + Vulkan)
- **Parsing**: Custom HTML/CSS parser with tokenization
- **Network**: libcurl with OpenSSL for secure connections
- **Build System**: QMake with recursive source/header discovery
- **Resource Management**: Qt Resource System for assets

### Engine Components

#### Parser Engine (`src/Engine/Parser/`)
- **HTML Parser**: Custom HTML parsing with DOM tree construction
- **Tokenizer**: Advanced tokenization system for HTML/CSS
- **Document Model**: Structured document representation
- **Node System**: Hierarchical node management for DOM elements

#### Renderer Engine (`src/Engine/Renderer/`)
- **OpenGL Renderer**: Hardware-accelerated OpenGL 4.5 rendering
- **Vulkan Renderer**: Next-generation Vulkan API support
- **Entity System**: Renderable entity management
- **Text Rendering**: Advanced text rendering with custom fonts
- **Color Management**: Comprehensive color handling system

#### Request Engine (`src/Engine/Request/`)
- **HTTP Client**: libcurl-based HTTP/HTTPS client
- **SSL/TLS Support**: Secure connections with OpenSSL
- **Content Fetching**: Efficient web content retrieval

### UI Components (`include/Widget/`)
- **Widget**: Base widget class with advanced event handling
- **Nav**: Navigation bar with tab management and history
- **LeftSideBar**: Navigation sidebar with home and settings
- **RightSideBar**: Quick access sidebar for web applications
- **Image**: Custom image widget with border radius support
- **SvgWidget**: SVG rendering widget with color customization
- **Tab**: Advanced tab management system

### Utility Systems (`include/Utils/`)
- **Theme**: Centralized theme management with color palettes
- **History**: Tab history and navigation tracking
- **Font**: Custom font management (Nunito, Nano)
- **UUID**: Unique identifier generation
- **Settings**: Application configuration management

### Theme System
The application uses a centralized theme system with the following color palette:
- Background: `#111111`
- Tab background: `#262626`
- Surface: `#1c1c1c`
- Primary color: `#2b0099ff`
- Text: `#ffffff`
- Text hover: `#c1c1c1`

## Requirements

### System Requirements
- **Operating System**: Windows 10+, Linux (Ubuntu 20.04+), or macOS 10.15+
- **Graphics**: OpenGL 4.5 compatible graphics card (Vulkan optional but recommended)
- **Memory**: Minimum 4GB RAM (8GB recommended)
- **Storage**: 500MB free space for installation

### Development Dependencies
- **Qt6**: Qt6 framework with the following modules:
  - `widgets` - UI components
  - `svg` - SVG rendering support
  - `network` - Network functionality
  - `concurrent` - Concurrent processing
  - `opengl` - OpenGL integration
  - `openglwidgets` - OpenGL widget support
- **C++ Compiler**: C++20 compatible compiler (GCC 10+, Clang 12+, or MSVC 2019+)
- **libcurl**: HTTP client library with SSL support
- **OpenSSL**: SSL/TLS cryptographic library
- **GLM**: OpenGL Mathematics library (included)
- **STB**: Single-file public domain libraries for C/C++ (included)

## Installation

### Prerequisites
Install the required development libraries:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev libqt6svg6-dev libqt6opengl6-dev
sudo apt install libcurl4-openssl-dev libssl-dev build-essential
```

**Arch Linux:**
```bash
sudo pacman -S qt6-base qt6-svg qt6-tools qt6-5compat
sudo pacman -S curl openssl base-devel
```

**Fedora/RHEL:**
```bash
sudo dnf install qt6-qtbase-devel qt6-qtsvg-devel qt6-qttools-devel
sudo dnf install libcurl-devel openssl-devel gcc-c++ make
```

**Windows:**
1. Download and install Qt6 from [qt.io](https://www.qt.io/download)
2. Install Visual Studio 2019+ or MinGW-w64
3. Install vcpkg and install dependencies:
```cmd
vcpkg install curl openssl
```

**macOS:**
```bash
brew install qt6 curl openssl
```

### Building from Source

1. **Clone the repository:**
```bash
git clone https://github.com/wyrexdev/void
cd Void
```

2. **Generate Makefile:**
```bash
qmake6 Void.pro
```

3. **Build the application:**
```bash
make
```

4. **Run the application:**
```bash
./Void
```

**Quick Run (using RUN file):**
```bash
# Execute the RUN file directly
bash RUN

# Or manually run the commands
qmake6 && make && ./Void
```

## Usage

1. **Launch the application** by running the executable
2. **Navigate** using the tab system in the top navigation bar
3. **Access quick links** from the right sidebar
4. **Use the search bar** in the center of the interface
5. **Access settings** from the left sidebar

## Project Structure

```
Void/
├── src/                           # Source code
│   ├── main.cpp                  # Application entry point
│   ├── Engine/                   # Core browser engine
│   │   ├── Parser/              # HTML/CSS parsing engine
│   │   │   ├── Html/            # HTML parser implementation
│   │   │   │   ├── Parser.cpp   # Main HTML parser
│   │   │   │   └── Tokenizer/   # HTML tokenization system
│   │   │   │       ├── Document.cpp
│   │   │   │       ├── Node.cpp
│   │   │   │       └── Tokenizer.cpp
│   │   │   └── Css/             # CSS parsing (future)
│   │   ├── Renderer/            # Rendering engine
│   │   │   ├── OpenGL/          # OpenGL 4.5 renderer
│   │   │   │   ├── OpenGLRenderer.cpp
│   │   │   │   └── Entities/    # Renderable entities
│   │   │   │       ├── Entity.cpp
│   │   │   │       └── Elements/
│   │   │   │           └── Text.cpp
│   │   │   ├── Vulkan/          # Vulkan renderer
│   │   │   │   └── VulkanRenderer.cpp
│   │   │   └── Utils/           # Rendering utilities
│   │   │       └── Color.cpp
│   │   ├── Request/             # Network layer
│   │   │   └── Fetcher.cpp      # HTTP client implementation
│   │   └── Storage/             # Data storage (future)
│   ├── Utils/                   # Utility classes
│   │   ├── History.cpp          # Tab history management
│   │   └── Theme.cpp            # Theme system
│   └── Stb/                     # STB library implementation
│       └── Implementation.cpp
├── include/                      # Header files
│   ├── Engine/                  # Engine headers
│   │   ├── Parser/             # Parser engine headers
│   │   ├── Renderer/           # Renderer engine headers
│   │   └── Request/            # Network layer headers
│   ├── Utils/                  # Utility headers
│   │   ├── Font.hpp            # Font management
│   │   ├── History.hpp         # History system
│   │   ├── Renderer.hpp        # Renderer interface
│   │   ├── Settings.hpp        # Settings management
│   │   ├── Theme.hpp           # Theme system
│   │   └── UUID.hpp            # UUID generation
│   ├── Widget/                 # UI widget headers
│   │   ├── Buttons/            # Button components
│   │   ├── Image/              # Image handling
│   │   ├── Layouts/            # Layout components
│   │   │   ├── LeftBar/        # Left sidebar
│   │   │   ├── LeftSideBar.hpp
│   │   │   ├── Nav/            # Navigation
│   │   │   └── RightSideBar.hpp
│   │   ├── Nav/                # Navigation components
│   │   │   ├── Preview/        # Tab preview
│   │   │   └── Tab.hpp         # Tab management
│   │   ├── Svg/                # SVG rendering
│   │   │   └── SvgWidget.hpp
│   │   └── Widget.hpp          # Base widget class
│   ├── glm/                    # GLM mathematics library
│   └── Stb/                    # STB library headers
│       └── Truetype.h
├── icons/                       # SVG icon assets
│   ├── arrowLeft.svg
│   ├── arrowRight.svg
│   ├── cancel.svg
│   ├── category.svg
│   ├── home.svg
│   ├── plus.svg
│   ├── refresh.svg
│   ├── settings.svg
│   └── wallet.svg
├── fonts/                       # Custom font files
│   ├── nano.ttf
│   └── nunito.ttf
├── images/                      # Image assets
│   ├── apple.png
│   └── logo.png
├── build/                       # Build artifacts
├── readme/                      # Documentation images
│   ├── 130925.png
│   └── 130925-2.png
├── Void.pro                     # QMake project file
├── Makefile                     # Generated Makefile
├── resources.qrc                # Qt resource file
├── RUN                          # Quick run script
├── LICENSE                      # License file
└── README.md                    # This file
```

## Development

### Architecture Overview
Void Browser follows a modular architecture with clear separation of concerns:

1. **Engine Layer**: Core browser functionality (parsing, rendering, networking)
2. **UI Layer**: Qt-based user interface components
3. **Utility Layer**: Shared utilities and systems

### Adding New Features

#### Engine Development
1. **Parser Extensions**: Add new parsing capabilities in `src/Engine/Parser/`
2. **Renderer Features**: Extend rendering capabilities in `src/Engine/Renderer/`
3. **Network Features**: Add new network protocols in `src/Engine/Request/`

#### UI Development
1. Create new widget classes inheriting from the base `Widget` class
2. Add new theme colors to `include/Utils/Theme.hpp` and `src/Utils/Theme.cpp`
3. Update the resource file (`resources.qrc`) for new assets
4. Follow the existing code style and architecture patterns

### Customization

#### Theming
- **Colors**: Modify the color palette in `src/Utils/Theme.cpp`
- **Fonts**: Update font files in the `fonts/` directory and `include/Utils/Font.hpp`
- **Icons**: Replace SVG files in the `icons/` directory

#### Layout
- **Widgets**: Modify widget classes in `include/Widget/`
- **Navigation**: Customize navigation in `include/Widget/Layouts/Nav/`
- **Sidebars**: Adjust sidebar behavior in `include/Widget/Layouts/`

#### Rendering
- **OpenGL**: Extend OpenGL renderer in `src/Engine/Renderer/OpenGL/`
- **Vulkan**: Add Vulkan features in `src/Engine/Renderer/Vulkan/`
- **Entities**: Create new renderable entities in `src/Engine/Renderer/OpenGL/Entities/`

### Build Configuration
The project uses QMake with automatic source discovery:
- All `.cpp` files in `src/` and subdirectories are automatically included
- All `.h` and `.hpp` files in `include/` and subdirectories are automatically included
- Build artifacts are organized in the `build/` directory
- Compiler flags include `-Wall -Wextra -Wpedantic` for strict compilation

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the GNU General Public License v2.0 - see the [LICENSE](LICENSE) file for details.

## Performance

### Rendering Performance
- **OpenGL 4.5**: Hardware-accelerated rendering with modern OpenGL features
- **Vulkan Support**: Next-generation graphics API for maximum performance
- **Entity System**: Efficient renderable entity management
- **Memory Management**: Optimized memory usage with Qt's object system

### Network Performance
- **libcurl**: High-performance HTTP client with connection pooling
- **SSL/TLS**: Hardware-accelerated cryptography with OpenSSL
- **Concurrent Processing**: Multi-threaded network operations

### Parsing Performance
- **Custom Parser**: Optimized HTML parsing without external dependencies
- **Tokenization**: Efficient token-based parsing system
- **DOM Tree**: Fast document object model construction

## Credits

- **Author**: Ömer Karakaş (Wyrex)
- **Framework**: Qt6 by The Qt Company
- **Mathematics**: GLM (OpenGL Mathematics) library
- **Fonts**: Nunito and Nano fonts
- **Graphics**: STB libraries for image and font processing
- **Network**: libcurl and OpenSSL libraries
- **License**: GNU GPL v2.0

## Acknowledgments

- Qt Community for the excellent cross-platform framework
- Open source contributors for inspiration and code examples
- Font creators for the beautiful typography
- Graphics programming community for rendering techniques
- Web standards organizations for HTML/CSS specifications

## Roadmap

### Current Status
- ✅ Basic UI framework with Qt6
- ✅ OpenGL 4.5 rendering engine
- ✅ HTML parsing and tokenization
- ✅ Network layer with libcurl
- ✅ Tab management system
- ✅ Theme system
- 🔄 Vulkan renderer (in development)
- 🔄 CSS parsing (planned)
- 🔄 JavaScript engine (planned)
- 🔄 Bookmark system (planned)
- 🔄 Download manager (planned)

### Future Features
- **Advanced Rendering**: WebGL support, CSS animations
- **Security**: Enhanced privacy features, ad blocking
- **Performance**: Multi-process architecture, hardware acceleration
- **Extensions**: Plugin system for custom functionality
- **Sync**: Cross-device synchronization

---

**Note**: Void Browser is currently in active development. The core engine is functional with basic web browsing capabilities. Advanced features like JavaScript execution and full CSS support are planned for future releases. Please report issues and contribute to help improve the project.
