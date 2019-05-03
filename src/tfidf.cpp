#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <regex>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <nan.h>
#include "./include/NLPIR.h"
#include "./include/KeyExtract.h"

using namespace std;
using namespace Nan;
using namespace v8;

class TFIDF {
    private:
        vector<vector<string>> docs_worlds;
        vector<string> query_worlds;
        vector<string> ignore_world{ "，", "。", "[", "]", "-", "：" };

    public:
        TFIDF() {}
    
        //获取关键字
        vector<pair<string, double>> getKeywords(const vector<string> &docs, const string &query) {
            // map<string, double> tmp;
            vector<pair<string, double>> result;

            const string path = "/home/sagewu/Documents/nodejs/recommend/src";
            // if (!NLPIR_Init(path.c_str(), UTF8_CODE, ""))
            // {
            //     printf("初始化失败\n");
            //     return result;
            // }

            if(!KeyExtract_Init(path.c_str(), UTF8_CODE, ""))
            {
                printf("初始化失败\n");
                return result;
            }

            string keywords(KeyExtract_GetKeyWords(query.c_str(), 10, true));
            stringstream stream(keywords);
            string item;
            regex rgx("([^/]*)/([^/]*)/([^/]*)/([^/]*)");
            smatch match;
            while(getline(stream, item, '#')) {
                regex_match(item, match, rgx);
                result.push_back(make_pair(match[1], atof(match[3].str().c_str())));
            }
            stream.clear();

            // this->query_worlds = this->split(query);
            // for(unsigned i = 0, s = docs.size(); i < s; i++) {
            //     this->docs_worlds.push_back(this->split(docs.at(i)));
            // }

            // NLPIR_Exit();
            // KeyExtract_Exit();

            // for(unsigned i = 0, s = this->query_worlds.size(); i < s; i++) {
            //     string term = this->query_worlds.at(i);
            //     tmp[term] += this->tfidf(term);
            // }

            // for(const pair<string, double> &r: tmp) {
            //     result.push_back(r);
            // }

            // struct compare {
            //     inline bool operator() (const pair<string, double> &a, const pair<string, double> &b) {
            //         return a.second > b.second;
            //     }
            // };
            // sort(result.begin(), result.end(), compare());

            return result;
        }

        //分词
        vector<string> split(const string &str) {
            vector<string> result;
            string tmp = NLPIR_ParagraphProcess(str.c_str(), 0);

            stringstream stream(tmp);
            string world;
            while(stream >> world) {
                if(find(this->ignore_world.begin(), this->ignore_world.end(), world) != this->ignore_world.end()) {
                    continue;
                }
                // transform(world.begin(), world.end(), world.begin(), ::tolower);
                result.push_back(world);
            }

            return result;
        }

        //计算tfidf
        double tfidf(const string &term) {
            unsigned c = count(this->query_worlds.begin(), this->query_worlds.end(), term);
            double tf = c / (double)this->query_worlds.size();

            unsigned c_doc = 0;
            for(unsigned i = 0, s = this->docs_worlds.size(); i < s; i++) {
                vector<string> doc_worlds = this->docs_worlds.at(i);
                bool is_exist = false;
                for(unsigned j = 0, s = doc_worlds.size(); j < s; j++) {
                    if(doc_worlds.at(j) == term) {
                        is_exist = true;
                        break;
                    }
                }

                if(is_exist) {
                    c_doc++;
                }
            }
            double idf = log(this->docs_worlds.size()  / (double)c_doc);
            idf++;

            return tf * idf;
        }
};

class TFIDFWorker: public AsyncWorker {
    private:
        vector<string> docs;
        string query;
        TFIDF *tfidf;
        vector<pair<string, double>> result;

    public:
        TFIDFWorker(const vector<string> &docs, const string &query, Callback *callback): AsyncWorker(callback) {
            this->docs = docs;
            this->query = query;
            this->tfidf = new TFIDF();
        }

        void Execute() {
            this->result = this->tfidf->getKeywords(this->docs, this->query);
        }

        void HandleOKCallback() {
            Local<Array> result = Nan::New<Array>();

            for(const pair<string, double> &r: this->result) {
                Local<Object> tfidf = Nan::New<Object>();
                tfidf->Set(Nan::New<String>("keyword").ToLocalChecked(), Nan::New<String>(r.first).ToLocalChecked());
                tfidf->Set(Nan::New<String>("score").ToLocalChecked(), Nan::New<Number>(r.second));

                result->Set(result->Length(), tfidf);
            }

            Local<Value> argv[] = { result };
            callback->Call(1, argv);
        }
};