#include "Node.h"
#include <assert.h>

Node::Node() {
    left=nullptr;
    right=nullptr;
    book_code=-1;
    page=-1;
    paragraph=-1;
    sentence_no=-1;
    offset=-1;
}

Node::Node(long long b_code, long long pg, long long para, long long s_no, long long off){
    book_code = b_code;
    page = pg;
    paragraph = para;
    sentence_no = s_no;
    offset = off;
    left=nullptr;
    right=nullptr;
}