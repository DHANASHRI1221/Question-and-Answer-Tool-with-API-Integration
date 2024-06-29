


// #include <string> 
// #include <vector>
// #include <iostream>

// #include "Node.h"
// using namespace std;

// #define ALPH_SIZE 50

// class SearchEngine {

// private:
//     // You can add attributes/helper functions here
//     struct WordCount {
//     string sentence1;
//     int book_code;
//     int page;
//     int paragraph;
//     int sentence_no;
//    // int offset;

// };
//  vector<WordCount> v;    
// public: 
//     int book_code = 0;
//     int page = 0;
//     int paragraph = 0;
//     int sentence_no = 0;
//     int offset = 0;
//     SearchEngine();

//     ~SearchEngine();
//     void insert1(string s);
//     int hash_sentence(string s);
//     bool is_punct(char d);

// int power(int base, int exponent);
// int findd(const string& sentence, const string& pattern);
//     void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

//     Node* search(string pattern, int& n_matches);
//     long long count(string pattern);
// };







// Do NOT add any other includes
#pragma once //////////////////////////
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;

class SearchEngine {
private:
    // You can add attributes/helper functions here

public: 
    vector< pair<string,vector<long long> > > corpus; 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int & n_matches);

    /* -----------------------------------------*/

    long long count(string pattern);
};