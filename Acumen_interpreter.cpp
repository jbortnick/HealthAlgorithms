#include "sample.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <plot.h>
#include <algorithm>
#include <fftw3.h>

using namespace std;


Lg::Sample::~Sample(void){}
  
//!empty sample
Lg::Sample::Sample(double br,string str,string spk){
  _speaker = spk;
  _phrase = str;
  _bitrate=br;
}

//!load sample from file
Lg::Sample::Sample(string file){
  ifstream in(file.c_str(),ios::in);
  double i;
  int num_samples;
  getline(in,_phrase);
  getline(in,_speaker);
  in>>_bitrate;
  in>>num_samples;
  for(int c=0;c<num_samples;c++){
    in>>i;
    push_back(i);
  }
}

//!initialize as a periodic signal of freq-Hz and length l
Lg::Sample::Sample(int freq,double l,int br,string str,string spk){
  _speaker=spk;
  _phrase=str;
  _bitrate=br;
  double p = (double)br/(double)freq;
  for(int c=0;c<l;c++){
    double rads = 6.28*((double)(c-(c/(int)p))/(double)p);
    this->push_back((float)(sin(rads)));
  }
}

//!treat a vector of values as a signal (try to call this with a vector<double> for best results)
//template<class Input>
Lg::Sample::Sample(vector<double> &i,double br,string str,string spk){
  _speaker = spk;
  _phrase = str;
  _bitrate=br;
  for(uint c=0;c<i.size();c++)
    push_back((double)i[c]);
}

//!save sample to file
void Lg::Sample::save(string file){
  ofstream out(file.c_str(),ios::out|ios::app);
  out<<_phrase<<endl;
  out<<_speaker<<endl;
  out<<_bitrate<<endl;
  out<<this->size()<<endl;
  for(uint c=0;c<this->size();c++)
    out<<(*this)[c]<<endl;
  out.close();
}
//!adds another sample on top
void Lg::Sample::add(Sample &x){
  for(uint c=0;c<x.size() && c<this->size();c++){
    (*this)[c] += x[c];
    (*this)[c] /=2;
  }
}

//!length in seconds
double Lg::Sample::length(void){
  return (double)(this->size())/_bitrate;
}

//!get/set the phrase/speaker associated with the audio
string Lg::Sample::phrase(void){ return _phrase; }
void Lg::Sample::phrase(string str){ _phrase = str; }

string Lg::Sample::speaker(void){ return _speaker; }
void Lg::Sample::speaker(string str){ _speaker = str; }

//!adds another sample onto the end
void Lg::Sample::append(Sample &x){
  for(uint c=0;c<x.size();c++)
    this->push_back(x[c]);
}
 