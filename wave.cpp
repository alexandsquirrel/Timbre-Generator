#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>

using namespace std;

struct wave
{
    unsigned long file_size;
    unsigned short channel=2;
    unsigned long frequency=44100;
    unsigned long Bps=176400;
    unsigned short sample_num_bit=16;
    unsigned long data_size;
    unsigned char *data;
};

double dat[1000];

void gen_wav(double freq,double t,ofstream &os)
{
    double k=0.0f;
    for(int j=0;j<t*44100;j++)
    {
        k=k+10.0f*freq/441;
        short data=32768*dat[((int)k)%1000];
        os.write((const char *)&data,sizeof(short));
        os.write((const char *)&data,sizeof(short));
    }
}

int main(int argc, char *argv[])
{
    ofstream os("wave.wav", ios::binary | ios::out);
    int i,j,freq,t;
    double k;
    short a;
    char fq[12],tm[12];
    strcpy(fq,argv[2]);
    strcpy(tm,argv[3]);

    int s=0;
    double totaltime=atof(tm);
    wave w;
    FILE *fin=fopen(argv[1],"r");
    for(i=0;i<1000;i++) fscanf(fin,"%lf,",&dat[i]);
    os<<"RIFF";
    w.data_size=176400*totaltime;
    w.file_size=w.data_size+36;
    os.write((const char*)&w.file_size,sizeof(long));
    os<<"WAVEfmt ";i=16;a=1;
    os.write((const char*)&i,sizeof(int));
    os.write((const char*)&a,sizeof(short));
    os.write((const char*)&w.channel,sizeof(short));
    os.write((const char*)&w.frequency,sizeof(int));
    os.write((const char*)&w.Bps,sizeof(int));
    a=4;
    os.write((const char*)&a,sizeof(short));
    os.write((const char*)&w.sample_num_bit,sizeof(short));
    os<<"data";
    os.write((const char*)&w.data_size,sizeof(int));
    gen_wav(atof(fq),atof(tm),os);
    os.close();
    return 0;
}
