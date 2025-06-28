//
// NESkvik - Fredrik Ofstad
//

#include "framebuffer.h"
#include <cstring>
#include <stdexcept>

Framebuffer::Framebuffer(SDL_Renderer* renderer, int width, int height)
    : m_renderer(renderer), m_width(width), m_height(height)
{
    // Lag en SDL-texture som framebuffer
    m_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    if (!m_texture) {
        throw std::runtime_error("Failed to create framebuffer texture");
    }

    // allocating for pixels
    m_pixels.resize(width * height, 0xFF000000); // svart med full alpha
}


Framebuffer::~Framebuffer()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

void Framebuffer::clear(uint32_t color)
{
    std::fill(m_pixels.begin(), m_pixels.end(), color);
}

void Framebuffer::setPixel(int x, int y, uint32_t color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
    m_pixels[y * m_width + x] = color;
}

void Framebuffer::update()
{
    SDL_UpdateTexture(m_texture, nullptr, m_pixels.data(), m_width * sizeof(uint32_t));
}

void Framebuffer::render(int x, int y, int scale)
{
    SDL_Rect dst = {
        x,
        y,
        m_width * scale,
        m_height * scale
    };
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &dst);
}