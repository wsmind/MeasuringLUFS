/*
  =================================================================

  This file is part of the Measuring LUFS program.
  Copyright (c) 2014 - Mathieu Pavageau - contact@repetito.com

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  Details of these licenses can be found at: www.gnu.org/licenses

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  =================================================================
*/

#ifndef __LUFS_APPINCLUDESANDDEFS_H__
#define __LUFS_APPINCLUDESANDDEFS_H__

// Juce include files

#include "AppConfig.h"

#include "modules/juce_audio_basics/juce_audio_basics.h"
#include "modules/juce_audio_devices/juce_audio_devices.h"
#include "modules/juce_audio_formats/juce_audio_formats.h"
#include "modules/juce_audio_processors/juce_audio_processors.h"
#include "modules/juce_audio_utils/juce_audio_utils.h"
#include "modules/juce_core/juce_core.h"
#include "modules/juce_data_structures/juce_data_structures.h"
#include "modules/juce_graphics/juce_graphics.h"
#include "modules/juce_gui_basics/juce_gui_basics.h"
#include "modules/juce_gui_extra/juce_gui_extra.h"

//#define TESTCOLORS

#define DEFAULT_MIN_VOLUME ( -100.f )

#define LUFS_EDITOR_WIDTH 750
#define LUFS_EDITOR_HEIGHT 500

#define LUFS_COLOR_BACKGROUND juce::Colours::black
#define LUFS_COLOR_FONT juce::Colours::lightgrey

#define COLOR_LUFSTIME LUFS_COLOR_FONT
#define COLOR_MOMENTARY ( juce::Colours::blue ).interpolatedWith( juce::Colours::lightblue, 0.5f )
#define COLOR_SHORTTERM ( juce::Colours::green ).interpolatedWith( juce::Colours::white, 0.2f )
#define COLOR_INTEGRATED juce::Colours::orange 
#define COLOR_RANGE juce::Colours::grey 
#define COLOR_BACKGROUND_GRAPH juce::Colours::darkgrey

#endif // __LUFS_APPINCLUDESANDDEFS_H__
