# Multimedia_Systems_Uni_Project

## Table of contents
* [General info](#general-info)
* [TODO](#todo)
* [Setup](#setup)
* [Examples](#examples)
* [Technologies](#technologies)

## General info
The goal of the project is to extend the raster graphics file specification prepared earlier in Computer Graphics that captures color (using n imposed and n dedicated colors) and grayscale (using n grayscale steps) images to support 15-bit, 16-bit and 24-bit modes using:
* various color spaces (RGB, YUV, YIQ, YCbCr, HSV),
* predictive algorithms,
* lossless compression (ByteRun, RLE, LZW, LZ77),
* lossy compression ( DCT  transform + component subsampling),
* the application should convert from a BMP file to a new type of image file and from a new type of file to BMP format with the possibility of the user to indicate the color mode, color space, prediction algorithm and type of compression (lossy or lossless).

**_NOTE:_**  Project is still under development

## TODO
- [x] various color spaces (RGB, YUV, YIQ, YCbCr, HSV),
- [ ] predictive algorithms,
- [ ] lossless compression (ByteRun, RLE, LZW, LZ77),
- [ ] lossy compression ( DCT  transform + component subsampling),
- [ ] app interface
- [ ] bmp <-> new format

## Setup

1. Clone the project
```
git clone https://github.com/VoxeveR/Multimedia_Systems_Uni_Project.git
```
2. Build project and run it
```
make run
```
**_NOTE:_** VS Code is prefered editor for this project.

## Examples

(filling that later)

## Technologies
- C++
- [SDL 2.0](https://wiki.libsdl.org/SDL2/Introduction)
