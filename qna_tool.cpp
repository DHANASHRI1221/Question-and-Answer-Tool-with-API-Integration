
#include <assert.h>
#include <sstream>
#include "qna_tool.h"
// #include "Node.cpp"
// #include "search.cpp"


using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

QNA_tool::QNA_tool(){}

QNA_tool::~QNA_tool(){}


vector<string> *sentence_to_word(string s)
{
    vector<string> *v = new vector<string>();
    string str = "";

    for (char &c : s)
    {
        if (std::isupper(c))
        {
            c = std::tolower(c);
        }
    }
    for (std::size_t i = 0; i < s.length(); i++)
    {
        if (std::isalpha(s[i]) || std::isdigit(s[i]) || s[i] == '/' || s[i] == '|' || s[i] == '&' || s[i] == '$' || s[i] == '%' || s[i] == '~' || s[i] == '_' || s[i] == '#' || s[i] == '+' || s[i] == '=' || s[i] == '*' || s[i] == '<' || s[i] == '>' || s[i] == '^' || s[i] == '`' || s[i] == '{' || s[i] == '}' || s[i] == '\\')
        {
            // if(s[i]!='.' && s[i]!=' ' && s[i]!=',' && s[i]!='-' && s[i]!='!' && s[i]!='"' && s[i]!='\'' && s[i]!='(' && s[i]!=')' && s[i]!='?' && s[i]!='—' && s[i]!='[' && s[i]!=']' && s[i]!='“'&& s[i]!='”' && s[i]!='˙'&& s[i]!=';'&&s[i]!='@' ){
            str += s[i];
        }
        else if (!str.empty())
        {
            v->push_back(str);
            str = "";
        }
    }

    if (!str.empty())
    {
        v->push_back(str);
    }
    return v;
}

double  QNA_tool::score_word(string i)
{
    double a = QNA_tool::temp.count(i) + 1;
    double c = 0;
    std::ifstream file("unigram_freq.csv");
    if (!file.is_open()){   
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    std::string line;
    std::getline(file, line); // Skip the header line
    bool found = false;
    while (std::getline(file, line) && found == false)
    {
        std::istringstream ss(line);
        std::string token;
        std::getline(ss, token, ',');
        if (token == i)
        {
            std::getline(ss, token, ',');
            c = std::stoll(token);
            found = true;
            break;
        }
    }
    if (!found){
        c = 0;
    }
    file.close();
   double b = c + 1;
   double d = a/b ;
//    cout<<" i am here to calculate score a "<<a<<endl;
//    cout<<" i am here to calculate score  b "<<b<<endl;
//    cout<<" i am here to calculate score"<<d<<endl;
    return d;
}



// void mysort(std::vector<QNA_tool::triplet>& arr) {
//     bool swapped;
//     int n = arr.size();
//     do {
//         swapped = false;
//         for (int i = 0; i < n - 1; ++i) {
//             if (arr[i].score < arr[i + 1].score) {
//                 std::swap(arr[i], arr[i + 1]);
//                 swapped = true;
//             }
//         }
//         n--;
//     } while (swapped);
// }









int sort1(std::vector< QNA_tool::triplet>&v1,int l,int e ){
  QNA_tool:: triplet p = v1[l];
	int c = 0;
	for (int i = l + 1; i <= e; i++) {
		if (v1[i].score >=  p.score)
			c++;
	}
	int h = l + c;
    QNA_tool::triplet tmp=v1[h];
            v1[h]=v1[l];
            v1[l]=tmp;
	int i = l, j = e;

	while (i < h && j > h) {

		while (v1[i].score >= p.score) {
			i++;
		}

		while (v1[j].score < p.score) {
			j--;
		}

		if (i < h && j > h) {
		 QNA_tool:: triplet temp=v1[i];
            v1[i]=v1[j];
            v1[j]=temp;
            i++;
            j--;
		}
	}

	return h;
    }

void mysort(std::vector< QNA_tool::triplet> &v1,int l,int k ){
if(l<k){
    int h=sort1(v1,l,k);
    mysort(v1,l,h-1);
    mysort(v1,h+1,k);
}
}









vector<QNA_tool::triplet> QNA_tool::query_helper(string question, vector<vector<vector<triplet>>> &track_paragraph) {

    vector<string> *question_vector = sentence_to_word(question);
    vector<triplet> result;
    for (int i = 0; i < question_vector->size(); i++) {

        int n = 0;
        double word_score = score_word((*question_vector)[i]);
        Node *LL = temp.search((*question_vector)[i], n);
// cout<<"llllll"<<LL->book_code<<endl;

        while (LL != nullptr) {
// cout<<"llllll"<<LL->book_code<<endl;
            int book_code = LL->book_code;
            int page = LL->page;
            int paragraph = LL->paragraph;

            if (page >= track_paragraph[book_code].size()) {
                track_paragraph[book_code].resize(page , vector<triplet>(50));}  // Resize the book code vector}

                if (paragraph >= (track_paragraph[book_code][page].size())) {
                    (track_paragraph[book_code][page]).resize(paragraph + 1);
                }

                (track_paragraph[book_code][page][paragraph]).paragraph_contain_word = 1;
                track_paragraph[book_code][page][paragraph].frequency++;
                track_paragraph[book_code][page][paragraph].score += word_score;
                track_paragraph[book_code][page][paragraph].book_code1 = book_code;
                track_paragraph[book_code][page][paragraph].paragraph_no = paragraph;
                track_paragraph[book_code][page][paragraph].page_no = page;

            LL = LL->right;
// cout<<"i am at  the end of while loop"<<endl;
        }
    }
// cout<<"done with step 1 of query helper"<<endl;
    for (size_t i = 0; i < track_paragraph.size(); ++i) {
        for (size_t j = 0; j < track_paragraph[i].size(); ++j) {
            for (size_t k = 0; k < track_paragraph[i][j].size(); ++k) {
                if (track_paragraph[i][j][k].paragraph_contain_word == 1) {
                    result.push_back(track_paragraph[i][j][k]);
                }
            }
        }
    }    

// cout<<"done and going into sort"<<endl;

    mysort(result , 0 , result.size());

// cout<<"done and came out of sort"<<endl;
    return result;
}



void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence) {
    vector<string> * words = sentence_to_word(sentence);
    for (const string &word : *words) {
        if (!word.empty()) {
            temp.insert_sentence(book_code, page, paragraph, sentence_no, word);
        }
    }
}


double min1( int k , int l){
    if(k>l){
        return l;
    }
    else{
        return k ;
    }
}


Node* QNA_tool::get_top_k_para(string query,int k){
    vector < vector < vector < triplet > > > track_paragraph (100,vector < vector < triplet > > (800,vector < triplet > (50)));
    vector <triplet> abj = query_helper(query,track_paragraph);
// cout<<"done and came out of query"<<endl;
    Node* nill = new Node();
    Node* temp = nill;
    // for(double i = 0  ; i < (k )  ;i++ ){
    // double s_no = -1;
    // double off = -1;
    //     Node * n = new Node( abj[i].book_code1 , abj[i].page_no , abj[i].paragraph_no , s_no , off);
    //         temp->right=n;
    //         n->left=temp;
    //         temp=n;
    //         // cout << "Book_code: " << abj[i].book_code1 << " Page: " << abj[i].page_no << " Paragraph: " << abj[i].paragraph_no << endl;
    // }
    for (double i = 0; i < min1(k, abj.size()); i++) {
    double s_no = -1;
    double off = -1;
    Node* n = new Node(abj[i].book_code1, abj[i].page_no, abj[i].paragraph_no, s_no, off);
// cout<<" hey i " <<abj[i].book_code1<<" "<<abj[i].page_no<<" "<<abj[i].paragraph_no<<endl;
    temp->right = n;
    n->left = temp;
    temp = n;
}
    temp->right = NULL;

    return nill->right;
}



void QNA_tool::query(string question, string filename)
{
    int k = 5; // You can adjust this based on your needs
    Node* topKParagraphs = get_top_k_para(question, k);
// cout<<"query ... 1...."<<endl;


    // Open the file for writing
    std::ofstream outFile(filename);
// cout<<"query ... 2...."<<endl;
    // Check if the file is successfully opened
    if (!outFile.is_open()) {
// cout<<"query ... 3...."<<endl;
        std::cerr << "Error: Unable to open the output file " << filename << "." << std::endl;
        // You might want to handle this error in an appropriate way
        return;
    }
// cout<<"query ... 4...."<<endl;
    // Write the results to the file
    outFile << "Q: " << question << std::endl;
    outFile << "A: ";
// cout<<"query ... 5...."<<endl;
    // Write the top-k paragraphs to the file
    Node* current = topKParagraphs;
// cout<<"query ... 6...."<<endl;
    while (current != nullptr) {  
// cout<<"query ... 7...."<<endl;        
        std::string paragraphText = get_paragraph(current->book_code, current->page, current->paragraph);
// cout<<"query ... 8...."<<endl;        
        outFile << paragraphText << std::endl;
// cout<<"query ... 9...."<<endl;
        current = current->right;
    }
// cout<<"query ... 10...."<<endl;
    // query_llm( filename ,topKParagraphs , 5 ,  , question );

    // Close the file
    outFile.close();
}



std::string QNA_tool::get_paragraph(int book_code,int page,int paragraph){

    // cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}


void QNA_tool::query_llm(string filename, Node *root,int k, string API_KEY, string question)
{
    // first write the k paragraphs into different files

    Node* traverse = root;
    double num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}






// int main() {
//     QNA_tool qnaTool;

//     // Insert some sample sentences
    
    

//     qnaTool.insert_sentence(7, 1, 1, 1, "This is a sample sentence.");
//     qnaTool.insert_sentence(2, 2, 2, 2, "Another sample sentence for testing.");
//     qnaTool.insert_sentence(3, 3, 3, 3, "This sentence is in a different book.");

//     // Perform a sample query
//     string question = "sample ";
//     string filename = "output.txt";

//     qnaTool.get_top_k_para(question , 2);

//     // qnaTool.query(question, filename);

//     // // Display the results from the file
//     // ifstream resultFile(filename);
//     // if (resultFile.is_open()) {
//     //     string line;
//     //     while (getline(resultFile, line)) {
//     //         cout << line << endl;
//     //     }
//     //     resultFile.close();
//     // } else {
//     //     cout << "Error: Unable to open the result file." << endl;
//     // }

//     return 0;
// }



