# OpenGL Coffee Shop

A 3D coffee shop simulation built with OpenGL, featuring realistic lighting, shadows, and interactive elements. This project demonstrates advanced computer graphics techniques including Physically Based Rendering (PBR), shadow mapping, and real-time 3D scene management.

## Features

- **Realistic 3D Environment**: Detailed coffee shop interior with tables, chairs, counter, and equipment
- **Physically Based Rendering (PBR)**: Realistic materials with albedo, normal, roughness, and metallic textures
- **Advanced Lighting**: Multiple light sources with directional and point light shadow mapping
- **Interactive Elements**: Clickable light switches and other interactive objects
- **Real-time Shadows**: Both directional light shadows and cube map shadows for point lights
- **Camera Controls**: Free-look camera with keyboard and mouse controls
- **Animation System**: Smooth object animations and transformations

## Demo

<!-- TODO: Add demo GIF or video here -->
*Demo GIF/video coming soon - showcase the 3D coffee shop environment, lighting effects, and interactive features*

## Prerequisites

- **Windows Operating System**
- **Visual Studio 2017 or later** (with C++ development tools)
- **OpenGL 3.3+ compatible graphics card**

## Dependencies

The project includes all necessary dependencies in the repository:

- **GLFW 3.4** - Window management and input handling
- **GLM** - OpenGL Mathematics library for matrix operations
- **GL3W** - OpenGL loader
- **STB Image** - Image loading library for textures

## Installation

### Clone the Repository

```bash
git clone https://github.com/andriskokins/opengl-coffee-shop.git
cd opengl-coffee-shop
```

### Build Instructions

1. **Open the Solution**
   - Navigate to the `Assessment2` directory
   - Open `Assessment2.sln` in Visual Studio

2. **Configure the Project**
   - Ensure the platform is set to **x64**
   - Select **Debug** configuration

3. **Build**
   - Start Without Debugging `Ctrl+F5` or **Debug > Start Without Debugging**

### Directory Structure

```
opengl-coffee-shop/
├── Assessment2/
│   ├── Assessment2.sln          # Visual Studio solution file
│   ├── Assessment2/
│   │   ├── Assessment2.cpp      # Main application source
│   │   ├── *.vert, *.frag       # Shader files (PBR, shadows)
│   │   └── Assessment2.vcxproj  # Visual Studio project file
│   ├── objects/                 # 3D model files (.obj)
│   └── textures/                # Texture files (.png, .jpg)
├── include/                     # Project headers and third-party libraries
│   ├── *.h                      # Custom headers (camera, lighting, etc.)
│   ├── glm/                     # GLM mathematics library
│   ├── GLFW/                    # GLFW headers
│   └── stb_image.h              # STB image loading
├── lib/                         # GLFW binaries for Windows
└── README.md                    # This file
```

## Usage

### Controls

- **W/A/S/D** - Move camera forward/left/backward/right
- **Left CTRL** - Crouch
- **G** - Toggle between fly through and first person camera
- **Mouse** - Look around (first-person camera)
- **Mouse Scroll** - Zoom in/out
- **Left Click** - Interact with objects (e.g., light switches)
- **ESC** - Exit application

## Technical Details

### Graphics Features

- **Shadow Mapping**: Implements both 2D shadow maps for directional lights and cube shadow maps for point lights
- **PBR Shading**: Uses albedo, normal, roughness, metallic, and ambient occlusion textures
- **Multi-pass Rendering**: Separate shadow generation and main rendering passes
- **Transparency**: Alpha blending for transparent objects

### Asset Pipeline

- **3D Models**: .obj files loaded using custom parser
- **Textures**: Various formats supported through STB Image
- **Materials**: PBR workflow with multiple texture maps per material

## Acknowledgments
      
- **GLFW** for window management
- **GLM** for mathematics operations
- **STB** for image loading
- Various texture and model assets used for demonstration purposes
- University of Nottingham for the coursework opportunity
