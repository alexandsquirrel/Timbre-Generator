#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "music.h"
using namespace std;

unsigned int getnum(const char* str)
{
    unsigned ans;
    switch (str[0]) {
    case 'c':
        ans = 0;
        break;
    case 'd':
        ans = 2;
        break;
    case 'e':
        ans = 4;
        break;
    case 'f':
        ans = 5;
        break;
    case 'g':
        ans = 7;
        break;
    case 'a':
        ans = 9;
        break;
    case 'b':
        ans = 11;
        break;
    }
    ans += 12 * (str[1] - '0') - 9;
    return ans;
}

struct Part part;
struct Chord chord;
struct Note note;
unsigned int rate, speed, wholelen;
double totaltime;
ofstream fout;
ifstream fin;

int main(int argc, char* argv[])
{
    char temp[100];
    double stime;
    unsigned int duration;
    int alter;
    if (argc != 3)
        return 1;
    fin.open(argv[1], ios::in);
    rate = atoi(argv[2]);
    fin >> speed;
    stime = 0;
    while (!fin.eof()) {
        fin >> temp;
        strlwr(temp);
        if (strcmp(temp, "chordend") == 0) {
            chord.note.push_back(note);
            note.nt.clear();
            stime = stime + 240.0 * duration * rate / wholelen / speed;
            continue;
        }
        if (strcmp(temp, "partend") == 0) {
            if (stime == 0)
                continue;
            part.chord.push_back(chord);
            chord.note.clear();
            part.totaltime = max(part.totaltime, (unsigned int)stime);
            cout << "Chord total time:" << (unsigned int)stime << endl;
            stime = 0;
            continue;
        }
        alter = 0;
        switch (strlen(temp)) {
        case 1:
            note.nt.push_back(0);
            note.isrest = true;
            break;
        case 3:
            if (temp[0] == '#')
                alter = 1;
            else
                alter = -1;
        case 2:
            note.nt.push_back(getnum(temp + (alter != 0)) + alter);
            note.isrest = false;
            break;
        }
        fin >> note.force;
        note.force *= 128;
        fin >> duration;
        fin >> wholelen;
        note.stime = stime;
    }
    fin.close();
    fout.open("temp.txt", ios::out);
    part.write(fout);
    fout.close();
}