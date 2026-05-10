# SolarSystemExplorer

A real-time interactive 3D solar system simulation built with OpenGL 3.3 (Core Profile) in C++, developed as the final project for COMP 371 — Computer Graphics at Concordia University.

---

## Authors

| Name | Student ID |
|---|---|
| Nesrine Larbi | 40079009 |
| Aditi Abhaysingh Ingle | 40266449 |
| Hanad-Keysse Mohamed Hassan | 40299566 |

---

## Overview

SolarSystemExplorer renders a fully animated solar system in real time. Every planet orbits the Sun, moons orbit their parent planets using hierarchical transformation matrices, and the entire scene is lit by a dynamic Phong lighting system driven by per-planet point lights. A two-pass shadow mapping pipeline casts soft shadows from moons onto planets. The user can fly freely through the scene using a first-person camera.

---

## Features

### Rendering
- Procedurally generated UV spheres for all celestial bodies
- Real planet surface textures (JPG/PNG) loaded via `stb_image`
- Space skybox rendered as an OpenGL cubemap
- OBJ model loading for the satellite and rocket

### Lighting & Shadows
- Full Phong shading model — ambient, diffuse, and specular components
- Up to 8 simultaneous light sources (directional, point, and spotlight types)
- Per-planet point lights positioned between the Sun and each planet
- Light intensity scales with the inverse-square law — closer planets are brighter
- Two-pass shadow mapping with a depth framebuffer and orthographic light projection
- PCF (Percentage Closer Filtering) for soft shadow edges
- Runtime toggle for all lighting and shadows (P key)

### Animation
- All 8 planets + Pluto orbit the Sun at independently tuned speeds and radii
- Earth's Moon and Mars's two moons orbit their parent planets
- Hierarchical animation: moon positions are computed relative to their parent planet's current world position each frame
- Venus rotates retrograde (backwards), Jupiter and Saturn spin fast — physically motivated values throughout
- Satellite with togglable spin; rocket with directional controls

### Camera
- First-person free-fly camera
- Mouse look with yaw/pitch control and pitch clamping to prevent gimbal lock
- Movement relative to the camera's viewing direction

---

## Solar System Contents

| Body | Orbital Radius | Notes |
|---|---|---|
| Sun | — | Stationary, scale 20, emissive glow shader |
| Mercury | 50 | Fastest orbit |
| Venus | 80 | Retrograde rotation |
| Earth | 100 | Has one Moon |
| Mars | 150 | Has two moons (Phobos & Deimos approximated) |
| Jupiter | 200 | Largest planet, fastest rotation |
| Saturn | 250 | |
| Uranus | 300 | |
| Neptune | 350 | |
| Pluto | 400 | Slowest orbit |

---

## Controls

| Input | Action |
|---|---|
| W / A / S / D | Fly forward, left, backward, right |
| Mouse | Look around |
| Arrow Keys | Move the rocket |
| Space | Toggle satellite spin |
| L | Toggle light position visualizers |
| P | Toggle dynamic lighting and shadows |
| ESC | Exit |

---

## Architecture

The project is structured as a set of modular C++ classes:

```
CelestialBody       Base class: sphere generation, texture loading, orbit math, lighting/shadow rendering
  Planet            Adds texture path and scale; base for all planets and moons
    Earth, Mars,    One class per body with hardcoded orbital and rotation properties
    Venus, ...
    Moon            Accepts a parent pointer for hierarchical orbit computation
  Sun               Overrides update() to stay fixed at the origin; uses a custom glow shader

SceneObject         Base for non-planet scene objects
  Skybox            Cubemap-textured box rendered at infinite depth

Scene               Main controller: render loop, input handling, lighting setup, shadow pass
Camera              First-person fly camera with mouse look
LightManager        Manages up to 8 lights, uploads them to shaders as a uniform array
ShadowMap           Depth FBO, light-space matrix calculation, two-pass shadow rendering
LightVisualizer     Renders small cubes at each light position for debugging
Shader              Loads and compiles .vert / .frag files from disk, provides uniform setters
```

---

## Dependencies

| Library | Purpose |
|---|---|
| [OpenGL 3.3](https://www.opengl.org/) | Graphics API |
| [GLFW](https://www.glfw.org/) | Window creation and input |
| [GLEW](http://glew.sourceforge.net/) | OpenGL extension loading |
| [GLM](https://github.com/g-truc/glm) | Math (vectors, matrices) |
| [stb_image](https://github.com/nothings/stb) | Texture loading (JPG, PNG, BMP) |
| OBJ Loader | Loads `.obj` models for the satellite and rocket |

---

## Build & Run

### macOS (VS Code)

This project includes a `.vscode/tasks.json` for building on macOS with clang++.

1. Clone the repository:
   ```bash
   git clone https://github.com/Nesrine-larbi/COMP-371.git
   cd COMP-371
   ```

2. Install dependencies via Homebrew:
   ```bash
   brew install glfw glew glm
   ```

3. Open the project in VS Code and run the build task:
   ```
   Terminal > Run Build Task (Cmd+Shift+B)
   ```

4. Run the executable from the project root (so shader and asset paths resolve correctly):
   ```bash
   ./SolarSystem
   ```

> The executable must be run from the project root directory. Shaders are loaded from `shaders/` and textures from `assets/` using relative paths.

---

## Project Structure

```
COMP-371/
├── assets/             Planet textures, skybox images, OBJ models
├── include/            Header files for all classes
├── shaders/            GLSL vertex and fragment shaders
│   ├── celestial_body.vert / .frag     Phong lighting with shadow support
│   ├── shadow_map.vert / .frag         Depth pass for shadow mapping
│   ├── sun.vert / .frag                Emissive glow shader for the Sun
│   ├── skybox.vert / .frag             Cubemap skybox
│   └── object.vert / .frag             OBJ model rendering
├── src/                Implementation files
└── main.cpp            Entry point
```

---

## Course

**COMP 371 — Computer Graphics**
Concordia University, Montreal
