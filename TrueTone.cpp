#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#include <stdint.h>
#include <string.h>
#include <vector>
#include "music.h"
#define rate 44100
using namespace std;

struct Wave
{
    unsigned long file_size;
    unsigned short channel=2;
    unsigned long frequency=rate;
    unsigned long Bps=4*rate;
    unsigned short sample_num_bit=16;
    unsigned long data_size;
    unsigned char *data;
};

struct Tone
{
    struct
    {
        unsigned int samplerate,samplesize;
        short left[409600],right[409600];
    }key[88];
    void init(const char *filename)
    {
        ifstream fin(filename,ios::in|ios::binary);
        for(int i=0;i<88;i++)fin.read((char *)&key[i],sizeof(key));
    }
};

ofstream os;
ifstream info,fin;
vector<struct Tone> tone;
vector<struct Part> part;
struct Part partt;
struct Tone tonet;
unsigned int totaltime,partn;

void init(char timbre[], char score[])
{
    char temp[100];
    unsigned int i,t;
    os.open("wave.wav", ios::binary | ios::out);
    while(!os.is_open())
    {
        cout<<"wave.wav is occupied by another program,press any key to try again..."<<endl;
        getch();
        os.open("wave.wav",ios::binary|ios::out);
    }
    sprintf(temp,"parser.exe %s %d",score, rate);
    system(temp);
    fin.open("temp.txt",ios::in);
    partt.read(fin);
    totaltime=max(totaltime,partt.totaltime);
    part.push_back(partt);
    fin.close();
    tonet.init(timbre);
    tone.push_back(tonet);
}

void write_header()
{
    unsigned int i;
    unsigned short a;
    Wave wave;
    os<<"RIFF";
    wave.data_size=4*totaltime;
    wave.file_size=wave.data_size+36;
    os.write((const char*)&wave.file_size,sizeof(long));
    os<<"WAVEfmt ";i=16;a=1;
    os.write((const char*)&i,sizeof(int));
    os.write((const char*)&a,sizeof(short));
    os.write((const char*)&wave.channel,sizeof(short));
    os.write((const char*)&wave.frequency,sizeof(int));
    os.write((const char*)&wave.Bps,sizeof(int));
    a=4;
    os.write((const char*)&a,sizeof(short));
    os.write((const char*)&wave.sample_num_bit,sizeof(short));
    os<<"data";
    os.write((const char*)&wave.data_size,sizeof(int));
}

void write_data()
{
    unsigned int i;
    unsigned int j;
    for(auto &i:part)for(auto &j:i.chord)j.tnote=0;
    short ldata,rdata;
    unsigned int phase;
    for(i=0;i<totaltime;i++)
    {
        if(!rand())cout<<"\routputing..."<<100ll*i/totaltime<<"%";
        ldata=0;rdata=0;
        for(j=0;j<part.size();j++)
        {
            for(auto &c:part[j].chord)
            {
                if(c.tnote<c.note.size()-1&&i>c.note[c.tnote+1].stime)c.tnote++;
                if(c.note[c.tnote].isrest)continue;
                for(auto n:c.note[c.tnote].nt)
                {
                    phase=(1.0*i-c.note[c.tnote].stime)*tone[j].key[n].samplerate/rate;
                    if(phase<tone[j].key[n].samplesize)
                    {
                        //cout<<"force of key="<<c.note[c.tnote].force<<endl;
                        ldata+=tone[j].key[n].left[phase]*c.note[c.tnote].force/32768;
                        rdata+=tone[j].key[n].right[phase]*c.note[c.tnote].force/32768;
                    }
                }
            }
            os.write((const char *)&ldata,sizeof(short));
            os.write((const char *)&rdata,sizeof(short));
        }
    }
    cout<<"\routputing...100%"<<endl;
}

int main(int argc, char* argv[])
{
    init(argv[1], argv[2]);
    write_header();
    write_data();
    os.close();
    remove("temp.txt");
    return 0;
}
