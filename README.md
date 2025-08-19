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
   - Ensure the platform is set to **x64** (recommended) or **Win32**
   - Select **Debug** or **Release** configuration

3. **Build**
   - Build the solution using `Ctrl+Shift+B` or **Build > Build Solution**
   - The executable will be created in the appropriate configuration directory

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
- **Mouse** - Look around (first-person camera)
- **Mouse Scroll** - Zoom in/out
- **Left Click** - Interact with objects (e.g., light switches)
- **ESC** - Exit application

### Running the Application

1. Build the project successfully
2. Run the executable from Visual Studio or navigate to the output directory
3. Ensure the working directory is set correctly so the application can find the `objects/` and `textures/` folders

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

## Troubleshooting

### Common Issues

1. **Build Errors**
   - Ensure Visual Studio has C++ development tools installed
   - Check that the platform target (x64/Win32) matches your system

2. **Missing Textures/Models**
   - Verify the working directory is set to the project root
   - Ensure `objects/` and `textures/` directories are accessible

3. **OpenGL Errors**
   - Update graphics drivers
   - Verify OpenGL 3.3+ support on your graphics card

4. **Performance Issues**
   - Try Release configuration instead of Debug
   - Lower shadow map resolution if needed
   - Ensure graphics drivers are up to date

## Contributing

This project is part of a computer graphics coursework. If you'd like to contribute or use this as a learning resource:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

This project is created for educational purposes. Please respect any third-party asset licenses included in the `objects/` and `textures/` directories.

## Acknowledgments

- **GLFW** for window management
- **GLM** for mathematics operations
- **STB** for image loading
- Various texture and model assets used for demonstration purposes