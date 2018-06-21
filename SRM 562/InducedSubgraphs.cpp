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

template <typename T> inline bool cmin(T & a, const T & b) { return a > b ? a = b, 1 : 0;}
template <typename T> inline bool cmax(T & a, const T & b) { return a < b ? a = b, 1 : 0;}

const int NX = 50, M = 1e9 + 9;
class InducedSubgraphs {
	public:
	int n, C[NX][NX]; vector<int> e[NX];
	
	int exp(int a, int n) {
		int r = 1;
		for (; n; n >>= 1) {
			if (n & 1)
				r = (LL) r * a % M;
			a = (LL) a * a % M;
		}
		return r;
	}
	
	int par[NX], siz[NX], ok[NX], chain;
	void dfs(int x, int k, bool kep = true, int dep = 1) {
		siz[x] = 1;
		for (int y : e[x]) {
			if(y == par[x]) continue;
			bool nk = kep && (SZ(e[x]) <= 2 || dep == 1);
			par[y] = x; dfs(y, k, nk, dep + 1); siz[x] += siz[y];
		}	
		ok[x] = (dep == chain && siz[x] == k && kep);
	}
	
	int vis[NX], f[NX];
	void dp(int x) {
		siz[x] = 0; vis[x] = f[x] = 1;
		for (int y : e[x]) {
			if(vis[y]) continue;
			dp(y); siz[x] += siz[y];
			f[x] = (LL) f[x] * f[y] % M * C[siz[x]][siz[y]] % M;
		}
		++siz[x];
	}
	
	int solve1(int k) {
		chain = n - k * 2 + 2;
		int ans = 0;
		for (int x = 1; x <= n; x++) {
			par[x] = 0;
			dfs(x, k);	
			for (int y = 1; y <= n; y++) {
				if(x == y || !ok[y]) continue;
				for (int z = 1; z <= n; z++)
					vis[z] = 0;
				for (int z = y; z; z = par[z])
					vis[z] = 1;
				dp(x); dp(y);
				ans = (ans + (LL) f[x] * f[y]) % M;
			}
		} 
		return ans;
	}
	
	int centre, cur_size;
	void find_centre(int x, int fa = 0) {
		int mx = 0; siz[x] = 1; par[x] = fa;
		for (int y : e[x]) {
			if (y == fa) continue;
			find_centre(y, x); cmax(mx, siz[y]); siz[x] += siz[y];
		}
		cmax(mx, n - siz[x]);
		if(cmin(cur_size, mx))
			centre = x;
	}
	
	int g[NX][NX][NX];
	
	void dp_(int x, int fa = 0) {
		g[x][0][0] = 1; siz[x] = 0;
		for (int y : e[x]) {
			if(y == fa) continue;
			dp_(y, x); static int h[NX][NX]; 
			memset(h, 0, sizeof(h));
			for (int a = 0; a <= siz[x]; a++) {
				for (int c = 0; c <= siz[y]; c++) {
					for (int b = 0; a + b <= siz[x]; b++) {
						for (int d = 0; c + d <= siz[y]; d++) {
							int w = (LL) C[a + c][a] * C[b + d][b] % M;
							h[a + c][b + d] = (h[a + c][b + d] + (LL) g[x][a][b] * g[y][c][d] % M * w) % M;  
						}
					}
				}
			}
			siz[x] += siz[y];
			for (int i = 0; i <= siz[x]; i++) {
				for (int j = 0; j <= siz[x]; j++) {
					g[x][i][j] = h[i][j];
				}
			}
		}
		siz[x]++;
		g[x][siz[x]][0] = g[x][siz[x] - 1][0];
		g[x][0][siz[x]] = g[x][0][siz[x] - 1];
	}
	int solve2(int k) {
		cur_size = n + 1;
		find_centre(1);
		dp_(centre);
	
		int ans = g[centre][n - k][n - k];
		for (int i = 1; i <= 2 * k - n; i++)
			ans = (LL) ans * i % M; 
		return ans;
	}	
	
	int getCount(vector<int> eL, vector<int> eR, int k) {
		n = SZ(eL) + 1;
		for (int i = 0; i < SZ(eL); i++) {
			++eL[i]; ++eR[i];
			e[eL[i]].pb(eR[i]);
			e[eR[i]].pb(eL[i]);
		}
		for (int i = 0; i <= n; i++) {
			for (int j = C[i][0] = 1; j <= i; j++)
				C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % M;
		}
		
		if (k == 1) {
			int fac = 1;
			for (int i = 1; i <= n; i++)
				fac = (LL) fac * i % M;
			return fac;
		}
		return 2 * k <= n ? solve1(k) : solve2(k);
	}
};

// CUT begin
ifstream data("InducedSubgraphs.sample");

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

bool do_test(vector<int> edge1, vector<int> edge2, int k, int __expected) {
    time_t startClock = clock();
    InducedSubgraphs *instance = new InducedSubgraphs();
    int __result = instance->getCount(edge1, edge2, k);
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
        vector<int> edge1;
        from_stream(edge1);
        vector<int> edge2;
        from_stream(edge2);
        int k;
        from_stream(k);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... \n";
        if ( do_test(edge1, edge2, k, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1528365241;
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
        cout << "InducedSubgraphs (1000 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
