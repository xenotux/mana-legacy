/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "oalaudiocontext.hpp"
#include "oalaudiobuffer.hpp"
#include "oalaudiosource.hpp"

#include "oalcheckerror.hpp"

#include <AL/al.h>
#include <AL/alc.h>

namespace mana {
    OALAudioContext::OALAudioContext(ALCcontext *context) : context(context), listener() {}

    mana::OALAudioContext::~OALAudioContext() {
        if (alcGetCurrentContext() == context)
            alcMakeContextCurrent(nullptr);
        alcDestroyContext(context); //TODO:Fix: Destroying a openal context seems to always set invalid operation error
    }

    void mana::OALAudioContext::makeCurrent() {
        alcMakeContextCurrent(context);
        checkOALError();
    }

    AudioListener &mana::OALAudioContext::getListener() {
        return listener;
    }

    AudioBuffer *mana::OALAudioContext::createBuffer() {
        ALuint n;
        alGenBuffers(1, &n);
        checkOALError();
        return new OALAudioBuffer(n);
    }

    AudioSource *mana::OALAudioContext::createSource() {
        ALuint n;
        alGenSources(1, &n);
        checkOALError();
        return new OALAudioSource(n);
    }

    const ALCcontext *OALAudioContext::getContext() {
        return context;
    }
}