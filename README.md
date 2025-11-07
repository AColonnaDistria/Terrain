# Procedural Terrain Generation with SFML

This project implements procedural terrain generation using the **Midpoint Displacement Algorithm** (a variation of the Diamond-Square algorithm) and visualizes the results using the **SFML (Simple and Fast Multimedia Library)**. It displays the terrain in two views: a **top-down 2D representation** and an **isometric 3D-like representation**.

---

## Features

* **Procedural Generation:** Generates a heightmap using the Midpoint Displacement algorithm to create dynamic terrains. The algorithm starts by setting the four corners to random values and then iteratively calculates midpoints, adding a random spread.
* **SFML Graphics:** Utilizes SFML for window management, 2D graphics, and rendering.
* **Dual View:** Simultaneous display of:
    * A **Top-Down (2D)** view, where each square's color is mapped directly to its height.
    * An **Isometric** view, rendered using `sf::ConvexShape` and projected onto an `sf::RenderTexture`.
* **Color-to-Height Mapping:** Heights are mapped to colors: blue for low values (water), and shades of green for higher values, simulating elevation.
* **Normalization:** The generated heights are normalized to fit within a specific range (`-9.0` to `9.0` after normalization) to ensure consistent visualization.

---

## 🛠️ Project Structure

| File | Description |
| :--- | :--- |
| `main.cpp` | Main application entry point. Initializes SFML, creates the `Terrain` (size 65), sets up the 2D view and the `IsometricRenderer`, and handles the main game loop. |
| `terrain.hpp` / `terrain.cpp` | Defines the `Terrain` class, responsible for holding the heightmap (`std::vector<std::vector<double>>`) and implementing the **Midpoint Displacement** generation algorithm (`midpointDisplacement()`). |
| `iso.hpp` / `iso.cpp` | Defines the `IsometricRenderer` class, which is a subclass of `sf::RenderTexture`. It handles the isometric projection logic and renders the terrain using `sf::ConvexShape` tiles. |
| `common.hpp` / `common.cpp` | Provides utility functions, including `colorFromHeight(double height)` (maps height values to SFML colors) and `randomDouble(double min, double max)` (generates random numbers). |

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

### Isometric Projection

The `IsometricRenderer` uses the following parameters for projection:

* **Origin:** Set near the top-center of the rendering area (`width / 2`, `height / 8`).
* **Basis Vectors (scaled by 0.5):**
    * `e0` (X-axis/I index): `(15.0f, 9.0f)`
    * `e1` (Y-axis/J index): `(-15.0f, 9.0f)`
    * `e2` (Z-axis/Height): `(0.0f, -9.0f)`

The function `getIsometricPoint(double x, double y, double z)` performs the transformation:

$$
\text{Screen Point} = \text{Origin} + (x \cdot \mathbf{e_0}) + (y \cdot \mathbf{e_1}) + (z \cdot \mathbf{e_2})
$$

---

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

The application will launch an SFML window displaying the 2D flat heightmap on the left and the procedurally generated isometric terrain on the right.
