# Procedural Terrain Generation with SFML

This project implements procedural terrain generation using the **Midpoint Displacement Algorithm** (a variation of the Diamond-Square algorithm) and visualizes the results using the **SFML** library. 
It displays the terrain in two views: a top-down 2D representation and an isometric 3D-like representation.

---

## Features

- Procedural Generation: Generates a heightmap using the Midpoint Displacement algorithm to create dynamic terrains. 
The algorithm starts by setting the four corners to random values and then iteratively calculates midpoints, adding a random spread.
- Dual View: Simultaneous display of:
    * A top-Down (2D) view, where each square's color is mapped directly to its height.
    * An isometric view.
- Color-to-Height Mapping: Heights are mapped to colors: blue for low values (water), and shades of green for higher values, simulating elevation.
- Moving the map: using the left, right, up and down keys
- Increasing/Decreasing map height: Left click increases the height and right click decreases it

---

## Key Implementation Details

### Midpoint Displacement Algorithm

The `Terrain::midpointDisplacement()` function is the core generation method:

1.  **Initialization:** The four corners of the heightmap are set to a random height between -10.0 and 10.0.
2.  **Iteration:** A `while` loop runs as long as the current square size `r` is 2 or greater. The size `r` is halved in each step.
3.  **Diamond/Square Step:** The `midpointCenter(int x, int y, int r, double spread)` function calculates:
    * The **midpoint edges** by averaging the two adjacent corner heights and adding a random offset determined by `spread`.
    * The **center point** by averaging the four newly calculated midpoint edge heights and adding a random offset.
4.  **Damping:** The `spread` factor is multiplied by `0.5` in each outer iteration to reduce the magnitude of random changes, leading to smoother results at smaller scales.
5.  **Normalization and Bias:** After the loop, `normalize()` scales the heightmap values to be between -9.0 and 9.0. A fixed `biais` (bias) of 2.5 is then added to lift the terrain.

## How to Build and Run

1.  **Dependencies:** Ensure you have a C++ compiler and the **SFML** library (specifically the `graphics`, `window`, and `system` modules) installed and configured.
2.  **Compilation (Example using g++):**
    ```bash
    g++ main.cpp terrain.cpp iso.cpp common.cpp -o terrain_gen -lsfml-graphics -lsfml-window -lsfml-system
    ```
3.  **Execution:**
    ```bash
    ./terrain_gen
    ```
    
## Video

[![Watch the video](https://img.youtube.com/vi/OWB4mL2h0ps/maxresdefault.jpg)](https://youtu.be/OWB4mL2h0ps)
