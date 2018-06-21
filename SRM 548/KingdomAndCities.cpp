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

const int NX = 50 + 1, MX = NX * NX, M = 1e9 + 7;
class KingdomAndCities {
    int f[NX][NX], c[MX][MX];
	public:
	void prepro() {
		for (int i = 0; i < MX; i++) 
			for (int j = c[i][0] = 1; j <= i; j++)
				c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % M;
	}
	
	int e(int x) {
		return x * (x - 1) / 2;
	}
    int howMany(int n, int m, int k) {
    	prepro();
    	for (int i = 1; i <= n; i++) {
    		for (int j = 0; j <= k; j++) {
    			f[i][j] = c[e(i)][j];
    			for (int a = 1; a < i; a++) {
    				for (int b = 0; b <= j; b++) {
    					f[i][j] = (f[i][j] + M - (LL) c[i - 1][i - a] * c[e(i - a)][j - b] % M * f[a][b] % M) % M;
					}
				} 
			}
		}
		int ans = 0;
		if(k < m * 2) return 0;
		if(m == 0)  return f[n][k];
		
		if(m == 1) {
			ans = (LL) f[n - 1][k - 2] * c[n - 1][2] % M;
			for (int i = 1; i <= n - 2; i++) {
				int su = 0;
				for (int j = i - 1; j <= k - 2; j++) 
					su = (su + (LL) f[i][j] * f[n - i - 1][k - 2 - j]) % M;
				ans = (ans + (LL) su * i * (n - i - 1) % M * c[n - 2][n - i - 1]) % M;
			}
			return ans;
		}
		
		if(m == 2) {
			ans = (LL) f[n - 2][k - 3] * (n - 2) * (n - 2) % M;
			for (int i = 1; i <= n - 3; i++) {
				int su = 0;
				for (int j = i - 1; j <= k - 3; j++) 
					su = (su + (LL) f[i][j] * f[n - i - 2][k - 3 - j]) % M;
				ans = (ans + 2LL * i * (n - i - 2) * c[n - 3][n - i - 2] % M * su) % M;
			} 
			
			ans = (ans + (LL) f[n - 2][k - 4] * c[n - 2][2] * c[n - 2][2]) % M;
			for (int i = 1; i <= n - 3; i++) {
				int u = i * i * (n - i - 2) * (n - i - 2);
				int v = 2LL * i * (n - i - 2) * (c[i][2] + c[n - i - 2][2]) % M;
				u = (LL) (u + v) * c[n - 3][i - 1] % M;
				for (int j = i - 1; j <= e(i) && j <= k - 4; j++)
					ans = (ans + (LL) u * f[i][j] % M * f[n - i - 2][k - 4 - j]) % M;
			}
			
			for (int a = 1; a <= n - 3; a++) {
				for (int b = 1; a + b <= n - 3; b++) {
					int co = (LL) c[n - 3][a + b] * c[a + b][a] % M;
					int c = n - 2 - a - b, res = 0;
					for (int p = a - 1; p <= k - 4; p++) {
						for (int q = b - 1; q <= k - 4; q++) {
							int r = k - 4 - p - q; if(r < 0) break;
							res = (res + (LL) f[a][p] * f[b][q] % M * f[c][r]) % M;
						}
					}
					ans = (ans + (LL) co % M * a * b * c * (n - 2) % M * res) % M;
				}
			}
			return ans;
		}
		
		return 0;
    }
};

// CUT begin
ifstream data("KingdomAndCities.sample");

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

bool do_test(int N, int M, int K, int __expected) {
    time_t startClock = clock();
    KingdomAndCities *instance = new KingdomAndCities();
    int __result = instance->howMany(N, M, K);
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
        int N;
        from_stream(N);
        int M;
        from_stream(M);
        int K;
        from_stream(K);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if ( do_test(N, M, K, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1528250400;
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
        cout << "KingdomAndCities (1000 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
