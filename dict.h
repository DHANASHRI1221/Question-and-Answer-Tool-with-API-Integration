// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


class Nodes{
public:
    int  count;
    int  height;
    string words;

    Nodes*par=nullptr;
    Nodes*left=nullptr;
    Nodes*right=nullptr;
    Nodes(string k);
    ~Nodes();
};

class Dict {
private:
    Nodes* root=nullptr;
    vector<Nodes* > hash_table ;
    // You can add attributes/helper functions here

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int  book_code, int  page, int  paragraph, int  sentence_no, string sentence);

    int  get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};