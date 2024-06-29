// Do NOT add any other includes
#include "dict.h"

Dict::~Dict(){
    // Implement your function here    
}


int  hash1(std::string id) {
     int  hash_ans = 0;
    for (int  i = 0; i < id.length(); i++) {
        hash_ans += static_cast<int >(id[i]) * (i + 1); // Multiply by position (1-based)
    }
    int  hash_value = hash_ans % 100019;
    return hash_value;
}


Dict::Dict() {
    root = nullptr;
    hash_table.resize(100019, nullptr);
}


Nodes::Nodes(string k){
    words=k;
    count=1;
    height =1;
}


int  heights(Nodes*N) 
{ 
	if (N == nullptr) 
		return 0; 
	return N->height; 
} 

string max(string a,string b){
    if(a>=b){
        return a;
    }
    return b;
}

Nodes*rightRotate(Nodes*y) 
{ 
	Nodes*x = y->left; 
	Nodes*T2 = x->right; 

	x->right = y; 
	y->left = T2; 

	y->height = max(heights(y->left), 
					heights(y->right)) + 1; 
	x->height = max(heights(x->left), 
					heights(x->right)) + 1; 

	return x; 
} 
 
Nodes*leftRotate(Nodes*x) 
{ 
	Nodes*y = x->right; 
	Nodes*T2 = y->left; 

	y->left = x; 
	x->right = T2; 

	x->height = max(heights(x->left), 
					heights(x->right)) + 1; 
	y->height = max(heights(y->left), 
					heights(y->right)) + 1; 
 
	return y; 
} 

int  getBalance(Nodes*N) 
{ 
	if (N == nullptr) 
		return 0; 
	return heights(N->left) - 
		heights(N->right); 
} 

Nodes* insertion(Nodes* node, string key) 
{   
    
	if (node == nullptr) {
		return new Nodes(key); 
    }
	if (key < node->words) 
		node->left = insertion(node->left, key); 
	else if (key > node->words) 
		node->right = insertion(node->right, key); 
	else 
		return node; 


	node->height = 1 + max(heights(node->left), 
						heights(node->right)); 

	
	int  balance = getBalance(node); 

	

	if (balance > 1 && key < node->left->words) 
		return rightRotate(node); 

	if (balance < -1 && key > node->right->words) 
		return leftRotate(node); 

	if (balance > 1 && key > node->left->words) 
	{ 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	if (balance < -1 && key < node->right->words) 
	{ 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 


	return node; 
} 

Nodes* search(Nodes* node, string k) {
    while (node != NULL) {
        if (node->words == k) {
            return node;
        }
        if (k > node->words) {
            node = node->right;
        } else {
            node = node->left;
        }
    }
    return NULL;
}



void InOrderT(vector<pair<string, int > >& command1, Nodes* root) {
    if (root == NULL) {
        return;
    }

    InOrderT(command1, root->left);

    command1.push_back(std::make_pair(root->words, root->count));

    InOrderT(command1, root->right);
}



vector <string> * sent_to_word(string s){
	vector<string> * v = new vector<string> () ;
	string str="";

	for (char &c:s){
		if(std::isupper(c)){
			c=std::tolower(c);
		}
	}
for (std::size_t i = 0; i < s.length(); i++) {
        if (std::isalpha(s[i]) || std::isdigit(s[i]) || s[i]=='/' ||s[i]=='|' || s[i]=='&' || s[i]=='$'|| s[i]=='%'|| s[i]=='~'|| s[i]=='_'|| s[i]=='#'|| s[i]=='+'|| s[i]=='='|| s[i]=='*'|| s[i]=='<'|| s[i]=='>'|| s[i]=='^'|| s[i]=='`'|| s[i]=='{'|| s[i]=='}'|| s[i]=='\\') {
            // if(s[i]!='.' && s[i]!=' ' && s[i]!=',' && s[i]!='-' && s[i]!='!' && s[i]!='"' && s[i]!='\'' && s[i]!='(' && s[i]!=')' && s[i]!='?' && s[i]!='—' && s[i]!='[' && s[i]!=']' && s[i]!='“'&& s[i]!='”' && s[i]!='˙'&& s[i]!=';'&&s[i]!='@' ){
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

void Dict::insert_sentence(int  book_code, int  page, int  paragraph, int  sentence_no, string sentence){
    // Implement your function here  
     if (sentence.empty()) {
        return; // Skip empty sentences
    }

    vector<string>* temp = sent_to_word(sentence);

    while (!temp->empty()) {
        for (int  i = 0; i < temp->size(); i++) {
            int  position = hash1((*temp)[i]);
            if (hash_table[position] == NULL) {
                Nodes* temp1 = new Nodes((*temp)[i]);
                hash_table[position] = temp1;
            } else {
                Nodes* foundNode = search(hash_table[position], (*temp)[i]);
                if (foundNode == NULL) {
                    hash_table[position] = insertion(hash_table[position], (*temp)[i]);
                } else {
                    foundNode->count++;
                }
            }
        }
        temp->clear();
    }
    delete temp; // Free the allocated memory
}

int  Dict::get_word_count(string word){
    // Implement your function here  
     // Normalize the word to lowercase
    for (char& c : word) {
        c = std::tolower(c);
    }

    int  position = hash1(word);
    Nodes* searchNode = search(hash_table[position], word);

    if (searchNode) {
        return searchNode->count;
    } else {
        return 0; // Word not found in the dictionary
    }
}

void Dict::dump_dictionary(string filename){
    // Implement your function here  
      ofstream output(filename);
    if (!output.is_open()) {
        // Handle the case where the file couldn't be opened
        return;
    }

vector<pair<string, int > > commands;

    for (int  i = 0; i < hash_table.size(); i++)
    {
        if(hash_table[i]==NULL){
            continue;
        }
        else{
            InOrderT( commands , hash_table[i]);
        }
    }
    
    

    for (const auto& command : commands) {
        output << command.first << ", " << command.second << endl;
    }

    output.close();
}