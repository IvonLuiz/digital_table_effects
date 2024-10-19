import numpy as np
import sox

def AudioEffects():
    def __init__(self, audio_path):
        self.audio_path = audio_path
        self.audio_name = audio_path[:-4]
    
    def add_effects(self, reverb=False, delay=False, tremolo=False):
        self.tfm = sox.Transformer()        

        if (reverb==True):
            self.add_reverb()
        
        if (tremolo==True):
            self.add_tremolo()

        if (delay==True):
            self.add_delay()        


        status = self.tfm.build_file(
            input_filepath=self.audio_path,
            output_filepath=self.audio_name + '_rev_delay_out.wav'
        )
        
        print(status)
    
    def add_reverb(self):
        self.tfm.reverb(reverberance=50,
                high_freq_damping=50,
                room_scale=100,
                stereo_depth=100,
                pre_delay=0,
                wet_gain=0,
                wet_only=False)
        
    def add_delay(self):
        None
        
    def add_tremolo(self):
        None