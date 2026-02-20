# Prototype-Raylib-3D
Trying to create a 3D game with Raylib


## Technical Stack

- **Language**: C++ (C++17 or higher)
- **Build System**: CMake 3.15+
- **Package Manager**: CPM (C++ Package Manager)
- **Graphics Library**: Raylib

## Building the Project

### Linux

```bash
# Clone the repository
git clone <repository-url> Prototype-Raylib
cd Prototype-Raylib

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCPM_SOURCE_CACHE=$HOME/.cache/CPM --log-level=STATUS

# Build
make

# You can execute the binarie using these 2 command
./PrototypeRaylib
```
