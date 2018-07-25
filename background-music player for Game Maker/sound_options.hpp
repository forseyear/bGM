#pragma once

#include "stdafx.hpp"

using sound_options_p = struct
{
    tstring filename;
    float volume;
    float pan;
    float pitch;
    bool isPlaying;
    bool isLooping;
    bool isDebuging;
    bool isLoadOnly;
};
