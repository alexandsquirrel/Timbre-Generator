#include<vector>
#include<fstream>
using namespace std;

struct Note
{
    unsigned int force,stime;
    bool isrest;
    vector<unsigned int> nt;
    void write(ofstream &out)
    {
        out<<force<<' '<<stime<<' '<<isrest<<' '<<nt.size()<<' ';
        for(auto i:nt)out<<i<<' ';
    }
    void read(ifstream &in)
    {
        unsigned int i,n,t;
        in>>force;in.get();
        in>>stime;in.get();
        in>>isrest;in.get();
        in>>n;in.get();
        for(i=0;i<n;i++)
        {
            in>>t;in.get();
            nt.push_back(t);
        }
    }
    void print()
    {
        if(isrest)cout<<"This is rest!"<<endl;
        else for(auto i:nt)cout<<"note:"<<i<<endl;
        cout<<"force:"<<force<<endl;
        cout<<"start time:"<<stime<<endl;
    }
};

struct Chord
{
    vector<struct Note> note;
    unsigned int tnote;
    void read(ifstream &in)
    {
        unsigned int i,n;
        struct Note notet;
        in>>n;in.get();
        for(i=0;i<n;i++)
        {
            notet.nt.clear();
            notet.read(in);
            note.push_back(notet);
        }
    }
    void write(ofstream &out)
    {
        out<<note.size()<<' ';
        for(auto i:note)i.write(out);
    }
    void print()
    {
        cout<<"this note="<<tnote<<endl;
        cout<<"notenum="<<note.size()<<endl;
        for(auto i:note)i.print();
    }
};

struct Part
{
    unsigned int totaltime;
    vector<struct Chord> chord;
    void read(ifstream &in)
    {
        unsigned int i,n;
        struct Chord chordt;
        in>>totaltime;in.get();
        in>>n;in.get();
        for(i=0;i<n;i++)
        {
            chordt.note.clear();
            chordt.read(in);
            chord.push_back(chordt);
        }
    }
    void write(ofstream &out)
    {
        out<<totaltime<<' '<<chord.size()<<' ';
        for(auto i:chord)i.write(out);
    }
    void print()
    {
        cout<<"total time="<<totaltime<<endl;
        cout<<"chordnum="<<chord.size()<<endl;
        for(auto i:chord)i.print();
    }
};
