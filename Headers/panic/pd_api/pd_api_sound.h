//
//  pdext_sound.h
//  Playdate Simulator
//
//  Created by Dave Hayden on 10/6/17.
//  Copyright © 2017 Panic, Inc. All rights reserved.
//

#ifndef pdext_sound_h
#define pdext_sound_h

#if TARGET_EXTENSION

#define AUDIO_FRAMES_PER_CYCLE 512

typedef enum
{
	kSound8bitMono = 0,
	kSound8bitStereo = 1,
	kSound16bitMono = 2,
	kSound16bitStereo = 3,
	kSoundADPCMMono = 4,
	kSoundADPCMStereo = 5
} SoundFormat;

#define SoundFormatIsStereo(f) ((f)&1)
#define SoundFormatIs16bit(f) ((f)>=kSound16bitMono)

static inline uint32_t SoundFormat_bytesPerFrame(SoundFormat fmt)
{
	return (SoundFormatIsStereo(fmt) ? 2 : 1) * (SoundFormatIs16bit(fmt) ? 2 : 1);
}

#endif

// SOUND SOURCES

typedef struct SoundSource SoundSource;
typedef void sndCallbackProc(SoundSource* c);

// SoundSource is the parent class for FilePlayer, SamplePlayer, PDSynth, and DelayLineTap. You can safely cast those objects to a SoundSource* and use these functions:

struct playdate_sound_source
{
	void (*setVolume)(SoundSource* c, float lvol, float rvol);
	void (*getVolume)(SoundSource* c, float* outl, float* outr);
	int (*isPlaying)(SoundSource* c);
	void (*setFinishCallback)(SoundSource* c, sndCallbackProc callback);
};

typedef struct FilePlayer FilePlayer; // extends SoundSource

struct playdate_sound_fileplayer
{
	FilePlayer* (*newPlayer)(void);
	void (*freePlayer)(FilePlayer* player);
	void (*loadIntoPlayer)(FilePlayer* player, const char* path);
	void (*setBufferLength)(FilePlayer* player, float bufferLen);
	int (*play)(FilePlayer* player, int repeat);
	int (*isPlaying)(FilePlayer* player);
	void (*pause)(FilePlayer* player);
	void (*stop)(FilePlayer* player);
	void (*setVolume)(FilePlayer* player, float left, float right);
	void (*getVolume)(FilePlayer* player, float* left, float* right);
	float (*getLength)(FilePlayer* player);
	void (*setOffset)(FilePlayer* player, float offset);
	void (*setRate)(FilePlayer* player, float rate);
	void (*setLoopRange)(FilePlayer* player, float start, float end);
	int (*didUnderrun)(FilePlayer* player);
	void (*setFinishCallback)(FilePlayer* player, sndCallbackProc callback);
	void (*setLoopCallback)(FilePlayer* player, sndCallbackProc callback);
	float (*getOffset)(FilePlayer* player);
	float (*getRate)(FilePlayer* player);
	void (*setStopOnUnderrun)(FilePlayer* player, int flag);
	void (*fadeVolume)(FilePlayer* player, float left, float right, int32_t len, sndCallbackProc finishCallback);
	void (*setMP3StreamSource)(FilePlayer* player, int (*dataSource)(uint8_t* data, int bytes, void* userdata), void* userdata, float bufferLen);
};


typedef struct AudioSample AudioSample;
typedef struct SamplePlayer SamplePlayer;

struct playdate_sound_sample
{
	AudioSample* (*newSampleBuffer)(int byteCount);
	int (*loadIntoSample)(AudioSample* sample, const char* path);
	AudioSample* (*load)(const char* path);
	AudioSample* (*newSampleFromData)(uint8_t* data, SoundFormat format, uint32_t sampleRate, int byteCount);
	void (*getData)(AudioSample* sample, uint8_t** data, SoundFormat* format, uint32_t* sampleRate, uint32_t* bytelength);
	void (*freeSample)(AudioSample* sample);
	float (*getLength)(AudioSample* sample);
};

struct playdate_sound_sampleplayer // SamplePlayer extends SoundSource
{
	SamplePlayer* (*newPlayer)(void);
	void (*freePlayer)(SamplePlayer* player);
	void (*setSample)(SamplePlayer* player, AudioSample* sample);
	int (*play)(SamplePlayer* player, int repeat, float rate);
	int (*isPlaying)(SamplePlayer* player);
	void (*stop)(SamplePlayer* player);
	void (*setVolume)(SamplePlayer* player, float left, float right);
	void (*getVolume)(SamplePlayer* player, float* left, float* right);
	float (*getLength)(SamplePlayer* player);
	void (*setOffset)(SamplePlayer* player, float offset);
	void (*setRate)(SamplePlayer* player, float rate);
	void (*setPlayRange)(SamplePlayer* player, int start, int end);
	void (*setFinishCallback)(SamplePlayer* player, sndCallbackProc callback);
	void (*setLoopCallback)(SamplePlayer* player, sndCallbackProc callback);
	float (*getOffset)(SamplePlayer* player);
	float (*getRate)(SamplePlayer* player);
	void (*setPaused)(SamplePlayer* player, int flag);
};


// SIGNALS

// a PDSynthSignalValue represents a signal that can be used as an input to a modulator.
// its PDSynthSignal subclass is used for "active" signals that change their values automatically. PDSynthLFO and PDSynthEnvelope are subclasses of PDSynthSignal.

typedef struct PDSynthSignalValue PDSynthSignalValue;
typedef struct PDSynthSignal PDSynthSignal;

#if TARGET_EXTENSION
typedef enum
{
	kLFOTypeSquare,
	kLFOTypeTriangle,
	kLFOTypeSine,
	kLFOTypeSampleAndHold,
	kLFOTypeSawtoothUp,
	kLFOTypeSawtoothDown,
	kLFOTypeArpeggiator,
	kLFOTypeFunction
} LFOType;
#endif

typedef struct PDSynthLFO PDSynthLFO; // inherits from SynthSignal

struct playdate_sound_lfo
{
	PDSynthLFO* (*newLFO)(LFOType type);
	void (*freeLFO)(PDSynthLFO* lfo);
	
	void (*setType)(PDSynthLFO* lfo, LFOType type);
	void (*setRate)(PDSynthLFO* lfo, float rate);
	void (*setPhase)(PDSynthLFO* lfo, float phase);
	void (*setCenter)(PDSynthLFO* lfo, float center);
	void (*setDepth)(PDSynthLFO* lfo, float depth);
	void (*setArpeggiation)(PDSynthLFO* lfo, int nSteps, float* steps);
	void (*setFunction)(PDSynthLFO* lfo, float (*lfoFunc)(PDSynthLFO* lfo, void* userdata), void* userdata, int interpolate);
	void (*setDelay)(PDSynthLFO* lfo, float holdoff, float ramptime);
	void (*setRetrigger)(PDSynthLFO* lfo, int flag);
	
	float (*getValue)(PDSynthLFO* lfo);
};


typedef struct PDSynthEnvelope PDSynthEnvelope; // inherits from SynthSignal

typedef float MIDINote;
#define NOTE_C4 60

struct playdate_sound_envelope
{
	PDSynthEnvelope* (*newEnvelope)(float attack, float decay, float sustain, float release);
	void (*freeEnvelope)(PDSynthEnvelope* env);
	
	void (*setAttack)(PDSynthEnvelope* env, float attack);
	void (*setDecay)(PDSynthEnvelope* env, float decay);
	void (*setSustain)(PDSynthEnvelope* env, float sustain);
	void (*setRelease)(PDSynthEnvelope* env, float release);
	
	void (*setLegato)(PDSynthEnvelope* env, int flag);
	void (*setRetrigger)(PDSynthEnvelope* lfo, int flag);

	float (*getValue)(PDSynthEnvelope* env);
};


// SYNTHS

#if TARGET_EXTENSION
typedef enum
{
	kWaveformSquare,
	kWaveformTriangle,
	kWaveformSine,
	kWaveformNoise,
	kWaveformSawtooth,
	kWaveformPOPhase,
	kWaveformPODigital,
	kWaveformPOVosim
} SoundWaveform;
#endif

typedef int (*synthRenderFunc)(void* userdata, int32_t* left, int32_t* right, int nsamples, uint32_t rate, int32_t drate, int32_t l, int32_t dl, int32_t r, int32_t dr); // sample buffers are q8.24 format, and synth output should be added to existing samples in the buffers. l,dr and r,dr are envelope levels,ramp slope for left and right channels, in q4.28 format
typedef void (*synthNoteOnFunc)(void* userdata, MIDINote note, float velocity, float len); // len == -1 if indefinite
typedef void (*synthReleaseFunc)(void* userdata, int stop);
typedef int (*synthSetParameterFunc)(void* userdata, uint8_t parameter, float value);
typedef void (*synthDeallocFunc)(void* userdata);

typedef struct PDSynth PDSynth;

struct playdate_sound_synth // PDSynth extends SoundSource
{
	PDSynth* (*newSynth)(void);
	void (*freeSynth)(PDSynth* synth);
	
	void (*setWaveform)(PDSynth* synth, SoundWaveform wave);
	void (*setGenerator)(PDSynth* synth, synthRenderFunc* render, synthNoteOnFunc* noteOn, synthReleaseFunc* release, synthSetParameterFunc* setparam, synthDeallocFunc* dealloc, void* userdata);
	void (*setSample)(PDSynth* synth, AudioSample* sample, uint32_t sustainStart, uint32_t sustainEnd);

	void (*setAttackTime)(PDSynth* synth, float attack);
	void (*setDecayTime)(PDSynth* synth, float decay);
	void (*setSustainLevel)(PDSynth* synth, float sustain);
	void (*setReleaseTime)(PDSynth* synth, float release);
	
	void (*setTranspose)(PDSynth* synth, float halfSteps);
	
	void (*setFrequencyModulator)(PDSynth* synth, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getFrequencyModulator)(PDSynth* synth);
	void (*setAmplitudeModulator)(PDSynth* synth, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getAmplitudeModulator)(PDSynth* synth);
	
	int (*getParameterCount)(PDSynth* synth);
	int (*setParameter)(PDSynth* synth, int parameter, float value);
	void (*setParameterModulator)(PDSynth* synth, int parameter, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getParameterModulator)(PDSynth* synth, int parameter);
	
	void (*playNote)(PDSynth* synth, float freq, float vel, float len, uint32_t when); // len == -1 for indefinite
	void (*playMIDINote)(PDSynth* synth, MIDINote note, float vel, float len, uint32_t when); // len == -1 for indefinite
	void (*noteOff)(PDSynth* synth, uint32_t when); // move to release part of envelope
	void (*stop)(PDSynth* synth); // stop immediately
	
	void (*setVolume)(PDSynth* synth, float left, float right);
	void (*getVolume)(PDSynth* synth, float* left, float* right);
	
	int (*isPlaying)(PDSynth* synth);
};


// SEQUENCES

// a ControlSignal is a PDSynthSignal with values specified on a timeline

typedef struct ControlSignal ControlSignal;

struct playdate_control_signal
{
	ControlSignal* (*newSignal)(void);
	void (*freeSignal)(ControlSignal* signal);
	void (*clearEvents)(ControlSignal* control);
	void (*addEvent)(ControlSignal* control, int step, float value, int interpolate);
	void (*removeEvent)(ControlSignal* control, int step);
	int (*getMIDIControllerNumber)(ControlSignal* control);
};


// a PDSynthInstrument is a bank of voices for playing a sequence track

typedef struct PDSynthInstrument PDSynthInstrument;

struct playdate_sound_instrument
{
	PDSynthInstrument* (*newInstrument)(void);
	void (*freeInstrument)(PDSynthInstrument* inst);
	int (*addVoice)(PDSynthInstrument* inst, PDSynth* synth, MIDINote rangeStart, MIDINote rangeEnd, float transpose);
	PDSynth* (*playNote)(PDSynthInstrument* inst, float frequency, float vel, float len, uint32_t when);
	PDSynth* (*playMIDINote)(PDSynthInstrument* inst, MIDINote note, float vel, float len, uint32_t when);
	void (*setPitchBend)(PDSynthInstrument* inst, float bend);
	void (*setPitchBendRange)(PDSynthInstrument* inst, float halfSteps);
	void (*setTranspose)(PDSynthInstrument* inst, float halfSteps);
	void (*noteOff)(PDSynthInstrument* inst, MIDINote note, uint32_t when);
	void (*allNotesOff)(PDSynthInstrument* inst, uint32_t when);
	void (*setVolume)(PDSynthInstrument* inst, float left, float right);
	void (*getVolume)(PDSynthInstrument* inst, float* left, float* right);
	int (*activeVoiceCount)(PDSynthInstrument* inst);
};


// a SequenceTrack contains notes for an instrument to play

typedef struct SequenceTrack SequenceTrack;

struct playdate_sound_track
{
	SequenceTrack* (*newTrack)(void);
	void (*freeTrack)(SequenceTrack* track);
	
	void (*setInstrument)(SequenceTrack* track, PDSynthInstrument* inst);
	PDSynthInstrument* (*getInstrument)(SequenceTrack* track);
	
	void (*addNoteEvent)(SequenceTrack* track, uint32_t step, uint32_t len, MIDINote note, float velocity);
	void (*removeNoteEvent)(SequenceTrack* track, uint32_t step, MIDINote note);
	void (*clearNotes)(SequenceTrack* track);
	
	int (*getControlSignalCount)(SequenceTrack* track);
	ControlSignal* (*getControlSignal)(SequenceTrack* track, int idx);
	void (*clearControlEvents)(SequenceTrack* track);
	
	int (*getPolyphony)(SequenceTrack* track);
	int (*activeVoiceCount)(SequenceTrack* track);
	
	void (*setMuted)(SequenceTrack* track, int mute);
	
	// 1.1
	uint32_t (*getLength)(SequenceTrack* track); // in steps, includes full last note
	int (*getIndexForStep)(SequenceTrack* track, uint32_t step);
	int (*getNoteAtIndex)(SequenceTrack* track, int index, uint32_t* outStep, uint32_t* outLen, MIDINote* outNote, float* outVelocity);
};

// and a SoundSequence is a collection of tracks, along with control info like tempo and loops

typedef struct SoundSequence SoundSequence;
typedef void (*SequenceFinishedCallback)(SoundSequence* seq, void* userdata);

struct playdate_sound_sequence
{
	SoundSequence* (*newSequence)(void);
	void (*freeSequence)(SoundSequence* sequence);
	
	int (*loadMidiFile)(SoundSequence* seq, const char* path);
	uint32_t (*getTime)(SoundSequence* seq);
	void (*setTime)(SoundSequence* seq, uint32_t time);
	void (*setLoops)(SoundSequence* seq, int loopstart, int loopend, int loops);
	int (*getTempo)(SoundSequence* seq);
	void (*setTempo)(SoundSequence* seq, int stepsPerSecond);
	int (*getTrackCount)(SoundSequence* seq);
	SequenceTrack* (*addTrack)(SoundSequence* seq);
	SequenceTrack* (*getTrackAtIndex)(SoundSequence* seq, unsigned int track);
	void (*setTrackAtIndex)(SoundSequence* seq, SequenceTrack* track, unsigned int idx);
	void (*allNotesOff)(SoundSequence* seq);
	
	// 1.1
	int (*isPlaying)(SoundSequence* seq);
	uint32_t (*getLength)(SoundSequence* seq); // in steps, includes full last note
	void (*play)(SoundSequence* seq, SequenceFinishedCallback finishCallback, void* userdata);
	void (*stop)(SoundSequence* seq);
	int (*getCurrentStep)(SoundSequence* seq, int* timeOffset);
	void (*setCurrentStep)(SoundSequence* seq, int step, int timeOffset, int playNotes);
};


// EFFECTS

// A SoundEffect processes the output of a channel's SoundSources

typedef struct TwoPoleFilter TwoPoleFilter;

#if TARGET_EXTENSION
typedef enum
{
	kFilterTypeLowPass,
	kFilterTypeHighPass,
	kFilterTypeBandPass,
	kFilterTypeNotch,
	kFilterTypePEQ,
	kFilterTypeLowShelf,
	kFilterTypeHighShelf
} TwoPoleFilterType;
#endif

struct playdate_sound_effect_twopolefilter
{
	TwoPoleFilter* (*newFilter)(void);
	void (*freeFilter)(TwoPoleFilter* filter);
	void (*setType)(TwoPoleFilter* filter, TwoPoleFilterType type);
	void (*setFrequency)(TwoPoleFilter* filter, float frequency);
	void (*setFrequencyModulator)(TwoPoleFilter* filter, PDSynthSignalValue* signal);
	PDSynthSignalValue* (*getFrequencyModulator)(TwoPoleFilter* filter);
	void (*setGain)(TwoPoleFilter* filter, float gain);
	void (*setResonance)(TwoPoleFilter* filter, float resonance);
	void (*setResonanceModulator)(TwoPoleFilter* filter, PDSynthSignalValue* signal);
	PDSynthSignalValue* (*getResonanceModulator)(TwoPoleFilter* filter);
};

typedef struct OnePoleFilter OnePoleFilter;

struct playdate_sound_effect_onepolefilter
{
	OnePoleFilter* (*newFilter)(void);
	void (*freeFilter)(OnePoleFilter* filter);
	void (*setParameter)(OnePoleFilter* filter, float parameter);
	void (*setParameterModulator)(OnePoleFilter* filter, PDSynthSignalValue* signal);
	PDSynthSignalValue* (*getParameterModulator)(OnePoleFilter* filter);
};

typedef struct BitCrusher BitCrusher;

struct playdate_sound_effect_bitcrusher
{
	BitCrusher* (*newBitCrusher)(void);
	void (*freeBitCrusher)(BitCrusher* filter);
	void (*setAmount)(BitCrusher* filter, float amount);
	void (*setAmountModulator)(BitCrusher* filter, PDSynthSignalValue* signal);
	PDSynthSignalValue* (*getAmountModulator)(BitCrusher* filter);
	void (*setUndersampling)(BitCrusher* filter, float undersampling);
	void (*setUndersampleModulator)(BitCrusher* filter, PDSynthSignalValue* signal);
	PDSynthSignalValue* (*getUndersampleModulator)(BitCrusher* filter);
};

typedef struct RingModulator RingModulator;

struct playdate_sound_effect_ringmodulator
{
	RingModulator* (*newRingmod)(void);
	void (*freeRingmod)(RingModulator* filter);
	void (*setFrequency)(RingModulator* filter, float frequency);
	void (*setFrequencyModulator)(RingModulator* filter, PDSynthSignalValue* signal);
	PDSynthSignalValue* (*getFrequencyModulator)(RingModulator* filter);
};

typedef struct DelayLine DelayLine;
typedef struct DelayLineTap DelayLineTap;

struct playdate_sound_effect_delayline
{
	DelayLine* (*newDelayLine)(int length, int stereo);
	void (*freeDelayLine)(DelayLine* filter);
	void (*setLength)(DelayLine* d, int frames);
	void (*setFeedback)(DelayLine* d, float fb);
	DelayLineTap* (*addTap)(DelayLine* d, int delay);
	
	// note that DelayLineTap is a SoundSource, not a SoundEffect
	void (*freeTap)(DelayLineTap* tap);
	void (*setTapDelay)(DelayLineTap* t, int frames);
	void (*setTapDelayModulator)(DelayLineTap* t, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getTapDelayModulator)(DelayLineTap* t);
	void (*setTapChannelsFlipped)(DelayLineTap* t, int flip);
};

typedef struct Overdrive Overdrive;

struct playdate_sound_effect_overdrive
{
	Overdrive* (*newOverdrive)(void);
	void (*freeOverdrive)(Overdrive* filter);
	void (*setGain)(Overdrive* o, float gain);
	void (*setLimit)(Overdrive* o, float limit);
	void (*setLimitModulator)(Overdrive* o, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getLimitModulator)(Overdrive* o);
	void (*setOffset)(Overdrive* o, float offset);
	void (*setOffsetModulator)(Overdrive* o, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getOffsetModulator)(Overdrive* o);
};


typedef struct SoundEffect SoundEffect;
typedef int effectProc(SoundEffect* e, int32_t* left, int32_t* right, int nsamples, int bufactive); // samples are in signed q8.24 format

struct playdate_sound_effect
{
	SoundEffect* (*newEffect)(effectProc* proc, void* userdata);
	void (*freeEffect)(SoundEffect* effect);
	
	void (*setMix)(SoundEffect* effect, float level);
	void (*setMixModulator)(SoundEffect* effect, PDSynthSignalValue* signal);
	PDSynthSignalValue* (*getMixModulator)(SoundEffect* effect);

	void (*setUserdata)(SoundEffect* effect, void* userdata);
	void* (*getUserdata)(SoundEffect* effect);

	const struct playdate_sound_effect_twopolefilter* twopolefilter;
	const struct playdate_sound_effect_onepolefilter* onepolefilter;
	const struct playdate_sound_effect_bitcrusher* bitcrusher;
	const struct playdate_sound_effect_ringmodulator* ringmodulator;
	const struct playdate_sound_effect_delayline* delayline;
	const struct playdate_sound_effect_overdrive* overdrive;
};


// A SoundChannel contains SoundSources and SoundEffects

typedef struct SoundChannel SoundChannel;

typedef int AudioSourceFunction(void* context, int16_t* left, int16_t* right, int len); // len is # of samples in each buffer, function should return 1 if it produced output

struct playdate_sound_channel
{
	SoundChannel* (*newChannel)(void);
	void (*freeChannel)(SoundChannel* channel);
	int (*addSource)(SoundChannel* channel, SoundSource* source);
	int (*removeSource)(SoundChannel* channel, SoundSource* source);
	SoundSource* (*addCallbackSource)(SoundChannel* channel, AudioSourceFunction* callback, void* context, int stereo);
	void (*addEffect)(SoundChannel* channel, SoundEffect* effect);
	void (*removeEffect)(SoundChannel* channel, SoundEffect* effect);
	void (*setVolume)(SoundChannel* channel, float volume);
	float (*getVolume)(SoundChannel* channel);
	void (*setVolumeModulator)(SoundChannel* channel, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getVolumeModulator)(SoundChannel* channel);
	void (*setPan)(SoundChannel* channel, float pan);
	void (*setPanModulator)(SoundChannel* channel, PDSynthSignalValue* mod);
	PDSynthSignalValue* (*getPanModulator)(SoundChannel* channel);
	PDSynthSignalValue* (*getDryLevelSignal)(SoundChannel* channel);
	PDSynthSignalValue* (*getWetLevelSignal)(SoundChannel* channel);
};

typedef int RecordCallback(void *context, int16_t* buffer, int length); // data is mono

struct playdate_sound
{
	const struct playdate_sound_channel* channel;
	const struct playdate_sound_fileplayer* fileplayer;
	const struct playdate_sound_sample* sample;
	const struct playdate_sound_sampleplayer* sampleplayer;
	const struct playdate_sound_synth* synth;
	const struct playdate_sound_sequence* sequence;
	const struct playdate_sound_effect* effect;
	const struct playdate_sound_lfo* lfo;
	const struct playdate_sound_envelope* envelope;
	const struct playdate_sound_source* source;
	const struct playdate_control_signal* controlsignal;
	const struct playdate_sound_track* track;
	const struct playdate_sound_instrument* instrument;

	uint32_t (*getCurrentTime)(void);
	SoundSource* (*addSource)(AudioSourceFunction* callback, void* context, int stereo);

	SoundChannel* (*getDefaultChannel)(void);
	
	void (*addChannel)(SoundChannel* channel);
	void (*removeChannel)(SoundChannel* channel);
	
	void (*setMicCallback)(RecordCallback* callback, void* context, int forceInternal);
	void (*getHeadphoneState)(int* headphone, int* headsetmic, void (*changeCallback)(int headphone, int mic));
	void (*setOutputsActive)(int headphone, int speaker);
	
	// 1.5
	void (*removeSource)(SoundSource* source);
};

#endif /* pdext_sound_h */
