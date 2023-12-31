// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2023 42yeah <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "image.h"
#include <cassert>
#include <cstring>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

int id = 0;

Image::Image() : w(0), h(0), ch(4), image(nullptr), id_(::id++), initialized(false)
{

}

Image::Image(int w, int h, int ch) : w(w), h(h), ch(ch), image(nullptr), id_(::id++), initialized(false)
{
    assert((ch == 3 || ch == 4) && "Unsupported numer of channels");
    image.reset(new CComp[w * h * ch]);
    initialized = true;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            for (int i = 0; i < ch; i++)
            {
                image[at(x, y) + i] = 0;
            }
        }
    }
}

unsigned int Image::at(int x, int y) const
{
    assert(initialized && "Image is not initialized");

    if (x < 0) { x = 0; }
    if (y < 0) { y = 0; }
    if (x >= w) { x = w; }
    if (y >= h) { y = h; }
    return (y * w + x) * ch;
}

void Image::set_rgb(int x, int y, CComp r, CComp g, CComp b)
{
    assert(initialized && "Image is not initialized");

    image[at(x, y) + 0] = r;
    image[at(x, y) + 1] = g;
    image[at(x, y) + 2] = b;

    if (ch == 4)
    {
        image[at(x, y) + 3] = 255;
    }
}

void Image::set_rgb(int x, int y, const RGB &rgb)
{
    assert(initialized && "Image is not initialized");

    set_rgb(x, y, rgb.r, rgb.g, rgb.b);
}

RGB Image::get_rgb(int x, int y) const
{
    assert(initialized && "Image is not initialized");

    RGB ret;
    ret.r = image[at(x, y) + 0];
    ret.g = image[at(x, y) + 1];
    ret.b = image[at(x, y) + 2];
    return ret;
}

Image::Image(const Image& other)
{
    w = other.w;
    h = other.h;
    ch = other.ch;
    initialized = other.initialized;
    std::memcpy(image.get(), other.image.get(), sizeof(CComp) * w * h * ch);
}

bool Image::save(const std::string &dest) const
{
    assert(initialized && "Image is not initialized");

    stbi_flip_vertically_on_write(true);
    int res = stbi_write_png(dest.c_str(), w, h, ch, image.get(), w * ch);

    return res != 0;
}

bool Image::save_compressed(const std::string &dest, int quality) const
{
    assert(initialized && "Image is not initialized");

    stbi_flip_vertically_on_write(true);
    int res = stbi_write_jpg(dest.c_str(), w, h, ch, image.get(), quality);

    return res != 0;
}

Image::~Image()
{

}

CComp ccomp(float t)
{
    CComp c = (CComp) (t * 255.0f);
    return c;
}

std::shared_ptr<Image> generate_gradient_image(int w, int h)
{
    std::shared_ptr<Image> ret = std::make_shared<Image>(w, h, 4);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            float r = ((float) x / w);
            float g = ((float) y / h);
            ret->set_rgb(x, y, ccomp(r), ccomp(g), 0);
        }
    }
    return ret;
}

int Image::id() const
{
    return id_;
}

bool Image::load(const std::string &path)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(path.c_str(), &w, &h, &ch, 0);
    assert((ch == 3 || ch == 4) && "Unsupported number of channels");

    if (!data)
    {
        return false;
    }

    image.reset(data);
    initialized = true;

    return true;
}

bool Image::resize(int nx, int ny)
{
    // 1. Allocate needed amount of memory
    std::unique_ptr<unsigned char[]> ptr(new unsigned char[nx * ny * ch]);
    int ok = stbir_resize_uint8(image.get(), w, h, 0, ptr.get(), nx, ny, 0, ch);

    if (ok == 0)
    {
        return false;
    }

    w = nx;
    h = ny;
    image = std::move(ptr);
    return true;
}
