//
// NESkvik - Fredrik Ofstad
//

#pragma once

#include <cstdint>
#include <SDL.h>

#include <SDL.h>
#include <vector>
#include <cstdint>

class Framebuffer {
public:
    Framebuffer(SDL_Renderer* renderer, int width, int height);
    ~Framebuffer();

    void clear(uint32_t color = 0xFF000000); // default: opaque black
    void setPixel(int x, int y, uint32_t color);
    void update();  // Copy pixel buffer to texture
    void render(int x, int y, int scale = 1);  // Draw texture to screen

    SDL_Texture* getTexture() const { return m_texture; }

    int width() const { return m_width; }
    int height() const { return m_height; }

    std::vector<uint32_t>& pixels() { return m_pixels; }

private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    int m_width;
    int m_height;
    std::vector<uint32_t> m_pixels;
};