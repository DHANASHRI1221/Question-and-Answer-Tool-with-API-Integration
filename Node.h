#pragma once
#include <string>
using namespace std;

class Node {
public: 
    Node* left;
    Node* right;

    long long book_code;
    long long page;
    long long paragraph;
    long long sentence_no;
    long long offset;

    Node();
    Node(long long b_code, long long pg, long long para, long long s_no, long long off);
};