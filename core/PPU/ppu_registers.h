#pragma once
#include <cstdint>

union Status {
    struct {
        uint8_t unused : 5;
        uint8_t sprite_overflow : 1;
        uint8_t sprite_zero_hit : 1;
        uint8_t vertical_blank : 1;
    };
    uint8_t reg;
};

union Mask {
    struct {
        uint8_t grayscale : 1;
        uint8_t render_background_left : 1;
        uint8_t render_sprites_left : 1;
        uint8_t render_background : 1;
        uint8_t render_sprites : 1;
        uint8_t enhance_red : 1;
        uint8_t enhance_green : 1;
        uint8_t enhance_blue : 1;
    };
    uint8_t reg;
};

union PPUCTRL {
    struct {
        uint8_t nametable_x : 1;
        uint8_t nametable_y : 1;
        uint8_t increment_mode : 1;
        uint8_t pattern_sprite : 1;
        uint8_t pattern_background : 1;
        uint8_t sprite_size : 1;
        uint8_t slave_mode : 1; // unused
        uint8_t enable_nmi : 1;
    };
    uint8_t reg;
};

union LoopyRegister {
    struct {
        uint16_t coarse_x : 5;
        uint16_t coarse_y : 5;
        uint16_t nametable_x : 1;
        uint16_t nametable_y : 1;
        uint16_t y_offset : 3;
        uint16_t unused : 1;
    };
    uint16_t reg = 0x0000;
};
