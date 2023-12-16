# Seam carving

## Overview

Seam carving is a content-aware image resizing technique that dynamically adjusts the size of an image by identifying and removing or duplicating seams—continuous paths of pixels with the least importance. Unlike traditional resizing methods, seam carving preserves the important content in an image while minimizing distortion in crucial areas.

## Working of algorithm

- A vertical seam in an image is a path of pixels connected from the top to the bottom with one pixel in each row. A horizontal seam is a path of pixels connected from the left to the right with one pixel in each column.
- Energy Calculation: Each pixel has some RGB values. Using dual-gradient energy function, energy for each pixel is calculated.
- The lowest energy seam is identified and removed. Number of seams that needs to be removed depends upon the change in breadth and height of the image required to crop it.

## Program flow

- First  individual pixel’s RGB values are extracted from the sample image in `./data/input/sample.jpeg` and written into `./data/input/rgb_in.txt`.
- RGB values from `./data/input/rgb_in.txt` is loaded into a 3D matrix.
- Seam carving algorithm is applied.
- The RGB values for the resized image is written into `./data/output/rgb_out.txt`.
- Using above RGB values, the final image is generated in `./data/output/sample_out.jpeg`.

## Dependencies

- You’ll need to install the python image library Pillow to extract RGB values of each pixel and to generate images back from RGB values of each pixel.
- To install Pillow, write `pip3 install Pillow` onto terminal. The python script is only compatible with Linux/Mac operating systems.

## How to run

- Open `Seam Carving` directory in the terminal.
- The input image file must be present inside `./data/input`.
- Run the `driver.py` file located in `./src` folder. You nee to pass the input image filename as parameter. Ex - **`python3 ./src/driver.py sample1.jpeg`.**
- The output image will be generated in `./data/output` folder with name as `sample_out.jpeg`.