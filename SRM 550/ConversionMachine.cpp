#include <bits/stdc++.h>

#define X first
#define Y second
#define mp make_pair
#define pb push_back
#define SZ(x) ((int) (x).size())
#define ALL(x) x.begin(), x.end()
#define debug(x) cerr << #x" = " << x << endl

using namespace std;
typedef long long LL;
typedef pair<int, int> pii;
typedef unsigned long long u64;
typedef vector<int> VI;

const int NX = 13, MX = NX * NX, M = 1e9 + 7;

struct Matrix {
	int a[MX][MX];
	int* operator[] (int i) {
		return a[i];
	}
}o;

int p;
Matrix operator * (Matrix a, Matrix b) {
	Matrix c = o;
	for (int i = 0; i < p; i++)
		for (int j = 0; j < p; j++)
			for (int k = 0; k < p; k++)
				c[i][j] = (c[i][j] + (LL) a[i][k] * b[k][j]) % M;
	return c;
}

class ConversionMachine {
    public:
    
    int cnt[3], id[NX][NX];
    int countAll(string w1, string w2, vector<int> costs, int maxCost) {
        int n = SZ(w1); LL total = 0;

		for (int i = 0; i < n; i++) {
			int u = (w2[i] - w1[i] + 3) % 3;
			for (int k = 0; k < u; k++)
				total += costs[(w1[i] -'a' + k) % 3];
			++cnt[u];
		}

		if(total > maxCost) return 0;
		int maxOp = (maxCost - total) / (costs[0] + costs[1] + costs[2]) * 3 + cnt[1] + cnt[2] * 2 + 1;
		
		for (int i = p = 0; i <= n; i++)
			for (int j = 0; i + j <= n; j++) {
				id[i][j] = p++;
			}
		Matrix trans = o;
		
		for (int i = 0; i <= n; i++) {
			for (int j = 0; i + j <= n; j++) {
				if(i)
					trans[id[i][j]][id[i - 1][j]] = i;
				if(j)
					trans[id[i][j]][id[i + 1][j - 1]] = j;
				if(n > i + j)
					trans[id[i][j]][id[i][j + 1]] = n - i - j; 
			}
		}
		trans[0][p] = 1; trans[p][p] = 1; p++;
		
		Matrix res;
		for (int i = 0; i < p; i++) 
			for (int j = 0; j < p; j++)
				res[i][j] = (i == j);
		for (int t = maxOp; t; t >>= 1) {
			if(t & 1)
				res = res * trans;
			trans = trans * trans;
		}

		int init = id[cnt[1]][cnt[2]];
		return res[init][--p];
    }
};

// CUT begin
ifstream data("ConversionMachine.sample");

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

template <typename T> void from_stream(vector<T> &ts) {
    int len;
    from_stream(len);
    ts.clear();
    for (int i = 0; i < len; ++i) {
        T t;
        from_stream(t);
        ts.push_back(t);
    }
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

bool do_test(string word1, string word2, vector<int> costs, int maxCost, int __expected) {
    time_t startClock = clock();
    ConversionMachine *instance = new ConversionMachine();
    int __result = instance->countAll(word1, word2, costs, maxCost);
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
        string word1;
        from_stream(word1);
        string word2;
        from_stream(word2);
        vector<int> costs;
        from_stream(costs);
        int maxCost;
        from_stream(maxCost);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... \n";
        if ( do_test(word1, word2, costs, maxCost, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1528266494;
        double PT = T / 60.0, TT = 75.0;
        cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
        cout << "Score  : " << 850 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
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
        cout << "ConversionMachine (850 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
