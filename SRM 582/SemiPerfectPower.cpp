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


const int N3 = 501234;
const int N4 = 20000;

 
class SemiPerfectPower {
	public:
	LL sqr_root(LL x) {
		LL y = sqrt(x);
		while(y * y < x) ++y;
		while(y * y > x) --y;
		return y;
	}
	
	int mu[N3], c[N4]; VI square_free[N4]; vector<LL> su[N4];
	LL cub_root(LL x) {
		LL y = pow(x, 1.0 / 3);
		while(y * y * y < x) ++y;
		while(y * y * y > x) --y;
		return y;
	}
	
	void prepro() {
		mu[1] = 1;
		for (int i = 1; i < N3; i++) {
			for (int j = i << 1; j <= N3; j += i) {
				mu[j] -= mu[i];
			}
		}
		
		for (int i = 1; i < N4; i++)	
			if(mu[i]) {
				for (int j = i; j < N4; j += i) {
					square_free[j].pb(i);
				}
			}
		
		for (int i = 2, j; (j = i * i * i) < N4; i++) {
			for (int k = j; k < N4; k += j)
				c[k] = 1;
		}
		
		for (int i = 1; i < N4; i++) {
			if(mu[i]) {
				su[i].resize(N3 / i + 5);
				su[i][0] = 0;
				for (int j = 1; i * j < N3; ++j)
					su[i][j] = su[i][j - 1] + (mu[i * j] != 0);
			}
		}
	} 
	
	LL solve(LL n) {
		LL ans = 0;
		for (LL a = 1; a * a * a <= n; a++) {
			ans += (!!mu[a]) * (sqr_root(n / a) - a);
		} 
		for (LL a = 1; a * a * a * a <= n; a++) {
			if (c[a]) continue;
			for (LL b = 1; b * b * b <= a; b++) {
				LL g = __gcd(b * b, a), c = a / g, d = b * b / g, l = a / d, r = cub_root(n / a) / d;
				if (mu[c]) {
					for (auto e : square_free[c]) {
						ans += mu[e] * (su[e][r / e] - su[e][l / e]);
					}
				}
			}
		}
		return ans;
	}
	
	LL count(LL L, LL R) {
		prepro();		
		return solve(R) - solve(L - 1);
	}
};

// CUT begin
ifstream data("SemiPerfectPower.sample");

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

bool do_test(long long L, long long R, long long __expected) {
    time_t startClock = clock();
    SemiPerfectPower *instance = new SemiPerfectPower();
    long long __result = instance->count(L, R);
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
        long long L;
        from_stream(L);
        long long R;
        from_stream(R);
        next_line();
        long long __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... \n";
        if ( do_test(L, R, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1528434077;
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
        cout << "SemiPerfectPower (1000 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
