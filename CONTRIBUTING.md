# Contributing to Void Browser

Thank you for your interest in contributing to Void Browser! This document provides guidelines and information for contributors.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Contributing Guidelines](#contributing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Issue Reporting](#issue-reporting)
- [Development Areas](#development-areas)
- [Code Style](#code-style)
- [Testing](#testing)
- [Documentation](#documentation)

## Code of Conduct

This project adheres to a code of conduct that we expect all contributors to follow:

- Be respectful and inclusive
- Welcome newcomers and help them learn
- Focus on constructive feedback
- Respect different viewpoints and experiences
- Show empathy towards other community members

## Getting Started

**Current Status**: Void Browser is currently in Pre-0.1 development phase. The core engine is functional with basic web browsing capabilities, but many features are still in development.

### Prerequisites

Before contributing, ensure you have:

- **Qt6**: Qt6 framework with required modules
- **C++20 Compiler**: GCC 10+, Clang 12+, or MSVC 2019+
- **libcurl**: HTTP client library
- **OpenSSL**: SSL/TLS library
- **Git**: Version control system
- **CMake/QMake**: Build system

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:
```bash
git clone https://github.com/wyrexdev/void.git
cd Void
```

3. Add the upstream repository:
```bash
git remote add upstream https://github.com/wyrexdev/void.git
```

## Development Setup

### Building the Project

1. **Install Dependencies** (see README.md for platform-specific instructions)

2. **Generate Makefile**:
```bash
qmake6 Void.pro
```

3. **Build the Application**:
```bash
make
```

4. **Run the Application**:
```bash
./Void
```

### Development Workflow

1. **Create a Feature Branch**:
```bash
git checkout -b feature/your-feature-name
```

2. **Make Your Changes**:
   - Follow the code style guidelines
   - Add tests for new functionality
   - Update documentation as needed

3. **Test Your Changes**:
```bash
make && ./Void
```

4. **Commit Your Changes**:
```bash
git add .
git commit -m "Add: Brief description of your changes"
```

5. **Push to Your Fork**:
```bash
git push origin feature/your-feature-name
```

6. **Create a Pull Request** on GitHub

## Contributing Guidelines

### Types of Contributions

We welcome various types of contributions:

#### 🐛 Bug Fixes
- Fix existing issues
- Improve error handling
- Resolve memory leaks
- Fix rendering issues

#### ✨ New Features
- Add new UI components
- Implement new parsing capabilities
- Add rendering features
- Enhance network functionality

#### 📚 Documentation
- Improve README.md
- Add code comments
- Create tutorials
- Update API documentation

#### 🧪 Testing
- Add unit tests
- Improve test coverage
- Add integration tests
- Performance testing

#### 🎨 UI/UX Improvements
- Enhance themes
- Improve accessibility
- Add new icons
- Optimize layouts

### Contribution Areas

#### Engine Development
- **Parser Engine** (`src/Engine/Parser/`):
  - HTML parsing improvements
  - CSS parsing implementation
  - JavaScript parsing (future)
  - Performance optimizations

- **Renderer Engine** (`src/Engine/Renderer/`):
  - OpenGL rendering enhancements
  - Vulkan renderer completion
  - Text rendering improvements
  - Graphics optimizations

- **Request Engine** (`src/Engine/Request/`):
  - HTTP/2 support
  - WebSocket implementation
  - Caching mechanisms
  - Security enhancements

#### UI Development
- **Widget System** (`include/Widget/`):
  - New widget components
  - Layout improvements
  - Event handling enhancements
  - Accessibility features

- **Theme System** (`src/Utils/Theme.cpp`):
  - New color schemes
  - Dark/light mode toggle
  - Custom theme support
  - Theme editor

#### Utility Systems
- **History Management** (`src/Utils/History.cpp`):
  - Bookmark system
  - History search
  - Data persistence
  - Privacy controls

- **Settings System** (`include/Utils/Settings.hpp`):
  - Configuration management
  - User preferences
  - Import/export settings
  - Advanced options

## Pull Request Process

### Before Submitting

1. **Check Existing Issues**: Ensure your contribution addresses an existing issue or adds value
2. **Test Thoroughly**: Test your changes on multiple platforms if possible
3. **Update Documentation**: Update relevant documentation
4. **Follow Code Style**: Adhere to the project's coding standards

### Pull Request Template

When creating a pull request, please include:

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Performance improvement
- [ ] Code refactoring

## Testing
- [ ] Tested on Windows
- [ ] Tested on Linux
- [ ] Tested on macOS
- [ ] Added unit tests
- [ ] Manual testing completed

## Screenshots (if applicable)
Add screenshots to help explain your changes

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No breaking changes (or documented)
```

### Review Process

1. **Automated Checks**: CI/CD pipeline will run tests
2. **Code Review**: Maintainers will review your code
3. **Feedback**: Address any feedback or requested changes
4. **Approval**: Once approved, your PR will be merged

## Issue Reporting

### Bug Reports

When reporting bugs, please include:

- **OS and Version**: Operating system and version
- **Browser Version**: Void Browser version (currently Pre-0.1)
- **Steps to Reproduce**: Clear steps to reproduce the issue
- **Expected Behavior**: What should happen
- **Actual Behavior**: What actually happens
- **Screenshots**: If applicable
- **Logs**: Any error messages or logs

### Feature Requests

For feature requests, please include:

- **Use Case**: Why this feature would be useful
- **Proposed Solution**: How you envision it working
- **Alternatives**: Other solutions you've considered
- **Additional Context**: Any other relevant information

## Development Areas

### High Priority

- **Vulkan Renderer**: Complete the Vulkan rendering implementation
- **CSS Parser**: Implement CSS parsing and styling
- **JavaScript Engine**: Add JavaScript execution support
- **Security**: Enhance security features and privacy controls

### Medium Priority

- **Performance**: Optimize rendering and parsing performance
- **Accessibility**: Improve accessibility features
- **Internationalization**: Add multi-language support
- **Extensions**: Plugin system for custom functionality

### Low Priority

- **Mobile Support**: Adapt for mobile platforms
- **Advanced Features**: Developer tools, debugging features
- **Integration**: Better OS integration

## Code Style

### C++ Guidelines

- **Standard**: Use C++20 features appropriately
- **Naming**: Use camelCase for variables, PascalCase for classes
- **Indentation**: Use 4 spaces for indentation
- **Comments**: Document complex logic and public APIs
- **Headers**: Use `#pragma once` for header guards

### Qt Guidelines

- **Memory Management**: Use Qt's parent-child system
- **Signals/Slots**: Use modern Qt5/Qt6 syntax
- **Widgets**: Inherit from base `Widget` class when possible
- **Resources**: Use Qt Resource System for assets

### Example Code Style

```cpp
// Good example
class MyWidget : public Widget {
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    
    void setCustomProperty(const QString &value);
    QString getCustomProperty() const;

private slots:
    void onButtonClicked();

private:
    void initializeUI();
    void setupConnections();
    
    QString m_customProperty;
    QPushButton *m_button;
};
```

## Testing

### Unit Testing

- Add unit tests for new functionality
- Test edge cases and error conditions
- Maintain good test coverage

### Integration Testing

- Test component interactions
- Verify end-to-end functionality
- Test on multiple platforms

### Manual Testing

- Test UI changes thoroughly
- Verify rendering on different hardware
- Test network functionality

## Documentation

### Code Documentation

- Document public APIs
- Add inline comments for complex logic
- Use Doxygen-style comments for classes

### User Documentation

- Update README.md for new features
- Add usage examples
- Create tutorials for complex features

### Developer Documentation

- Document architecture decisions
- Explain design patterns used
- Provide setup and build instructions

## Getting Help

### Community

- **GitHub Issues**: For bug reports and feature requests
- **Discussions**: For questions and general discussion
- **Discord/Slack**: Real-time chat (if available)

### Resources

- **Qt Documentation**: https://doc.qt.io/
- **C++ Reference**: https://en.cppreference.com/
- **OpenGL Documentation**: https://www.opengl.org/documentation/
- **Vulkan Documentation**: https://www.khronos.org/vulkan/

## Recognition

Contributors will be recognized in:

- **CONTRIBUTORS.md**: List of all contributors
- **Release Notes**: Credit for significant contributions
- **GitHub**: Contributor statistics and activity

## License

By contributing to Void Browser, you agree that your contributions will be licensed under the GNU General Public License v2.0.

---

Thank you for contributing to Void Browser! Your efforts help make this project better for everyone.
