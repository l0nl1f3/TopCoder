#include <bits/stdc++.h>

#define fir first
#define sec second
using namespace std;
typedef long long LL;

template <typename T> inline bool cmin(T & a, const T & b) { return a > b ? a = b, 1 : 0;}
template <typename T> inline bool cmax(T & a, const T & b) { return a < b ? a = b, 1 : 0;}

const int P = 998244353;

int inv[20];
int exp(int a, int n) {
	int res = 1;
	for(; n; n >>= 1) {
		if(n & 1)
			res = (LL) res * a % P;
		a = (LL) a * a % P;
	}
	return res;
}
void init() {
	inv[0] = 1;
	for(int i = 1; i < 20; i++) 
		inv[i] = exp(i, P - 2);
}
   
int C(int n, int m) {
	if(n < 0 || m < 0 || n < m) return 0;
	int res = 1;
	for(int i = 0; i < m; i++) 
		res = (LL) res * (n - i) % P * inv[i + 1] % P;
	return res;
}
	
using Four = tuple<int, int, int, int>;

map<Four, int> g[2][1 << 6];
int id[6][6], f[1 << 15];

int pcnt(int x) {
	return !x ? -1 : 31 - __builtin_clz(x);
}

int add(int &a, int b) {
	(a += b) %= P;
}

int n, m, K;
void trans(int p, int ub, int op) {
	int o, i, j, k;
	for(o = 0; o < ub; o++) {
		for(const auto &it : g[p][o]) {
			int l, r, a, b; tie(l, r, a, b) = it.fir;
			int way = it.sec;
			int pre = pcnt(a);
			int stat = 0;
			for(i = 0; i < m; i++)
				if(l >> i & 1)
					if((!(r >> i & 1)) || (a >> i & 1))
						stat |= (1 << i);
			
			if(op) {
				for(i = 0; i < m; i++) 
					if(!(l >> i & 1)) {
						if(pre >= i) continue;
						int t = b;
						for(j = 0; j < m; j++)
							if(stat >> j & 1) t |= (1 << id[i][j]);
						Four to(l | (1 << i), r, a | (1 << i), t); 
						add(g[p ^ 1][o][to], way);
					}else if(!(r >> i & 1)) {
						if(pre >= i) continue;
						Four to(l, r | (1 << i), a | (1 << i), b);
						add(g[p ^ 1][o][to], way);
					}
			}
			if(~ pre) {
				Four to(l, r, 0, b);
				add(g[p][o + 1][to], way);
			}
		}
	}
}
void calc(int n, int m) {
	int i, j, p = 0;
	for(i = 0; i < m; i++)
		for(j = i + 1; j < m; j++)
			id[i][j] = id[j][i] = p++;
	
	Four o(0, 0, 0, 0); g[0][0][o] = 1;
	int ub = (m << 1 | 1);
	
	for(p = 0; p < ub; p++) {
		for(i = 0; i < ub; i++) g[~p & 1][i].clear();
		trans(p & 1, ub, p != ub - 1);
	}
	
	memset(f, 0, sizeof(f));
	for(p = 0; p < ub; p++) 
		for(const auto &it : g[0][p]) {
			int l, r, a, b;
			tie(l, r, a, b) = it.fir; int way = it.sec;
			if(a) continue;
			add(f[b], (LL) C(n, p) * way % P);
		}
}

//and convolution
void FwT(int n, int f[]) {
	for(int j = 0; j < n; j++)
		for(int i = 0; i < (1 << n); i++)
			if(~i >> j & 1)
				f[i] = (f[i] + f[i | (1 << j)]) % P; 
}

void iFwT(int n, int f[]) {
	for(int j = 0; j < n; j++)
		for(int i = 0; i < (1 << n); i++)	
			if(~i >> j & 1)
				f[i] = (f[i] - f[i ^ (1 << j)] + P) % P;
}
class Hyperboxes {
    public:
    
    int findCount(int n, int m, int k) {
		:: n = n; :: m = m; :: K = k;
    	init();
		calc(n, m);
		int mL = m * (m - 1) >> 1; 
		FwT(mL, f);
		for(int i = 0; i < (1 << mL); i++) f[i] = exp(f[i], K);
		iFwT(mL, f);
        return f[0];
    }
};

// CUT begin
ifstream data("Hyperboxes.sample");

string next_line() {
    string s;
    getline(data, s);
    return s;
}

template <typename T> void from_stream(T &t) {
    stringstream ss(next_line());
    ss >> t;
}

void from_stream(string &s) {
    s = next_line();
}

template <typename T>
string to_string(T t) {
    stringstream s;
    s << t;
    return s.str();
}

string to_string(string t) {
    return "\"" + t + "\"";
}

bool do_test(int n, int m, int k, int __expected) {
    time_t startClock = clock();
    Hyperboxes *instance = new Hyperboxes();
    int __result = instance->findCount(n, m, k);
    double elapsed = (double)(clock() - startClock) / CLOCKS_PER_SEC;
    delete instance;

    if (__result == __expected) {
        cout << "PASSED!" << " (" << elapsed << " seconds)" << endl;
        return true;
    }
    else {
        cout << "FAILED!" << " (" << elapsed << " seconds)" << endl;
        cout << "           Expected: " << to_string(__expected) << endl;
        cout << "           Received: " << to_string(__result) << endl;
        return false;
    }
}

int run_test(bool mainProcess, const set<int> &case_set, const string command) {
    int cases = 0, passed = 0;
    while (true) {
        if (next_line().find("--") != 0)
            break;
        int n;
        from_stream(n);
        int m;
        from_stream(m);
        int k;
        from_stream(k);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if ( do_test(n, m, k, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1503412186;
        double PT = T / 60.0, TT = 75.0;
        cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
        cout << "Score  : " << 900 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    set<int> cases;
    bool mainProcess = true;
    for (int i = 1; i < argc; ++i) {
        if ( string(argv[i]) == "-") {
            mainProcess = false;
        } else {
            cases.insert(atoi(argv[i]));
        }
    }
    if (mainProcess) {
        cout << "Hyperboxes (900 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
