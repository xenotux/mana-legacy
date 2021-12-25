#ifndef MANA_AUDIOSOURCECOMPONENT_HPP
#define MANA_AUDIOSOURCECOMPONENT_HPP

#include "engine/asset/audio.hpp"
#include "engine/asset/assethandle.hpp"

namespace engine {
    struct AudioSourceComponent {
        AssetPath audioPath;
        bool play = false;
        bool loop = false;
        Vec3f velocity = {};

        bool playing = false;
    };
}

#endif //MANA_AUDIOSOURCECOMPONENT_HPP