/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BoomerAudioProcessor::BoomerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    state = new AudioProcessorValueTreeState(*this, nullptr);

    state->createAndAddParameter("SpeedAttachment", "Speed", "Speed", NormalisableRange<float>(0.f, 200.f, 0.0f), 0.f, nullptr, nullptr);
    state->state = ValueTree("Speed");
    
    state->createAndAddParameter("HighFreqAttachment", "High", "High", NormalisableRange<float>(550.f, 3500.f, 0.0f), 0.f, nullptr, nullptr);
    state->state = ValueTree("High");

    state->createAndAddParameter("LowFreqAttachment", "Low", "Low", NormalisableRange<float>(200.f, 500.f, 0.0f), 0.f, nullptr, nullptr);
    state->state = ValueTree("Low");

    state->createAndAddParameter("MixAttachment", "Mix", "Mix", NormalisableRange<float>(0.f, 1.f, 0.01f), 0.f, nullptr, nullptr);
    state->state = ValueTree("Mix");

    state->createAndAddParameter("VolumeAttachment", "Volume", "Volume", NormalisableRange<float>(0.f, 1.f, 0.01f), 0.f, nullptr, nullptr);
    state->state = ValueTree("Volume");
}

BoomerAudioProcessor::~BoomerAudioProcessor()
{
}

//==============================================================================
const String BoomerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BoomerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BoomerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BoomerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BoomerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BoomerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BoomerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BoomerAudioProcessor::setCurrentProgram (int index)
{
}

const String BoomerAudioProcessor::getProgramName (int index)
{
    return {};
}

void BoomerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BoomerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    lastSampleRate = sampleRate;
    freq = 200.f;
    direction = true;
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    stateVariableFilter.reset();
    updateFilter();
    stateVariableFilter.prepare(spec);
}

void BoomerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void BoomerAudioProcessor::updateFilter() {

    //Sync to tempo
    //Value p = state->getParameterAsValue("SpeedAttachment");
    //p.setValue((3875.f-200.f)/getSamplesPerBeat());

    float speed = *state->getRawParameterValue("SpeedAttachment"); // speed
    float highFreq = *state->getRawParameterValue("HighFreqAttachment"); // speed
    float lowFreq = *state->getRawParameterValue("LowFreqAttachment"); // speed
    
    if (freq >= highFreq) {
        direction = false;
    }
    else if (freq <= lowFreq) {
        direction = true;
    }

    if (direction) freq += speed;
    else freq -= speed;

    stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
    stateVariableFilter.state->setCutOffFrequency(lastSampleRate, freq,  1/sqrt(2));
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BoomerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

double BoomerAudioProcessor::getHostTempo() const {
    double result = 120.f; //fallback value in case of an error

    AudioPlayHead* playHead = getPlayHead();
    if (playHead != NULL) {
        AudioPlayHead::CurrentPositionInfo currentPosition;
        playHead->getCurrentPosition(currentPosition);
        result = currentPosition.bpm;
    }
    return result;
}

double BoomerAudioProcessor::getSamplesPerBeat() const {
    double beatsPerSec = hostTempo / 60;
    double secPerBeat = 1 / beatsPerSec;
    double samplesPerSec = getSampleRate() / 60;
    return secPerBeat * samplesPerSec;
}

void BoomerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float dryWet = *state->getRawParameterValue("MixAttachment");
    float volume = *state->getRawParameterValue("VolumeAttachment");

    AudioSampleBuffer drybuffer;
    drybuffer.makeCopyOf(buffer, true); // this ain't so smart :)

    dsp::AudioBlock<float> dryBlock(drybuffer);
    dryBlock.multiplyBy(1 - dryWet);
    
    dsp::AudioBlock<float> wetBlock(buffer);
    updateFilter();
    wetBlock.multiplyBy(dryWet*3);
    stateVariableFilter.process(dsp::ProcessContextReplacing<float> (wetBlock));

    wetBlock.add(dryBlock);
    wetBlock.multiplyBy(volume);
}

AudioProcessorValueTreeState& BoomerAudioProcessor::getState() {
    return *state;
}

//==============================================================================
bool BoomerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BoomerAudioProcessor::createEditor()
{
    return new BoomerAudioProcessorEditor (*this);
}

//==============================================================================
void BoomerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    MemoryOutputStream stream(destData, false);
    state->state.writeToStream(stream);
}

void BoomerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    ValueTree tree = ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {
        state->state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BoomerAudioProcessor();
}
