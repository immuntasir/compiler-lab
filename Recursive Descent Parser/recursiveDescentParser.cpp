#include <bits/stdc++.h>
#define MAXSTRING 1000
using namespace std;
struct node {
    vector < int> prod;
    int id;
    node () {}
    node (vector <int> _prod, int _id) {
        prod = _prod;
        id = _id;
    }
};
map <string, int> stringMap;
map <int, string> reverseMap;
vector < node > grammer[MAXSTRING];
int cnt = 0;
int ruleCnt = 0;
void clearGrammer () {
    // This function clears the grammer
    stringMap.clear();
    reverseMap.clear();
    cnt = 0;
    ruleCnt = 0;
    for (int i=0; i<MAXSTRING; i++) grammer[i].clear();
}
int convertToInt (string str) {
    if (stringMap.find(str) != stringMap.end()) return stringMap[str];
    else {
        cnt++;
        stringMap[str] = cnt;
        reverseMap[cnt] = str;
    }
    return stringMap[str];
}
vector <int> strSplit (string str) {
    string tmp = "";
    vector <int> ret;
    ret.clear();

    for (int i=0; i<str.size(); i++) {
        if (str[i] == ' ') {
            if (tmp != "") {
                ret.push_back(convertToInt(tmp));
                tmp = "";
            }
        }
        else tmp.push_back(str[i]);
    }
    if (tmp != "") ret.push_back(convertToInt(tmp));
    return ret;
}
vector <string> divideRules (string str) {
    vector <string> ret;
    string tmp = "";
    for (int i=0; i<str.size(); i++) {
        if (str[i] == '|') {
            if (tmp != "") {
                ret.push_back(tmp);
                tmp = "";
            }
        }
        else tmp.push_back(str[i]);
    }
    if (tmp != "") {
        ret.push_back(tmp);
        tmp = "";
    }
    return ret;
}
int addRule (string from, string to) {
    // This function adds a rule
    vector <string> rules;
    rules = divideRules(to);
    vector <int> prod;
    for (int i=0; i<rules.size(); i++) {
        prod = strSplit(rules[i]);
        grammer[convertToInt(from)].push_back(node(prod, ++ruleCnt));
    }
}
void printRules () {
    for (int i=1; i<cnt; i++) {
        for (int j=0; j<grammer[i].size(); j++) {
            printf("Rule %d: ", grammer[i][j].id);
            cout << reverseMap[i] << " -> " ;
            for (int k=0; k<grammer[i][j].prod.size(); k++) {
                cout << reverseMap[grammer[i][j].prod[k]] << " ";
            }
            printf("\n");
        }
    }
}
string toString (int num) {
    string ret = "";
    ret.push_back('0' + num);
    return ret;
}
int main () {
    addRule("goal", "expr");
    addRule("expr", "term expr2");
    addRule("expr2", "+ term expr2 | - term expr2 | eps");
    addRule("term", "factor term2");
    addRule("term2", "* factor term2 | / factor term2 | eps");
    addRule("factor", "number | identifier | ( expr )");
    for (int i=0; i<10; i++) addRule("number", toString(i));
    addRule("factor", "+");
    addRule("factor", "-");
    addRule("factor", "*");
    addRule("factor", "/");
    addRule("factor", "|");
    printRules();
    return 0;
}


