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

const int NX = 41, MX = NX >> 1, M = 1e9 + 7;
class SweetFruits {
    public:
    VI a[NX], b[NX], u; LL cnt[NX]; int x[NX][NX], c[NX][NX], T[NX];
	void dfs1(int x, int y, int v, int p) {
		if(x == p) {
			a[y].pb(v);	
		} else {
			dfs1(x + 1, y + 1, v + u[x], p);
			dfs1(x + 1, y, v, p);
		}
	} 
	
	void dfs2(int x, int y, int v, int p) {
		if (x == p) {
			b[y].pb(v); 
		} else {
			dfs2(x + 1, y + 1, v + u[x], p);
			dfs2(x + 1, y, v, p);
		}
	}
		
	int exp(int a, int n) {
		int r = 1;
		for (; n; n >>= 1) {
			if (n & 1)
				r = (LL) r * a % M;
			a = (LL) a * a % M;
		}
		return r;
	}
	
	int get_det(int n, int x[NX][NX]) {
		int ans = 1;
		for (int i = 1, j; i < n; i++) {
			for (j = i; j < n; j++)
				if(x[j][i]) break;
			if(j > i) {
				for (int k = i; k < n; k++)
					swap(x[i][k], x[j][k]);
				ans = M - ans;
			}
			int tmp = exp(x[i][i], M - 2); ans = (LL) ans * x[i][i] % M;
			for (int j = i; j < n; j++)
				x[i][j] = (LL) x[i][j] * tmp % M;
			for (int j = i + 1; j < n; j++)
				for (int k = n - 1; k >= i; k--)
					x[j][k] = (x[j][k] + M - (LL) x[j][i] * x[i][k] % M) % M;
		}	
		return ans;
	}
	
    int countTrees(vector<int> v, int mx) {
		for (auto x:v) if(~x) u.pb(x); 
	    
		int m = SZ(u), nl = m >> 1;
    	dfs1(0, 0, 0, nl);
    	dfs2(nl, 0, 0, m);
    	int nr = m - nl;
    	for (int i = 0; i <= nl; i++)
    		sort(ALL(a[i]));
    	for (int i = 0; i <= nr; i++)
			sort(ALL(b[i]));
		for (int p = 0; p <= nl; p++) {
			for (int q = 0; q <= nr; q++) {
				for (int x = 0, y = SZ(b[q]) - 1; x < SZ(a[p]); x++) {
					while(y >= 0 && b[q][y] + a[p][x] > mx)
						--y;
					cnt[p + q] += y + 1;					
				}
			}
		} 
		
		int n = SZ(v);
		for (int i = 0; i <= n; i++) {
			for (int j = c[i][0] = 1; j <= i; j++)
				c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % M;
		}
		
		int ans = 0;
		for (int k = 0; k <= m; k++) {
			for (int i = 1; i <= n; i++) {
				for (int j = 1; j <= n; j++) {
					x[i][j] = 0;
				}
			}
			for (int i = 1; i <= n; i++) {
				for (int j = i + 1; j <= n; j++) {
					if(j > m || (i <= k && j <= k)) {
						--x[i][j]; --x[j][i];
						++x[i][i]; ++x[j][j];
					}
				}
			}
			T[k] = get_det(n, x); T[k] = (T[k] + M) % M;
			for (int i = 0; i < k; i++)
				T[k] = (T[k] + M - (LL) c[k][i] * T[i] % M) % M;
			ans = (ans + cnt[k] % M * T[k]) % M;
		}
			
		return ans;
    }
};

// CUT begin
ifstream data("SweetFruits.sample");

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

bool do_test(vector<int> sweetness, int maxSweetness, int __expected) {
    time_t startClock = clock();
    SweetFruits *instance = new SweetFruits();
    int __result = instance->countTrees(sweetness, maxSweetness);
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
        vector<int> sweetness;
        from_stream(sweetness);
        int maxSweetness;
        from_stream(maxSweetness);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if ( do_test(sweetness, maxSweetness, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1528279699;
        double PT = T / 60.0, TT = 75.0;
        cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
        cout << "Score  : " << 1000 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
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
        cout << "SweetFruits (1000 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
