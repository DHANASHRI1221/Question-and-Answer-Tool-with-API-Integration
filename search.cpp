#include "search.h"

template<typename T>
typename std::vector<T>::iterator custom_find(std::vector<T>& vec, const T& value) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it == value) {
            return it;
        }
    }
    return vec.end();
}

SearchEngine::SearchEngine() {
    // Implement your function here
}

SearchEngine::~SearchEngine() {
    // Implement your function here
    // delete [] corpus;
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
    // Implement your function here
    for (char &c : sentence) {
        if (std::isupper(c)) {
            c = std::tolower(c);
        }
    }
    vector<long long > details = {book_code, page, paragraph, sentence_no};
    corpus.push_back(make_pair(sentence, details));
    return;
}

vector<int> prefix_function(string s) {
    int n = s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];

        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            j++;
        }
        pi[i] = j;
    }
    return pi;
}

vector<string>* sentence_to_word2(string s) {
    vector<string>* v = new vector<string>();
    string str = "";

    for (char &c : s) {
        if (std::isupper(c)) {
            c = std::tolower(c);
        }
    }
    for (std::size_t i = 0; i < s.length(); i++) {
        if (std::isalpha(s[i]) || std::isdigit(s[i]) || s[i] == '/' || s[i] == '|' || s[i] == '&' || s[i] == '$' || s[i] == '%' || s[i] == '~' || s[i] == '_' || s[i] == '#' || s[i] == '+' || s[i] == '=' || s[i] == '*' || s[i] == '<' || s[i] == '>' || s[i] == '^' || s[i] == '`' || s[i] == '{' || s[i] == '}' || s[i] == '\\') {
            str += s[i];
        } else if (!str.empty()) {
            v->push_back(str);
            str = "";
        }
    }

    if (!str.empty()) {
        v->push_back(str);
    }
    return v;
}

Node* SearchEngine::search(string pattern, int& n_matches) {
    Node* nill = new Node();
    Node* temp = nill;
    n_matches = 0;
    vector<string>* it = sentence_to_word2(pattern);
    int l = 0;

    while (l < it->size()) {
        for (int k = 0; k < corpus.size(); k++) {
            string text = corpus[k].first;
            size_t pos = 0;
            size_t textLen = text.size();
            size_t patternLen = pattern.size();

            while (pos + patternLen <= textLen) {
                bool isMatch = true;
                for (size_t i = 0; i < patternLen; i++) {
                    if (text[pos + i] != pattern[i]) {
                        isMatch = false;
                        break;
                    }
                }

                if (isMatch) {
                    // Check if it's an independent word
                    bool isIndependentWord = true;

                    // Check if character before the pattern is alphanumeric or part of a larger word
                    if (pos > 0 && (std::isalpha(text[pos - 1]) || std::isdigit(text[pos - 1])) && custom_find(*it, text.substr(pos - 1, pattern.size())) == it->end()) {
                        isIndependentWord = false;
                    }

                    // Check if character after the pattern is alphanumeric or part of a larger word
                    if (pos + patternLen < textLen && (std::isalpha(text[pos + patternLen]) || std::isdigit(text[pos + patternLen])) && custom_find(*it, text.substr(pos, pattern.size() + 1)) == it->end()) {
                        isIndependentWord = false;
                    }

                    if (isIndependentWord) {
                        n_matches++;
                        Node* n = new Node(corpus[k].second[0], corpus[k].second[1], corpus[k].second[2], corpus[k].second[3], static_cast<int>(pos));

                        temp->right = n;
                        n->left = temp;
                        temp = n;
                    }
                }

                pos++;
            }
        }
        l++;
    }

    delete it; // Release memory allocated for vector<string>
    return nill->right;
}

long long SearchEngine::count(string pattern) {
    int n = 0;
    Node* temp = search(pattern, n);
    long long var = 1;
    while (temp != nullptr) {
        temp = temp->right;
        var++;
    }
    return var;
}
