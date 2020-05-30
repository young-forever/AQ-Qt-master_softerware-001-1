#ifndef SAMPLE_LABEL_H
#define SAMPLE_LABEL_H
#include <iostream>

using namespace std;

class sample_label
{
    std::__cxx11::string save_dir;
    int str_buffer_len;
    int label_cnt;
    string* label_buffer;

public:
    sample_label(int len);
    ~sample_label();

    void set_save_dir(string dir);
    bool add_label(string label);
    void save_label();

};

#endif // SAMPLE_LABEL_H
