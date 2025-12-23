# Pacman

## Required Libraries

* **SDL3**
* **SDL3_image**

## Setup Instructions

1. Create an `external` folder in the project root if it doesn’t exist:

   ```bash
   mkdir -p external
   ```

2. Download the libraries and place them in the `external` folder:

   ```
   external/SDL3
   external/SDL3_image
   ```

   Make sure each folder contains the full library source.

3. Configure and build with CMake:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ./PacmanClone
   ```