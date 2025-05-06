#pragma once

namespace character {
    class CharacterData {
    public:
        struct CharacterSpriteInfo {
            int frameCount;
            int x, y;
            int w, h;
        };

        static constexpr CharacterSpriteInfo MARIO_SPRITE[]{
                {1, 0, 8, 16, 16}, // Small-Mario Stand
                {3, 20, 8, 16, 16}, // Small-Mario walk
                {1, 76, 8, 16, 16}, // Small-Mario stop
                {1, 96, 8, 16, 16}, // Small-Mario jump
                {1, 116, 8, 16, 16}, // Small-Mario die
                {1, 0, 32, 16, 32}, // Big-Mario Stand
                {3, 20, 32, 16, 32}, // Big-Mario walk
                {1, 76, 32, 16, 32}, // Big-Mario stop
                {1, 96, 32, 16, 32}, // Big-Mario jump
                {1, 116, 32, 16, 32}, // Big-Mario crouch
                {1, 18, 80, 16, 24}, // Grow-Shrink
                {1, 0, 140, 16, 32}, // Fire-Mario Stand
                {3, 20, 140, 16, 32}, // Fire-Mario walk
                {1, 76, 140, 16, 32}, // Fire-Mario stop
                {1, 96, 140, 16, 32}, // Fire-Mario jump
                {1, 116, 140, 16, 32}, // Fire-Mario crouch
                {1, 136, 180, 16, 32},   // Fire-Mario fire
                {4, 172, 185, 8, 8},   // Fire
                {3, 172, 196, 16, 16},   // Explode
        };

    };
}