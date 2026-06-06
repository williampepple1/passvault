<p align="center">
  <img src="resources/banner.png" alt="PassVault Banner">
</p>

# PassVault

PassVault is a secure, modern, local-only password manager desktop application built with C++ and Qt. It uses strong AES-256 encryption to keep your credentials safe without ever sending data over the internet.

## Features

- **Local Storage**: Your data never leaves your machine.
- **AES-256 Encryption**: Vaults are encrypted using an embedded AES implementation.
- **Recovery Key System**: Dual-key encryption allows you to recover your vault using an auto-generated Recovery Key if you forget your Master Password.
- **Modern Interface**: A sleek, user-friendly Qt interface featuring a customizable Dark and Light mode.
- **One-Click Installer**: A standalone GUI installer that extracts and sets up the application easily on Windows.

## Getting Started

### Using the Installer (Windows)

The easiest way to use PassVault is via the pre-compiled installer included in the repository:

1. Download the `installer.exe` file from this repository.
2. Ensure `installer.zip` is in the same directory (if downloading manually).
3. Run `installer.exe` and click **Install**. The application will be extracted to a `PassVault` folder on your Desktop.

### Building from Source

If you prefer to compile PassVault yourself, you will need:
- **C++17 Compiler** (e.g., MinGW or MSVC)
- **CMake** (v3.16+)
- **Qt 6** (Core, Gui, Widgets)

**Build Instructions:**

```bash
git clone https://github.com/williampepple1/passvault.git
cd passvault
cmake -B build -S .
cmake --build build
```

## First-Time Setup

1. Launch `PassVault.exe`.
2. You will be prompted to create a **Master Password**. Make it strong!
3. The app will generate a **Recovery Key**. **Save this key in a secure place!** It is the only way to recover your data if you forget your Master Password.

## Security Warning

This application is strictly local. There is no cloud backup or email reset functionality. If you lose *both* your Master Password and your Recovery Key, your data is mathematically unrecoverable. 

## License

This project uses the open-source `QAESEncryption` library for symmetric encryption.
