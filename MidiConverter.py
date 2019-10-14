import midi, os

class Note:
    def __init__(self,pitch,beg,end):
        self.pitch=pitch
        self.beg=beg
        self.end=end

def convert(filename):
    notename = ['C', 'bD', 'D', 'bE', 'E', 'F', 'bG', 'G', 'bA', 'A', 'bB', 'B']
    fout = open("./midi/" + filename + ".txt", "w")
    fout.write("48\n")
    pattern = midi.read_midifile("./midi/" + filename + ".mid")
    notes = []
    timesig = [0]
    for track in pattern:
        print('new track found!')
        time = 0
        pending = []
        for i in range(len(track)):
            if not isinstance(track[i], midi.NoteOnEvent) and not isinstance(track[i], midi.NoteOffEvent):
                continue
            time += track[i].tick
            if track[i].tick != 0 and not time in timesig:
                timesig.append(time)
            if isinstance(track[i], midi.NoteOnEvent) and track[i].data[1] > 0 and\
                len(filter(lambda x: x.pitch == track[i].pitch, pending)) == 0:
                pending.append(Note(pitch=track[i].data[0], beg=time, end=-1))
            if (isinstance(track[i], midi.NoteOnEvent) and track[i].data[1] == 0) or\
                isinstance(track[i], midi.NoteOffEvent):
                idx = 0
                while idx < len(pending) and pending[idx].pitch != track[i].data[0]:
                    idx += 1
                if idx == len(pending):
                    continue
                notes.append(Note(pitch=pending[idx].pitch, beg=pending[idx].beg, end=time))
                del pending[idx]
    timesig = sorted(timesig)
    notes = sorted(notes, key=lambda x: x.beg)
    for i in range(len(timesig) - 1):
        beg = timesig[i]
        end = timesig[i + 1]
        play = []
        for j in range(len(notes)):
            if notes[j].beg > end:
                break
            if notes[j].beg <= beg and notes[j].end >= end:
                play.append(j)
        for n in play:
            realname = notename[(notes[n].pitch) % 12] + str(int((notes[n].pitch) / 12))
            fout.write('%s 88 %d 1920\n' % (realname, end - beg))
        if len(play) == 0:
            fout.write('0 100 %d 1920\n' % (end - beg))
        fout.write('ChordEnd\n')
    fout.write('PartEnd\n')
    fout.close()