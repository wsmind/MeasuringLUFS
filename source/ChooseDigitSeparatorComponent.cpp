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

#include "AppIncsAndDefs.h"

#include "ChooseDigitSeparatorComponent.h"

ChooseDigitSeparatorComponent::ChooseDigitSeparatorComponent( juce::ApplicationProperties & _settings )
    : m_useCommas( true )
    , m_settings( _settings )
    , m_useCommasString( "useCommasForDecimalMarkForDataExport" )
{
    setSize( 500, 100 );

    m_okButton.setButtonText( juce::String( "OK" ) );
    m_okButton.addListener( this );
    m_okButton.setColour( juce::TextButton::buttonColourId, LUFS_COLOR_BACKGROUND );
    m_okButton.setColour( juce::TextButton::buttonOnColourId, LUFS_COLOR_FONT );
    m_okButton.setColour( juce::TextButton::textColourOffId, LUFS_COLOR_FONT );
    m_okButton.setColour( juce::TextButton::textColourOnId, LUFS_COLOR_BACKGROUND );
    m_okButton.setBounds( 330, 30, 80, 40 );
    addAndMakeVisible( &m_okButton );
        
    m_commaButton.setBounds( 240, 21, 20, 20 );
    m_commaButton.setRadioGroupId( 1 );
    m_commaButton.addListener( this );
    m_commaButton.setClickingTogglesState( true );
    addAndMakeVisible( &m_commaButton );

    m_pointButton.setBounds( 240, 60, 20, 20 );
    m_pointButton.setRadioGroupId( 1 );
    m_pointButton.addListener( this );
    m_pointButton.setClickingTogglesState( true );
    addAndMakeVisible( &m_pointButton );

    m_useCommas = m_settings.getUserSettings()->getBoolValue( m_useCommasString, "true" );
    
    if ( m_useCommas )
    {
        m_commaButton.setToggleState( true, juce::dontSendNotification );
    }
    else
    {
        m_pointButton.setToggleState( true, juce::dontSendNotification );
    }
    //setClickingTogglesState( true );//setState( juce::Button::buttonNormal );
}

ChooseDigitSeparatorComponent::~ChooseDigitSeparatorComponent()
{
}

void ChooseDigitSeparatorComponent::paint( juce::Graphics & g )
{
    g.fillAll( LUFS_COLOR_BACKGROUND );

    juce::Font font( 16.f );
    font.setBold(true);
    g.setFont (font);
    g.setColour( LUFS_COLOR_FONT );

    g.drawFittedText( "Use decimal comma", 20, 20, 210, 20, juce::Justification::centredRight, 1, 0.01f );
    g.drawFittedText( "Use decimal point", 20, 60, 210, 20, juce::Justification::centredRight, 1, 0.01f );
}

void ChooseDigitSeparatorComponent::buttonClicked( juce::Button * _button ) 
{
    if ( _button == &m_okButton )
    {
        juce::DialogWindow * window = findParentComponentOfClass<juce::DialogWindow>();

        if ( window != nullptr )
            window->exitModalState( 51 );
    }
    else if ( _button == &m_commaButton )
    {
        m_useCommas = m_commaButton.getToggleState();
        m_settings.getUserSettings()->setValue( m_useCommasString, m_useCommas ? 1 : 0 );
    }
}
