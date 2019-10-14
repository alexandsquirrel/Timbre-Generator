#include<fstream>
#include<iostream>
#include<conio.h>
using namespace std;

struct key
{
    int samplerate,samplesize;
    short left[409600],right[409600];
    void init(const char *filename)
    {
        int i;
        ifstream fin(filename,ios::in|ios::binary);
        if(!fin.is_open())cout<<"Could not open file:"<<filename<<endl;
        fin.seekg(24,ios::beg);fin.read((char *)&samplerate,sizeof(long));
        fin.seekg(40,ios::beg);fin.read((char *)&samplesize,sizeof(long));
        samplesize/=4;
        for(i=0;i<samplesize;i++)
        {
            fin.read((char *)&left[i],sizeof(short));
            fin.read((char *)&right[i],sizeof(short));
        }
    }
}k;

int main(int argc, char* argv[])
{
    ofstream fout(argv[1],ios::out|ios::binary);
    int i;
    char temp[88];
    for(i=0;i<88;i++)
    {
        sprintf(temp,"%d.wav",i);
        k.init(temp);
        fout.write((const char *)&k,sizeof(key));
    }
    cout<<sizeof(key)<<endl;
    fout.close();
}
