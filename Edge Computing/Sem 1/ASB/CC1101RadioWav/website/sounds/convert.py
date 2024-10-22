import wave
from pydub import AudioSegment
from pydub.effects import low_pass_filter
import sys
import os

def convert_to_8bit_mono_pcm(input_wav, output_wav, sample_rate=4000):
    # Load the audio file using pydub
    audio = AudioSegment.from_file(input_wav)

    audio = low_pass_filter(audio, sample_rate//2)

    # Resample the audio to the desired sample rate
    audio = audio.set_frame_rate(sample_rate)
    
    # Convert to mono
    audio = audio.set_channels(1)
    
    # Convert to 8-bit PCM
    audio = audio.set_sample_width(1)
    
    # Export the converted audio
    audio.export(output_wav, format="wav")

def main():    
    try:
        convert_to_8bit_mono_pcm("gasoline.opus", "gasoline.wav", sample_rate=2500)
        convert_to_8bit_mono_pcm("formilesdown.opus", "formilesdown.wav", sample_rate=2500)
        convert_to_8bit_mono_pcm("16.opus", "16.wav", sample_rate=2500)
        convert_to_8bit_mono_pcm("moon1.wav", "moon.wav", sample_rate=2500)

        print(f"Successfully converted")
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
