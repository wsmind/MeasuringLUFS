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

#ifndef __LUFS_CHART_H__
#define __LUFS_CHART_H__

class LufsPluginEditor;

class ChartView;

class Chart : public juce::Component
{
public:
    Chart( float _minChartVolume, float _maxChartVolume ) 
        : m_editor( nullptr ) 
        , m_chartView( nullptr )
        , m_minChartVolume( _minChartVolume )
        , m_maxChartVolume( _maxChartVolume )
    {}

    void update();

    // juce::Component
    virtual void paint( juce::Graphics& g );

    inline void setEditor( LufsPluginEditor * editor ) { m_editor = editor; }
    int getVolumeY( const int height, const float decibels );

    inline void setChartView( ChartView * _chartView ) { m_chartView = _chartView; }

    float getMinChartVolume() const { return m_minChartVolume; }
    float getMaxChartVolume() const { return m_maxChartVolume; }

private:

    void paintValues( juce::Graphics& g, const juce::Colour _color, const float * _data, const int _itemsPerPixel, const int _offset, const int _pixels );

    LufsPluginEditor * m_editor;
    ChartView * m_chartView;
    float m_minChartVolume;
    float m_maxChartVolume;
    int m_validSize;
};

class ChartView : public juce::Viewport
{
public:
    ChartView( float _minChartVolume, float _maxChartVolume );

    // juce::Component
    virtual void resized();
    virtual void mouseWheelMove( const juce::MouseEvent& event, const juce::MouseWheelDetails & wheel ) override;

    // juce::Viewport
    // virtual void visibleAreaChanged( const juce::Rectangle<int>& newVisibleArea );

    inline void setEditor( LufsPluginEditor * editor ) { m_chart.setEditor( editor ); }

    void update();

private:

    Chart m_chart;
};


#endif  // __LUFS_CHART_H__
