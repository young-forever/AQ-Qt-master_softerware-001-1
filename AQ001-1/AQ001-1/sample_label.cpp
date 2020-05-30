#include "sample_label.h"
#include <qfile.h>
#include <sstream>
#include <fstream>

sample_label::sample_label(int len) : str_buffer_len(len)
{
    save_dir = "/iamge/default/default.csv";
    label_buffer = new string[len];
    label_cnt = 0;
}


sample_label::~sample_label()
{

}


void sample_label::set_save_dir(string dir)
{
    save_dir = dir;
}


bool sample_label::add_label(string label)
{
    if(label_cnt<str_buffer_len)
    {
        label_buffer[label_cnt] = label;
    }
    else
    {
        return false;// buffer full;
    }
    label_cnt++;

    return true;
}

void sample_label::save_label()
{
    ofstream outfile(save_dir.c_str(),ios::app);
    for(int i=0; i<label_cnt;i++)
    {
        outfile << label_buffer[i] << "\n";
    }
    outfile.close();

    label_cnt = 0;

}
