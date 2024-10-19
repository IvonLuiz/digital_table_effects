import numpy as np
import sox


sample_rate = 44100
audio_path = "proposal/audios/Lagrima.wav"


tfm = sox.Transformer()

# Reverb HALL 1*

"""
Add reverberation to the audio using the ‘freeverb’ algorithm.
A reverberation effect is sometimes desirable for concert halls
that are too small or contain so many people that the hall’s
natural reverberance is diminished. Applying a small amount of
stereo reverb to a (dry) mono signal will usually make it sound more natural.
"""
tfm.reverb(reverberance=50,
           high_freq_damping=50,
           room_scale=100,
           stereo_depth=100,
           pre_delay=0,
           wet_gain=0,
           wet_only=False)

status = tfm.build_file(
        input_filepath=audio_path,
        output_filepath=audio_path[:-4] + '_rev_out.wav'
    )

# Reverb Room2 (Reverb with delay)

tfm.echos(gain_in=1,
          gain_out=1,
          n_echos=3,
          delays=[300, 500, 700],
          decays=[0.4, 0.2, 0.1],)

status = tfm.build_file(
        input_filepath=audio_path,
        output_filepath=audio_path[:-4] + '_rev_delay_out.wav'
    )

# Reverb stage B

tfm = sox.Transformer()

tfm.reverb(reverberance=70,
           high_freq_damping=60,
           room_scale=30,
           stereo_depth=90,
           pre_delay=0,
           wet_gain=0,
           wet_only=False)

# Falling pitch effect to simulate a descending tone (falling B major)
# We'll drop the pitch over the duration of the audio to simulate "falling"
tfm.pitch(-5)  # Drop the pitch down by 5 semitones

# Build the output file
output_filepath = audio_path[:-4] + '_falling_b_major_out.wav'
status = tfm.build_file(
    input_filepath=audio_path,
    output_filepath=output_filepath
)

# Reverb falling D major

tfm = sox.Transformer()

# Add reverb for depth and electronic sound
tfm.reverb(reverberance=70,
           high_freq_damping=60,
           room_scale=90,
           stereo_depth=90,
           pre_delay=20,
           wet_gain=0,
           wet_only=False)

# Pitch shift to simulate falling D major
tfm.pitch(-2)  # D major falling, lowering pitch by 2 semitones

# Apply tremolo for the robotization effect (rate in Hz, depth as percentage)
# tfm.tremolo(speed=30, depth=80)  # Simulates robotized voice with tremolo effect

output_filepath = audio_path[:-4] + '_robotized_falling_d_major.wav'
status = tfm.build_file(
    input_filepath=audio_path,
    output_filepath=output_filepath
)


# Initialize the transformer
tfm = sox.Transformer()

# REV-STAGE F: Simulate depth with reverb and pitch shifting to F
tfm.pitch(-4)  # Pitch down by 4 semitones (for F)
tfm.reverb(reverberance=60,
           high_freq_damping=40,
           room_scale=70,
           stereo_depth=80,
           pre_delay=10,
           wet_gain=0,
           wet_only=False)

# RET-STAGE Gb: Reverb and pitch shifting to Gb
tfm.pitch(-6)  # Pitch down by 6 semitones (for Gb)
tfm.reverb(reverberance=70,
           high_freq_damping=50,
           room_scale=80,
           stereo_depth=90,
           pre_delay=15,
           wet_gain=0,
           wet_only=False)

# FLANGER: Add a flanger effect for electronic modulation
# Flanger adds a sense of motion and sweeping across the audio spectrum
tfm.flanger(delay=0.6, depth=2, regen=0.4, width=0.7, speed=0.5)

# TEEMOLO: A stronger or faster tremolo effect
# Tremolo modulates volume at a specified speed and depth
"""
Apply a tremolo (low frequency amplitude modulation) effect to the audio.
The tremolo frequency in Hz is giv en by speed, and the depth as a 
percentage by depth (default 40).
"""
tfm.tremolo(speed=12, depth=90)  # Higher speed for more intense tremolo effect

# Build the output file
output_filepath = audio_path[:-4] + '_final_mix_effects.wav'
status = tfm.build_file(
    input_filepath=audio_path,
    output_filepath=output_filepath
)

print(f"Output saved to: {output_filepath}")



tfm.tremolo(speed=6,
            depth=40)