#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int n;
vector<pair<int,int>> adX,adY;
vector<int> x, y, r;

inline bool isOverlap1D(int x1, int y1, int x2, int y2) {
    return x1 < y2 && x2 < y1;
}

inline bool isOverlap2D(int x11, int y11, int x12, int y12, int x21, int y21, int x22, int y22) {
    return isOverlap1D(x11,x12,x21,x22) && isOverlap1D(y11,y12,y21,y22);
}

inline bool isOverlapAd(int i, int j) {
    return isOverlap2D(adX[i].first, adY[i].first, adX[i].second, adY[i].second, adX[j].first, adY[j].first, adX[j].second, adY[j].second);
}

bool isOverlap() {
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (isOverlapAd(i,j)) return true;
        }
    }
    return false;
}

bool isOverlap(int i) {
    for (int j = 0; j < n; j++) {
        if (i == j) continue;
        if (isOverlapAd(i,j)) return true;
    }
    return false;
}

inline bool isInRange(int i) {
    return !(adX[i].first < 0 || adY[i].first < 0 || adX[i].second > 10000 || adY[i].second > 10000);
}

inline bool isSquare(int i) {
    return adX[i].first < adX[i].second && 
           adY[i].first < adY[i].second && 
           adX[i].first <= x[i] && 
           adX[i].second > x[i] &&
           adY[i].first <= y[i] &&
           adY[i].second > y[i];
} 

bool isSatisfied() {
    if (isOverlap()) return false;
    for (int i = 0; i < n; i++) {
        if (!isInRange(i)) {
            return false;
        }
    }
    return true;
}

void output() {
    for(int i = 0; i < n; i++) {
        cout << adX[i].first << " " << adY[i].first << " " << adX[i].second << " " << adY[i].second << endl;
    }
}

double getOneScore(int id) {
    int S = (adX[id].second-adX[id].first)*(adY[id].second-adY[id].first);
    if (adX[id].first <= x[id] && adY[id].first <= y[id] && x[id] < adX[id].second && y[id] < adY[id].second) {
        return 1-(1-(double)min(r[id],S)/max(r[id],S))*(1-(double)min(r[id],S)/max(r[id],S));
    }
    return 0;
}

double getScore() {
    double score = 0;
    for (int i = 0; i < n; i++) {
        score += getOneScore(i);
    }
    return 1e9*score/n;
}

const int time_limit = 4900;

struct Timer {
private:
    std::chrono::system_clock::time_point  s, e;
public:
    Timer(){}
    void start(){
        s = std::chrono::system_clock::now();
    }
    // ms
    double time() {
        e = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(e-s).count();
    }
};

struct RandGen {
    using T = int;
    std::default_random_engine generator;
    std::uniform_int_distribution<T> distribution; // 一様整数分布[l,r]
    // std::uniform_real_distribution<T> distribution; // 一様実数分布
    // std::normal_distribution<T> distribution; // 正規分布
    RandGen(T x, T y) : generator(time(nullptr)), distribution(x,y) {}
    T rand() {
        return distribution(generator);
    }
};

struct RandGenProb {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution; // 一様実数分布
    RandGenProb() : generator(time(nullptr)), distribution(0.0,1.0) {}
    double rand() {
        return distribution(generator);
    }
};

int main() {
    Timer timer;
    timer.start();
    cin >> n;
    x.resize(n);
    y.resize(n);
    r.resize(n);
    adX.resize(n);
    adY.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i] >> r[i];
        adX[i].first = x[i];
        adY[i].first = y[i];
        adX[i].second = x[i]+1;
        adY[i].second = y[i]+1;
    }
    RandGen rand_ad(0,n-1);
    RandGen rand_dir(-1000,1000);
    RandGenProb rand_prob;
    double PreScore = getScore();
    int start_time = timer.time();
    while (true) {
        int now_time = timer.time();
        if (now_time - start_time >= time_limit) break;
        int ad_id = rand_ad.rand();
        int dx1 = rand_dir.rand() * (1 - (now_time - start_time) / time_limit);
        int dx2 = rand_dir.rand() * (1 - (now_time - start_time) / time_limit);
        int dy1 = rand_dir.rand() * (1 - (now_time - start_time) / time_limit);
        int dy2 = rand_dir.rand() * (1 - (now_time - start_time) / time_limit);
        int pre_adx1 = adX[ad_id].first;
        int pre_adx2 = adX[ad_id].second;
        int pre_ady1 = adY[ad_id].first;
        int pre_ady2 = adY[ad_id].second;
        double NewScore = PreScore - 1e9*getOneScore(ad_id)/n;

        adX[ad_id].first += dx1;
        adX[ad_id].second += dx2;
        adY[ad_id].first += dy1;
        adY[ad_id].second += dy2;
        if (!isSquare(ad_id) || !isInRange(ad_id) || isOverlap(ad_id)) {
            adX[ad_id].first = pre_adx1;
            adX[ad_id].second = pre_adx2;
            adY[ad_id].first = pre_ady1;
            adY[ad_id].second = pre_ady2;
            continue;
        }

        NewScore += 1e9*getOneScore(ad_id)/n;
        // double prob = (now_time - start_time) / time_limit;
        double prob = 1; // 山登り
        if (NewScore > PreScore || rand_prob.rand() > prob) {
            PreScore = NewScore;
            output();
        }
        else {
            adX[ad_id].first = pre_adx1;
            adX[ad_id].second = pre_adx2;
            adY[ad_id].first = pre_ady1;
            adY[ad_id].second = pre_ady2;
        }
    }
    return 0;
}