# Contributing to NVDA SAPI Bridge

Thank you for considering contributing to the NVDA SAPI Bridge! This document provides guidelines for contributing.

## Code of Conduct

This project aims to help the blind and visually impaired community. Be respectful, inclusive, and helpful.

## How to Contribute

### Reporting Issues

If you encounter problems:

1. Check existing issues first
2. Provide detailed information:
   - Windows version
   - NVDA version
   - Application you're trying to use
   - Steps to reproduce
   - Error messages

### Suggesting Enhancements

Enhancement suggestions are welcome! Please:

1. Explain the use case
2. Describe the expected behavior
3. Consider backwards compatibility

### Code Contributions

#### Getting Started

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Make your changes
4. Test thoroughly
5. Submit a pull request

#### Coding Standards

- **C++ Standard**: C++17
- **Style**: Follow existing code style
- **Comments**: Add comments for complex logic
- **Documentation**: Update docs for user-facing changes

#### Code Guidelines

**Good Practices:**
- Use RAII for resource management
- Prefer smart pointers (`std::unique_ptr`, etc.)
- Check all pointers before dereferencing
- Return proper HRESULT codes
- Handle errors gracefully

**Avoid:**
- Raw `new`/`delete` (use smart pointers)
- Memory leaks
- Buffer overflows
- Unsafe string operations
- Breaking changes without discussion

#### Building and Testing

Before submitting:

1. **Build both architectures:**
   ```batch
   build.bat
   ```

2. **Test manually** (if you have access to Windows):
   - Register the DLL
   - Test with a SAPI5 application
   - Verify speech works through NVDA
   - Check both 32-bit and 64-bit builds

3. **Check for warnings:**
   - Code should compile without warnings
   - Use appropriate warning levels

#### Commit Messages

Write clear commit messages:

```
Short summary (50 chars or less)

More detailed explanation if needed. Wrap at 72 characters.
Explain what changed and why, not how (the code shows how).

- Bullet points are okay
- Reference issues: Fixes #123
```

#### Pull Request Process

1. Update README.md if needed
2. Update ARCHITECTURE.md for significant changes
3. Ensure the build passes
4. Request review from maintainers
5. Address review feedback

### Documentation Contributions

Documentation improvements are valuable:

- Fix typos and grammar
- Improve clarity
- Add examples
- Update for new features

### Testing Contributions

Help with testing:

- Test with different applications
- Try various Windows versions
- Report compatibility issues
- Suggest test cases

## Development Setup

### Windows

1. Install Visual Studio 2022
2. Install CMake
3. Clone the repository
4. Run `build.bat`

### Linux (Cross-Compilation)

1. Install MinGW-w64:
   ```bash
   sudo apt-get install mingw-w64
   ```
2. Clone the repository
3. Run `./build.sh`

## Project Structure

```
nvda-sapi/
├── src/                    # Source code
│   ├── nvda_sapi_bridge.*  # COM registration
│   ├── sapi_voice.*        # SAPI implementation
│   ├── nvda_client.*       # NVDA integration
│   └── sapi_minimal.h      # SAPI interface definitions
├── build.bat/sh            # Build scripts
├── CMakeLists.txt          # Build configuration
└── docs (README, etc.)     # Documentation
```

## Code Review Criteria

Pull requests are reviewed for:

- **Functionality**: Does it work as intended?
- **Code Quality**: Is it well-written and maintainable?
- **Documentation**: Are changes documented?
- **Testing**: Has it been tested?
- **Compatibility**: Does it maintain compatibility?

## Getting Help

- Open an issue for questions
- Discuss major changes before implementing
- Ask for clarification on review feedback

## Recognition

Contributors will be acknowledged in the project documentation.

## License

By contributing, you agree that your contributions will be licensed under the same terms as the project.

Thank you for helping make technology more accessible!
