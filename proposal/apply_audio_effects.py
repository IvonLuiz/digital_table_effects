from audio_effects import AudioEffects

sample_rate = 44100
audio_path = "proposal/audios/Lagrima.wav"
effects = AudioEffects(audio_path=audio_path)

# Reverb HALL 1*
effects.add_effects(reverb_specs=[50, 50, 100, 100])


# Reverb Room2 (Reverb with delay)
effects.add_effects(reverb_specs=[50, 50, 100, 100],
                    delay_specs=[1, 1, [300, 500, 700], [0.4, 0.2, 0.1]])


# Reverb stage B Major
"""
Falling pitch effect to simulate a descending tone (falling B major)
We'll drop the pitch over the duration of the audio to simulate "falling"
Drop the pitch down by 5 semitones
"""
effects.add_effects(reverb_specs=[70, 60, 30, 90],
                    pitch_shift=-5)


# Reverb falling D major
"""
D major falling, lowering pitch by 2 semitones
Apply tremolo for the robotization effect (rate in Hz, depth as percentage)
"""
effects.add_effects(reverb_specs=[70, 60, 90, 90],
                    pitch_shift=-2,
                    tremolo_specs=[30, 80])


# REV-STAGE F: Simulate depth with reverb and pitch shifting to F
# Pitch down by 4 semitones (for F)
effects.add_effects(reverb_specs=[60, 40, 70, 80],
                    pitch_shift=-4) 


# RET-STAGE Gb: Reverb and pitch shifting to Gb
# Pitch down by 6 semitones (for Gb)

# Flanger
"""
Flanger adds a sense of motion and sweeping across the audio spectrum
"""
effects.add_effects(flanger_specs=[0.6, 2, 0.4, 0.7, 0.5]) 


# TEEMOLO
"""
Apply a tremolo (low frequency amplitude modulation) effect to the audio.
The tremolo frequency in Hz is giv en by speed, and the depth as a 
percentage by depth (default 40).
"""
effects.add_effects(tremolo_specs=[12, 90])# Higher speed for more intense tremolo effect
