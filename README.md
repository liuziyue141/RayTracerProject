
# Raytracer

## Contributor
- **Name:** Ziyue Liu
- **Name:** Aarav Tiku 

## Development Environment
- **Platform:** MacOS
- **Build System:** Make

## Build & Run Instructions
### Compilation
```bash
make
```

### Running the Program
```bash
./tranforms testscenes/scene1.test  # For scene 1
```

## Test Scenes
We have implemented three test scenes for this milestone submission:

### Directory Structure
```
./scene1-milestone/
    Camera-1/
    ...
./scene2-milestone/
    Camera-1/
    ...
./scene3-milestone/
    Camera-1/
    ...
```

Each scene directory contains multiple camera test positions to validate different viewing angles and rendering capabilities.

## Implementation Details
For this milestone submission, we have completed:
- Camera implementation and positioning
- Basic ray-surface intersections:
  - Triangle intersection
  - Sphere intersection
- Basic transformations
