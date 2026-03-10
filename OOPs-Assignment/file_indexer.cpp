#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cctype>

using namespace std;

/* ---------------- BUFFERED READER ---------------- */

class BufferedReader {
private:
    ifstream file;
    size_t bufSize;

public:

    BufferedReader(string path, int sizeKB)
    {
        bufSize = sizeKB * 1024;

        file.open(path);

        if(!file.is_open())
            throw runtime_error("File could not be opened");
    }

    bool readChunk(string &chunk)
    {
        vector<char> buffer(bufSize);

        file.read(buffer.data(), bufSize);

        int bytes = file.gcount();

        if(bytes <= 0)
            return false;

        chunk.assign(buffer.data(), bytes);

        return true;
    }
};


/* ---------------- TOKENIZER ---------------- */

class Tokenizer {

public:

    // convert text chunk into words
    vector<string> tokenize(string text)
    {
        vector<string> words;

        string w = "";

        for(char c : text)
        {
            if(isalnum(c))
            {
                w += tolower(c);
            }
            else
            {
                if(w.size() > 0)
                {
                    words.push_back(w);
                    w.clear();
                }
            }
        }

        if(w.size() > 0)
            words.push_back(w);

        return words;
    }
};


/* ---------------- INDEX STORAGE ---------------- */

class Indexer {

private:

    unordered_map<string, unordered_map<string,int>> data;

public:

    void build(string version, string filename, int buffer)
    {
        cout<<"Building index for version: "<<version<<endl;

        BufferedReader br(filename, buffer);

        Tokenizer tk;

        string chunk;

        while(br.readChunk(chunk))
        {
            vector<string> words = tk.tokenize(chunk);

            for(string w : words)
                data[version][w]++;
        }
    }

    int getCount(string version, string word)
    {
        return data[version][word];
    }

    vector<pair<string,int>> getTop(string version,int k)
    {
        vector<pair<string,int>> v(data[version].begin(), data[version].end());

        sort(v.begin(),v.end(),
             [](auto &a,auto &b){ return a.second > b.second; });

        if(v.size() > k)
            v.resize(k);

        return v;
    }
};


/* ---------------- BASE QUERY CLASS ---------------- */

class BaseQuery {

public:

    virtual void run() = 0;

};


/* ---------------- WORD QUERY ---------------- */

class WordQuery : public BaseQuery {

private:

    Indexer &idx;
    string version;
    string word;

public:

    WordQuery(Indexer &i,string v,string w)
        : idx(i), version(v), word(w) {}

    void run()
    {
        cout<<"Version: "<<version<<endl;

        cout<<"Word: "<<word<<endl;

        cout<<"Frequency: "<<idx.getCount(version,word)<<endl;
    }
};


/* ---------------- TOP-K QUERY ---------------- */

class TopQuery : public BaseQuery {

private:

    Indexer &idx;
    string version;
    int k;

public:

    TopQuery(Indexer &i,string v,int kk)
        : idx(i), version(v), k(kk) {}

    void run()
    {
        cout<<"Top "<<k<<" words in "<<version<<endl;

        auto res = idx.getTop(version,k);

        for(auto p : res)
            cout<<p.first<<" : "<<p.second<<endl;
    }
};


/* ---------------- DIFF QUERY ---------------- */

class DiffQuery : public BaseQuery {

private:

    Indexer &idx;
    string v1,v2;
    string word;

public:

    DiffQuery(Indexer &i,string a,string b,string w)
        : idx(i), v1(a), v2(b), word(w) {}

    void run()
    {
        int diff = idx.getCount(v2,word) - idx.getCount(v1,word);

        cout<<"Word: "<<word<<endl;

        cout<<"Difference ("<<v1<<" -> "<<v2<<") : "<<diff<<endl;
    }
};


/* ---------------- TEMPLATE FUNCTION ---------------- */

template<typename T>
void printValue(T x)
{
    cout<<x<<endl;
}


/* ---------------- OVERLOADED FUNCTION ---------------- */

void printResult(string label,int value)
{
    cout<<label<<" : "<<value<<endl;
}

void printResult(string label,double value)
{
    cout<<label<<" : "<<value<<endl;
}


/* ---------------- MAIN PROGRAM ---------------- */

int main(int argc,char* argv[])
{

    try
    {

        auto start = chrono::high_resolution_clock::now();

        string file,file1,file2;
        string version,version1,version2;
        string queryType;
        string word;

        int buffer = 512;
        int topk = 10;

        // parse command line arguments
        for(int i=1;i<argc;i++)
        {
            string a = argv[i];

            if(a=="--file") file = argv[++i];
            else if(a=="--file1") file1 = argv[++i];
            else if(a=="--file2") file2 = argv[++i];
            else if(a=="--version") version = argv[++i];
            else if(a=="--version1") version1 = argv[++i];
            else if(a=="--version2") version2 = argv[++i];
            else if(a=="--query") queryType = argv[++i];
            else if(a=="--word") word = argv[++i];
            else if(a=="--top") topk = stoi(argv[++i]);
            else if(a=="--buffer") buffer = stoi(argv[++i]);
        }

        Indexer idx;

        if(file!="")
            idx.build(version,file,buffer);

        if(file1!="" && file2!="")
        {
            idx.build(version1,file1,buffer);
            idx.build(version2,file2,buffer);
        }

        BaseQuery *q = nullptr;

        if(queryType=="word")
            q = new WordQuery(idx,version,word);

        else if(queryType=="top")
            q = new TopQuery(idx,version,topk);

        else if(queryType=="diff")
            q = new DiffQuery(idx,version1,version2,word);

        if(q)
        {
            q->run();
            delete q;
        }

        auto end = chrono::high_resolution_clock::now();

        double timeTaken =
        chrono::duration<double>(end-start).count();

        printResult("Buffer Size",buffer);
        printResult("Execution Time",timeTaken);

    }

    catch(exception &e)
    {
        cout<<"Error: "<<e.what()<<endl;
    }

    return 0;
}
