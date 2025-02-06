import sox


class AudioEffects():
    def __init__(self, audio_path):
        self.audio_path = audio_path
        self.audio_name = audio_path[:-4]

    def add_effects(self,
                    reverb_specs=None,
                    delay_specs=None,
                    tremolo_specs=None,
                    flanger_specs=None,
                    pitch_shift=None):
        
        self.tfm = sox.Transformer()        
        effects = []

        if (reverb_specs != None):
            self.add_reverb(reverb_specs)
            effects.append("reverb")
        
        if (tremolo_specs != None):
            self.add_tremolo(tremolo_specs)
            effects.append("tremolo")

        if (delay_specs != None):
            self.add_delay(delay_specs)        
            effects.append("delay")
        
        if (flanger_specs != None):
            self.add_flanger(flanger_specs)        
            effects.append("flanger")
        
        if (pitch_shift != None):
            self.add_pitch(pitch_shift)
            effects.append("pitchshift")
        
        status = self.tfm.build_file(
            input_filepath=self.audio_path,
            output_filepath=self.audio_name + "_" + "_".join(effects) + '.wav'
        )
        
        print("Effects",  effects, "applied", self.audio_name)
    

    def add_reverb(self, specs):
        self.tfm.reverb(reverberance=specs[0],
                        high_freq_damping=specs[1],
                        room_scale=specs[2],
                        stereo_depth=specs[3],
                        pre_delay=0,
                        wet_gain=0,
                        wet_only=False)
        
    def add_delay(self, specs):
        self.tfm.echos(gain_in=specs[0],
                       gain_out=specs[1],
                       n_echos=len(specs[3]),
                       delays=specs[2],
                       decays=specs[3])
    
    def add_tremolo(self, specs):
        self.tfm.tremolo(speed=specs[0], depth=specs[1])
    
    def add_pitch(self, pitch):
        self.tfm.pitch(pitch)

    def add_flanger(self, specs):
        self.tfm.flanger(delay=specs[0],
                         depth=specs[1],
                         regen=specs[2],
                         width=specs[3],
                         speed=specs[4])
