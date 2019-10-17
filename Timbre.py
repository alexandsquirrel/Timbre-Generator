from PIL import Image
import os
from MidiConverter import *

def process_image(name):
    try:
        im = Image.open("./pic/" + name + ".bmp")
        imarray = im.load()
    except:
        print("Oops! File does not exist. Make sure it's inside \"pic\" folder.")
    if(im.size != (1000, 200)):
        raise ValueError("Image has to be 1000 by 200.")
    threshold = 1 # threshold for identifying a color to be black
    res = []
    for col in range(1000):
        for row in reversed(range(200)):
            if imarray[col, row][0] < threshold:
                res.append(row)
                break
    mf = open('./pic/' + name + ".txt", 'w')
    s = ''
    for n in res:
        s += ('%.10f,' % (float(n) / 200))
    mf.write(s)

def output_tone(input, output):
    BASE_FREQ = 27.500
    FREQ_INCR_RATIO = 1.0594630943593
    file_name = "./pic/" + input + ".txt"
    freq = BASE_FREQ
    for i in range(88):
        os.system('wave %s %f 5' % (file_name, freq))
        os.rename('wave.wav', '%d.wav' % i)
        freq *= FREQ_INCR_RATIO
    if not os.path.exists("./timbre"):
        os.mkdir("timbre")
    os.system('Converter ./timbre/' + output + ".tone")
    for i in range(88):
        os.remove("%d.wav" % i)

def create_timbre(input, output):
    process_image(input)
    output_tone(input, output)

def play_midi(midi, timbre):
    if not os.path.exists("./midi/" + midi + ".txt"):
        try:
            convert(midi)
        except FileNotFoundError:
            print("Input midi not found!")
    os.system('TrueTone ' + "./timbre/" + timbre + ".tone ./midi/" + midi + ".txt")