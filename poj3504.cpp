#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <queue>
#include <stack>
#include <deque>

using namespace std;
typedef long long ll;
typedef long double ld;
const int inf = 0x3f3f3f3f;
map<char, ll> hval;
const int maxn = 10005;
const int maxprime = 200025;
const ll moder = 1e9 + 7 ;
const ll Radix = 10007;
int lennum[27];
int lenss[27][maxn];
bool nprime[maxprime];
void init()
{
    nprime[0] = true;
    nprime[1] = true;
    nprime[2] = false;
    for (int i = 2; i < maxprime; i++)
    {
        if (!nprime[i])
        {
            for (int j = i + i; j < maxprime; j+= i)
            {
                nprime[i] = true;
            }
        }
    }
    char hkey = 127;
    for (int k = 100010; k < maxprime && hkey; k++)
    {
        if (!nprime[k])
        {
            hval[hkey--] = k;
        }
    }
}

ll Hash[maxn];
ll hpower[maxn];

char S[maxn];
int n;
void build()
{
    Hash[0] = 0;
    hpower[0] = 1;
    for (int i = 0; i < n; i++)
    {
        Hash[i+1] = (Hash[i] + hval[S[i]]) % moder;
        hpower[i+1] = hpower[i] * Radix % moder;
    }
}
//i,j inclusive
ll get(int i, int j)
{
    if (i == j)
    {
        return S[j];
    }
    ll ret = (Hash[j ] - Hash[i + 1] + moder )% moder;
    ret = (ret + S[i] * hpower[0] + S[j] * hpower[j - i + 1]) % moder;
    return ret;
}
char wd[maxn][200];
map<ll, int> htable[27][27];
map<ll, int> h2str;
int mem[maxn];
int pre[maxn];
int dpsearch()
{
    for (int i = n - 1; i >= 0; i--)
    {
        int idx = S[i] - 'a';
        int sz = lennum[idx];
        int ans = 0;
        for (int j = 0; j < sz; j++)
        {
            int ed = i + lenss[idx][j] - 1;
            int idy = S[ed] - 'a';
            if (!mem[ed + 1] || !htable[idx][idy].size()) continue;
            if (ed >= n)
            {
                break;
            }
            ll hh = get(i, ed);
            if (htable[idx][idy].find(hh) != htable[idx][idy].end())
            {
                ans += htable[idx][idy][hh] * mem[ed + 1];
                pre[ed + 1] = i;
                if (ans > 1)
                {
                    break;
                }
            }
        }
        mem[i] = ans;
    }
    return mem[0];
}

int main()
{
    int T;
    init();
    while(scanf("%d",&T)!=EOF)
    {
        while(T--)
        {
            memset(lennum, 0, sizeof(lennum));
            scanf("%s", S);
            n = strlen(S);
            build();
            for (int i = 0; i < 27; i++)
            for (int j = 0; j < 27; j++) htable[i][j].clear();
            h2str.clear();
            int wdnum;
            scanf("%d", &wdnum);
            for (int i = 0; i < wdnum; i++)
            {
                scanf("%s", wd[i]);
                int m = strlen(wd[i]);
                int idx = wd[i][0] - 'a';
                int idy = wd[i][m - 1] - 'a';
                lenss[idx][lennum[idx]++] = m;
                if (m == 1)
                {
                    htable[idx][idy][wd[i][0]]++;
                    h2str[wd[i][0]] = i;
                    continue;
                }
                if (m > n)
                {
                    continue;
                }
                ll hh = (wd[i][0] * 1LL + wd[i][m - 1] * hpower[m]) % moder;
                for (int j = 1; j + 1 < m; j++)
                {
                    hh = (hval[wd[i][j]] + hh) % moder;
                }
                htable[idx][idy][hh]++;
                h2str[hh] = i;
            }
            for (int i = 0; i < 27; i++) sort(lenss[i], lenss[i] + lennum[i]);
            for (int i = 0; i < 27; i++) lennum[i] = unique(lenss[i], lenss[i] + lennum[i]) - lenss[i];
            memset(mem, -1, sizeof(mem));
            mem[n] = 1;
        //    int ans = dfs(0);
            int ans = dpsearch();
            if (ans == 0)
            {
                printf("impossible\n");
            }
            else if (ans > 1)
            {
                printf("ambiguous\n");
            }
            else
            {
                int p = n;
                vector<ll> ans;
                while(p)
                {
                    ll hh = get(pre[p], p - 1);
                    ans.push_back(hh);
                    p = pre[p];
                }
                reverse(ans.begin(), ans.end());
                for (int i = 0; i < ans.size(); i++)
                {
                    printf("%s%c", wd[h2str[ans[i]]], i != ans.size() - 1 ? ' ' : '\n');
                }
    //            printf("\n");
            }
        }
    }
}
