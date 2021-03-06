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

#ifndef __LUFS_PROCESSOR_H__
#define __LUFS_PROCESSOR_H__

class BiquadProcessor
{
public:
    enum FilterType
    {
        Off = 0,
        LowPass, 
        HighPass,
        BandPass,
        Notch,
        PeakBandEq,
        LowShelf,
        HighShelf
    };

    BiquadProcessor();

    void process( float * _data, const int _sampleSize );

    void setFilterParams( const float _samplingRate, const FilterType _filterType, const float _frequency, const float _quality, const float _decibelGain );

private:
    float m_X_1;
    float m_X_2;
    float m_Y_1;
    float m_Y_2;
    float m_B0, m_B1, m_B2, m_A1, m_A2;
};

class LufsFloatArray : public juce::Array<float>
{
public:

    LufsFloatArray() : m_sum( 0.f ) {}
    
    void addLufs( float element ) 
    { 
        m_sum += element; 

        addSorted( *this, element ); 
    }

    static int compareElements( const float first, const float second ) noexcept
    {
        const int result = (int)( 1000000000.f * ( first - second ) );

        return result;
    }

    float getPercentileValue( const int beginIndex, const float percentile ) const
    {
        jassert( beginIndex < size() );
        jassert( percentile >= 0.f );
        jassert( percentile <= 1.f );

        switch( size() )
        {
        case 0:
            return 0.f;
        case 1:
        case 2:
            return getUnchecked( 0 );
        }

        const int indexBefore = beginIndex + (int)( percentile * (float)( size() - beginIndex - 1 ) );
        return getUnchecked( indexBefore );/*
        const int indexAfter = indexBefore + 1;
        jassert( indexAfter < size() );

        float alpha = percentile * (float)( size() - 1 ) - (float)indexBefore;

        return ( 1.f - alpha ) * getUnchecked( indexBefore ) + alpha * getUnchecked( indexAfter );*/
    }

    int findIndexAfterValue_( float value )
    {
        // to be optimized
        jassert( size() );

        for ( int i = 0 ; i < size() ; ++i )
        {
            if ( getUnchecked( i ) > value )
            {
/*
#pragma message ("debug code to be removed from findIndexAfterValue")
                for ( int j = i ; j < size() ; ++j )
                {
                    jassert( getUnchecked( j ) >= value );
                }*/
                return i;
            }
        }

        return size() - 1;
    }

    int findIndexAfterValue( float value )
    {
        // to be optimized
        jassert( size() );

        if ( getUnchecked( 0 ) > value ) 
        {
            int test = findIndexAfterValue_( value );
            jassert( 0 == test );
            return 0;
        }

        int indexBefore = 0; 
        int indexAfter = size() - 1;

        while ( indexAfter - indexBefore > 1 )
        {
            int index = ( indexAfter + indexBefore ) >> 1;
            if ( getUnchecked( index ) > value )
                indexAfter = index;
            else
                indexBefore = index;
        }

        int test = findIndexAfterValue_( value );
//        if ( test )
        {
            jassert( indexAfter == test );
        }

        return indexAfter;
    }

    float getSum() const 
    {
        return m_sum;
    }

    void reset()
    {
        clearQuick();

        m_sum = 0.f;
    }

private:

    float m_sum;
};


class LufsProcessor
{
public:

    LufsProcessor( const int nbChannels );

    ~LufsProcessor();

    void update();

    void reset();
    void prepareToPlay(const double sampleRate, int samplesPerBlock);
    void processBlock( juce::AudioSampleBuffer& buffer );

    inline void pause() { m_paused = true; }
    inline void resume() { m_paused = false; }
    inline bool isPaused() { return m_paused; }

    inline float * getMomentaryVolumeArray() const { return m_momentaryVolumeArray; } 
    inline float * getShortTermVolumeArray() const { return m_shortTermVolumeArray; } 
    inline float * getIntegratedVolumeArray() const { return m_integratedVolumeArray; }

    inline float getIntegratedVolume() { return m_integratedVolume; }
    inline float getRangeMinVolume() { return m_rangeMin; }
    inline float getRangeMaxVolume() { return m_rangeMax; }

    inline int getValidSize() const { return m_validSize; }
    inline int getMaxSize() const { return m_maxSize; }

    inline int getSeconds() const { return m_processSize / 10; }

private:
    void processPeak( const juce::AudioSampleBuffer& buffer );

    void addSquaredInput( const float squaredInput );
    void updatePosition( int position );

    static double ms_log10;
    float getLufsVolume( const float sum ) { return float( juce::jmax( float(-0.691 + 10.0 * log( sum ) / ms_log10 ), DEFAULT_MIN_VOLUME ) ); }
    float getLufsSum( const float volume ) { return float( exp( ( volume + 0.691 ) * ms_log10 / 10.0 ) ); }

    juce::AudioSampleBuffer m_block; // data is copied to this buffer then filtered
    juce::AudioSampleBuffer m_memory; // samples not processed from previous callback 
    double m_sampleRate;
    int m_nbChannels;

    juce::Array<BiquadProcessor> m_shelveFilterArray;
    juce::Array<BiquadProcessor> m_highPassFilterArray;

    int m_maxSize;
    volatile int m_processSize;
    int m_validSize; // process size as seen by client, in main update 
    int m_memorySize;
    int m_sampleSize100ms;

    float * m_squaredInputArray;
    float * m_momentaryVolumeArray;
    float * m_shortTermVolumeArray;
    float * m_integratedVolumeArray;
    float m_integratedVolume;
    float m_rangeMin;
    float m_rangeMax;

    // 4 samples to calculate min/max, per channel
    float ** m_memArray;
    // linear max, per channel 
    float * m_maxLinArray;
    juce::AudioSampleBuffer m_tempBlock; // to process min max;

    juce::SpinLock m_locker;

    LufsFloatArray m_sum400ms70;
    LufsFloatArray m_sum3s70;

    bool m_paused;
};

typedef float f32;
typedef unsigned int u32;

class LinInterpolationMinMaxEstimate
{
    // middle value of _fiveValueArray is min or max 
    // _findMinimum is true when looking for a minimum, false when looking for a maximum

public:
    LinInterpolationMinMaxEstimate( const f32 * _fiveValueArray, const u32 _xOffset, const bool _findMinimum ); 

    inline f32 getXMiddle() const { return m_xMiddle; }
    inline bool isValid() const { return m_isValid; }
    inline f32 getMinMax() const { jassert( m_isValid ); return m_minMax; }

private:

    f32 calculateMinOrMax();

    f32 m_y0, m_y1, m_y2, m_y3; 
    f32 m_xMiddle;
    f32 m_xMiddleWithoutOffset;
    f32 m_minMax;
    bool m_isValid;
};


#endif  // __LUFS_PROCESSOR_H__
