# Computer Graphics Project
It is my academic project of Computer Graphics course

## Dynamic City Lake View

A C++ OpenGL/GLUT computer graphics project showing a dynamic city lake scene with vehicles, boat movement, rain, lighting, clouds, and ferris wheel animation.

## Features

- Dynamic city and lake view
- Moving cars and boat
- Ferris wheel animation
- Clear sky and overcast/rainy weather mode
- Building and street light effects
- Interactive keyboard and mouse controls

## Controls

### Vehicle Controls

| Key | Action |
|---|---|
| `C` | Start car movement |
| `S` | Stop car movement |
| `B` | Start boat movement |
| `N` | Stop boat movement |

### Ferris Wheel Controls

| Input | Action |
|---|---|
| Left Mouse Click | Start anticlockwise rotation |
| Right Mouse Click | Stop rotation |
| `H` | Increase speed |
| `L` | Decrease speed |

### Environment Controls

| Key | Action |
|---|---|
| `1` | Overcast/rainy scenario with lights on |
| `2` | Clear sky scenario with lights off |

## Requirements

- Code::Blocks IDE
- MinGW/GCC compiler
- OpenGL
- GLUT/freeglut

## How to Run

1. Open `City_Lake_View.cbp` in Code::Blocks.
2. Make sure OpenGL and GLUT/freeglut are installed and linked.
3. Build and run the project.

## Project Structure

```text
Computer-Graphics-Project/
├── README.md
├── .gitignore
├── City_Lake_View.cbp
├── main.cpp
└── docs/
    └── geogebra-export.ggb
```

## Notes

Generated build files such as `bin/`, `obj/`, `.exe`, `.o`, `.depend`, and `.layout` are ignored because they can be recreated after building the project.
