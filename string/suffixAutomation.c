// suffix_automaton.c
// Build suffix automaton for ascii strings, compute:
//  - number of distinct substrings
//  - occurrences (end-pos counts) per state
//  - longest common substring (LCS) with another string

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXLEN 200000   // adjust for expected maximum string length
#define ALPH 128        // ASCII alphabet size (0..127)
#define MAXST (MAXLEN * 2)

typedef struct State {
    int next[ALPH]; // transitions, -1 if none
    int link;       // suffix link
    int len;        // max length of substring in this state
    long long occ;  // number of end positions (occurrence count)
} State;

State *st;
int sz;    // number of states
int last;  // last state (for extension)

void sa_init(int maxlen) {
    st = (State*)malloc(sizeof(State) * (2 * maxlen + 5));
    if (!st) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    sz = 1;
    last = 0;
    for (int i = 0; i < ALPH; ++i) st[0].next[i] = -1;
    st[0].len = 0;
    st[0].link = -1;
    st[0].occ = 0;
}

void sa_extend(int c) {
    int cur = sz++;
    for (int i = 0; i < ALPH; ++i) st[cur].next[i] = -1;
    st[cur].len = st[last].len + 1;
    st[cur].occ = 1; // new end position
    int p = last;
    while (p != -1 && st[p].next[c] == -1) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            for (int i = 0; i < ALPH; ++i) st[clone].next[i] = st[q].next[i];
            st[clone].len = st[p].len + 1;
            st[clone].link = st[q].link;
            st[clone].occ = 0; // clones initially 0 occurrences (we will propagate)
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

// propagate occ counts from longer states to shorter (topological by len)
void sa_propagate_occ() {
    // bucket states by length
    int maxlen = 0;
    for (int i = 0; i < sz; ++i) if (st[i].len > maxlen) maxlen = st[i].len;
    int *cnt = (int*)calloc(maxlen + 1, sizeof(int));
    for (int i = 0; i < sz; ++i) cnt[st[i].len]++;
    for (int i = 1; i <= maxlen; ++i) cnt[i] += cnt[i-1];
    int *order = (int*)malloc(sizeof(int) * sz);
    for (int i = sz - 1; i >= 0; --i) order[--cnt[st[i].len]] = i;
    free(cnt);
    // iterate from longest to shortest
    for (int i = sz - 1; i > 0; --i) {
        int v = order[i];
        int p = st[v].link;
        if (p != -1) st[p].occ += st[v].occ;
    }
    free(order);
}

// compute number of distinct substrings = sum( len[v] - len[link[v]] ) over v != 0
long long sa_count_distinct_substrings() {
    long long ans = 0;
    for (int v = 1; v < sz; ++v) {
        ans += (long long)(st[v].len - st[st[v].link].len);
    }
    return ans;
}

// longest common substring between base automaton (built for s1) and s2
// returns length and optionally end position in s2
int sa_longest_common_substring(const char *t, int *endpos_t) {
    int v = 0; // current state
    int l = 0; // current length
    int best = 0;
    int bestpos = 0;
    int n = strlen(t);
    for (int i = 0; i < n; ++i) {
        int c = (int)(unsigned char)t[i];
        if (st[v].next[c] != -1) {
            v = st[v].next[c];
            l++;
        } else {
            while (v != -1 && st[v].next[c] == -1) v = st[v].link;
            if (v == -1) {
                v = 0;
                l = 0;
            } else {
                l = st[v].len + 1;
                v = st[v].next[c];
            }
        }
        if (l > best) {
            best = l;
            bestpos = i;
        }
    }
    if (endpos_t) *endpos_t = bestpos;
    return best;
}

// free memory
void sa_free() {
    if (st) free(st);
    st = NULL;
}

void run_sample() {
    // SAMPLE 1: distinct substrings + occurrences
    const char *s = "ababa";
    int n = strlen(s);
    sa_init(n);
    for (int i = 0; i < n; ++i) sa_extend((int)(unsigned char)s[i]);
    sa_propagate_occ();
    printf("String: \"%s\"\n", s);
    printf("States: %d\n", sz);
    printf("Distinct substrings: %lld\n", sa_count_distinct_substrings());
    printf("Occurrences summary (state index, len, link, occ):\n");
    for (int i = 0; i < sz; ++i) {
        printf(" %2d: len=%2d link=%2d occ=%lld\n", i, st[i].len, st[i].link, st[i].occ);
    }

    // SAMPLE 2: Longest Common Substring between two strings
    const char *a = "abcabxabcd";
    const char *b = "zabxabcdyy";
    sa_free();
    sa_init(strlen(a));
    for (int i = 0; a[i]; ++i) sa_extend((int)(unsigned char)a[i]);
    int endpos;
    int lcslen = sa_longest_common_substring(b, &endpos);
    printf("\nLCS between\n  s1=\"%s\"\n  s2=\"%s\"\n", a, b);
    printf("LCS length = %d\n", lcslen);
    if (lcslen > 0) {
        printf("LCS substring in s2 (end pos %d): ", endpos);
        for (int k = endpos - lcslen + 1; k <= endpos; ++k) putchar(b[k]);
        putchar('\n');
    }

    sa_free();
}

int main() {
    run_sample();
    return 0;
}
