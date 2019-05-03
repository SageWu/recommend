#include <vector>
#include <nan.h>

#include "src/collaborative-filtering.cpp"
#include "src/tfidf.cpp"

using namespace v8;
using namespace Nan;
using namespace std;

//协同过滤推荐列表
void getCFRecommendations(const Nan::FunctionCallbackInfo<Value>& info) {
    //检查输入
    if(!info[0]->IsArray()) Nan::ThrowError("scores must be matrix");
    if(!info[1]->IsNumber()) Nan::ThrowError("row index must be number");
    if(!info[2]->IsObject()) Nan::ThrowError("option must be object");

    //对输入数据进行转换
    vector<vector<double>> scores;
    Local<Array> tmp_scores = info[0].As<Array>();
    for(unsigned i = 0, l = tmp_scores->Length(); i < l; i++) {
        vector<double> row;
        Local<Array> tmp_row = tmp_scores->Get(i).As<Array>();
        for(unsigned j = 0; j < tmp_row->Length(); j++) {
            row.push_back(tmp_row->Get(j)->NumberValue());
        }

        scores.push_back(row);
    }

    int row_index = info[1]->IntegerValue();
    if(row_index < 0 || row_index >= (int)scores.size()) Nan::ThrowError("row index is out of range");

    Local<Object> tmp_option = info[2].As<Object>();
    Option option;
    option.num = tmp_option->Get(Nan::New<String>("num").ToLocalChecked())->IntegerValue();
    option.rated = tmp_option->Get(Nan::New<String>("rated").ToLocalChecked())->BooleanValue();

    if(info[3]->IsFunction()) { //异步
        Callback *callback = new Callback(info[3].As<Function>());
        AsyncQueueWorker(new CollaborativeFilteringWorker(scores, row_index, &option, callback));
    }
    else {  //同步
        CollaborativeFiltering *cf = new CollaborativeFiltering();
        vector<pair<int, double>> recoms = cf->getRecommendations(scores, row_index, &option);

        Local<Array> result = Nan::New<Array>();
        for(unsigned i = 0, s = recoms.size(); i < s; i++) {
            Local<Object> recom = Nan::New<Object>();
            recom->Set(Nan::New<String>("item_id").ToLocalChecked(), Nan::New<Number>(recoms.at(i).first));
            recom->Set(Nan::New<String>("score").ToLocalChecked(), Nan::New<Number>(recoms.at(i).second));

            result->Set(i, recom);
        }

        return info.GetReturnValue().Set(result);
    }
}

void tfidf(const Nan::FunctionCallbackInfo<Value> &info) {
    if(!info[0]->IsArray() || !info[0].As<Array>()->Get(0)->IsString()) Nan::ThrowError("documents must be array of string");
    if(!info[1]->IsString()) Nan::ThrowError("query must be string");

    Local<Array> tmp_docs = info[0].As<Array>();
    vector<string> docs;
    for(unsigned i = 0, l = tmp_docs->Length(); i < l; i++) {
        // String::Utf8Value tmp_doc(tmp_docs->Get(i)->ToString());
        string str = *Utf8String(tmp_docs->Get(i));
        docs.push_back(str);
    }

    // Local<String> tmp_query = info[1]->ToString();
    string query = *Utf8String(info[1]);

    if(info[2]->IsFunction()) {
        Callback *callback = new Callback(info[2].As<Function>());
        AsyncQueueWorker(new TFIDFWorker(docs, query, callback));
    }
    else {

    }
}

void init(Handle<Object> exports, Handle<Object> module) {
    exports->Set(Nan::New<String>("getCFRecommendations").ToLocalChecked(), Nan::New<FunctionTemplate>(getCFRecommendations)->GetFunction());
    exports->Set(Nan::New<String>("tfidf").ToLocalChecked(), Nan::New<FunctionTemplate>(tfidf)->GetFunction());
}

NODE_MODULE(recommend, init)