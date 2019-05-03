#include <vector>
#include <math.h>
#include <nan.h>

using namespace v8;
using namespace Nan;
using namespace std;

struct Option
{
    unsigned num;
    bool rated;
};


class CollaborativeFiltering {
    private:
        vector<vector<double>> original;

    public:
        CollaborativeFiltering() {}

        vector<pair<int, double>> getRecommendations(vector<vector<double>> &scores, unsigned row_index, Option *option) {
            vector<pair<int, double>> result;
            if(scores.size() == 1) return result;

            for(unsigned i = 0, s = scores.size(); i < s; i++) {
                this->original.push_back(scores.at(i));
                if(i == row_index) {
                    this->centralize(scores.at(i));
                }
            }

            //获取邻居
            vector<pair<int, double>> neighborhood = this->getNeighborhood(scores, row_index);
            double similarity_sum = 0;
            for(unsigned i = 0, s = neighborhood.size(); i < s; i++) {
                similarity_sum += neighborhood.at(i).second;
            }

            //计算预测评分
            for(unsigned i = 0, s = scores.at(row_index).size(); i < s; i++) {
                if(!option->rated && scores.at(row_index).at(i) != 0) continue;

                double score_sum = 0;
                for(unsigned j = 0, n_s = neighborhood.size(); j < n_s; j++) {
                    score_sum += neighborhood.at(j).second * this->original.at(neighborhood.at(j).first).at(i);
                }

                result.push_back(make_pair(i, score_sum / similarity_sum));
            }

            struct compare {
                inline bool operator() (const pair<int, double> &a, const pair<int, double> &b) {
                    return a.second > b.second;
                }
            };
            sort(result.begin(), result.end(), compare());

            if(option->num > 0 && result.size() > option->num) {
                result.erase(result.begin() + option->num, result.end());
            }

            return result;
        }

        //求均值
        double mean(const vector<double> &v) {
            double sum = 0;
            int no_zero = 0;
            for(unsigned i = 0, s = v.size(); i < s; i++) {
                if(v.at(i) == 0) continue;
                sum += v.at(i);
                no_zero++;
            }

            return sum / no_zero;
        }

        //向量中心化
        void centralize(vector<double> &v) {
            double mean = this->mean(v);
            for(unsigned i = 0, s = v.size(); i < s; i++) {
                if(v.at(i) == 0) continue;
                v.at(i) -= mean;
            }
        }

        //平方和
        double squareSum(const vector<double> &v) {
            double result = 0;
            for(unsigned i = 0, s = v.size(); i < s; i++) {
                result += v.at(i) * v.at(i);
            }

            return result;
        }

        //点积
        double dotProduct(const vector<double> &x, const vector<double> &y) {
            double result = 0;
            for(unsigned i = 0, s = x.size(); i < s; i++) {
                result += x.at(i) * y.at(i);
            }
            
            return result;
        }

        //余弦相似度
        double cosineSimilarity(double dot_product, double x_square_sum, double y_square_sum) {
            if(dot_product == 0 || x_square_sum == 0 || y_square_sum == 0) {
                return 0;
            }

            return dot_product / (sqrt(x_square_sum) * sqrt(y_square_sum));
        }
        

        //获取邻居
        vector<pair<int, double>> getNeighborhood(const vector<vector<double>> &scores, unsigned row_index) {
            vector<pair<int, double>> similarities;
            double x_square_sum = this->squareSum(scores.at(row_index));
            for(unsigned i = 0, s = scores.size(); i < s; i++) {
                if(i == row_index) continue;

                vector<double> tmp_v(scores.at(i));
                this->centralize(tmp_v);
                double y_square_sum = this->squareSum(tmp_v);
                double dot_product = this->dotProduct(this->original.at(row_index), this->original.at(i));

                similarities.push_back(make_pair(i, this->cosineSimilarity(dot_product, x_square_sum, y_square_sum)));
            }

            struct compare {
                inline bool operator() (const pair<int, double> &a, const pair<int, double> &b) {
                    return a.second > b.second;
                }
            };
            sort(similarities.begin(), similarities.end(), compare());
            
            return similarities;
        }
};

class CollaborativeFilteringWorker: public AsyncWorker {
    private:
        vector<vector<double>> scores;
        int row_index;
        Option *option;
        CollaborativeFiltering *cf;
        vector<pair<int, double>> result;

    public:
        CollaborativeFilteringWorker(const vector<vector<double>> &scores, int row_index, Option *option, Callback *callback): AsyncWorker(callback) {
            this->scores = scores;
            this->row_index = row_index;
            this->option = option;
            this->cf = new CollaborativeFiltering();
        }

        //开始执行
        void Execute() {
            this->result = this->cf->getRecommendations(this->scores, this->row_index, this->option);
        }

        //执行结束
        void HandleOKCallback() {
            Local<Array> result = Nan::New<Array>();
            for(unsigned i = 0, s = this->result.size(); i < s; i++) {
                Local<Object> recom = Nan::New<Object>();
                recom->Set(Nan::New<String>("item_id").ToLocalChecked(), Nan::New<Number>(this->result.at(i).first));
                recom->Set(Nan::New<String>("score").ToLocalChecked(), Nan::New<Number>(this->result.at(i).second));

                result->Set(i, recom);
            }

            Local<Value> argv[] = { result };
            callback->Call(1, argv);
        }
};