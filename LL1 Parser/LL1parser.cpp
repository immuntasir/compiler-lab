#include <bits/stdc++.h>
using namespace std;
#define MAX_VARIABLES 17
int num_of_prod;
int num_of_variables;
map <char, int> charToInt;
map <int, char> intToChar;
vector < string > productions[MAX_VARIABLES];
vector <char> first[MAX_VARIABLES];
int flag_first[MAX_VARIABLES];
vector <char> follow[MAX_VARIABLES];
int flag_follow[MAX_VARIABLES];
map <char, int> map_first[MAX_VARIABLES];
map <char, int> map_follow[MAX_VARIABLES];
string terminals = "abcdefghijklmnopqrstuvwxyz+*()";
map <char, int> tableCount[MAX_VARIABLES];
map <char, string> table[MAX_VARIABLES];
typedef struct ta {
    char terminal;
    string production;
} TableEntry;
vector <TableEntry> tableEntries[MAX_VARIABLES];
void clearAll() {
    charToInt.clear();
    intToChar.clear();
    num_of_variables = 0;
    for (int i=0; i<MAX_VARIABLES; i++) {
        productions[i].clear();
        first[i].clear();
        follow[i].clear();
        map_first[i].clear();
        map_follow[i].clear();
        tableCount[i].clear();
        tableEntries[i].clear();
        table[i].clear();
    }
    memset(flag_first, 0, sizeof flag_first);
    memset(flag_follow, 0, sizeof flag_follow);
}

int addProduction(string str) {
    if (str.size() < 4) return -1;
    if (!(str[1] == '-' && str[2] == '>'))  return -1;
    if (!(str[0] >= 'A' && str[0] <= 'Z')) return -1;
    string tmp;
    for (int i=3; i<str.size(); i++) tmp.push_back(str[i]);
    if (charToInt.find(str[0]) == charToInt.end()) {
        intToChar[num_of_variables] = str[0];
        charToInt[str[0]] = num_of_variables++;
    }
    productions[charToInt[str[0]]].push_back(tmp);
    return 1;
}
int isTerminal (char ch) {
    for (int i=0; i<terminals.size(); i++) if (terminals[i] == ch) return 1;
    return 0;
}
int findFirst (int idx) {
    //printf("%d\n", idx);
    flag_first[idx] = -1;
    for (int i=0; i<productions[idx].size(); i++) {
        if (isTerminal(productions[idx][i][0]) || productions[idx][i][0] == '#') {
            if (map_first[idx].find(productions[idx][i][0]) == map_first[idx].end()) {
                first[idx].push_back(productions[idx][i][0]);
                map_first[idx][productions[idx][i][0]] = 1;
            }
        }
        else {
            int cur = 0;

            for (int cur = 0; cur < productions[idx][i].size(); cur++) {
                //printf("%d %d %d %d\n", idx, i,  cur);
                int flag = 0;
                if (flag_first[productions[idx][i][cur]] == -1) {
                    //printf("There's some problem in the grammar. {recursive follow}\n");
                    //return -1;
                    //continue;
                }
                else if (flag_first[productions[idx][i][cur]] == 0) {
                    findFirst(charToInt[productions[idx][i][cur]]);
                }
                for (int j=0; j<first[charToInt[productions[idx][i][cur]]].size(); j++) {
                    if (first[charToInt[productions[idx][i][cur]]][j] == '#') flag = 1;
                    if (map_first[idx].find(first[charToInt[productions[idx][i][cur]]][j]) == map_first[idx].end()) {
                        first[idx].push_back(first[charToInt[productions[idx][i][cur]]][j]);
                        map_first[idx][first[charToInt[productions[idx][i][cur]]][j]] = 1;
                    }
                }
                if (!flag) break;
            }
        }
    }
    //printf("done\n");
    flag_first[idx] = 1;
    return 1;
}
int findFirst () {
    int flag = 1;
    int cnt = 5;
    while (flag) {
        flag = 0;
        for (int i=0; i<num_of_variables; i++) {
            if (!flag_first[i]) {
                int ret = findFirst(i);
                if (ret == -1) flag = 1;
            }
        }
        cnt--;
        if (cnt == 0) {
            printf("Error while making the first table.\n");
            return -1;
        }
    }
    return 1;
}
int findFollow (int idx) {
    flag_follow[idx] = -1;
    for (int i=0; i<num_of_variables; i++) {
        for (int j=0; j<productions[i].size(); j++) {
            for (int k=0; k<productions[i][j].size(); k++) {
                if (productions[i][j][k] == intToChar[idx]) {
                    if (k == productions[i][j].size() - 1) {
                        if (flag_follow[i] == -1) {
                            //printf("Error while making the follow table. {Recursive follow}\n");
                            //return -1;
                            //continue;
                        }
                        else if (flag_follow[i] == 0) {
                            findFollow(i);
                        }
                        for (int l=0; l<follow[i].size(); l++) {
                            if (map_follow[idx].find(follow[i][l]) == map_follow[idx].end()) {
                                map_follow[idx][follow[i][l]] = 1;
                                follow[idx].push_back(follow[i][l]);
                            }
                        }
                    }
                    else {
                        int flag = 0;
                        for (int cur = k+1; cur < productions[i][j].size(); cur++) {
                            flag = 0;
                            if (isTerminal(productions[i][j][cur]) || productions[i][j][cur] == '#') {
                                if (map_follow[idx].find(productions[i][j][cur]) == map_follow[idx].end()) {
                                    map_follow[idx][productions[i][j][cur]] = 1;
                                    follow[idx].push_back(productions[i][j][cur]);
                                }
                            }
                            else {
                                int index = charToInt[productions[i][j][cur]];
                                for (int l=0; l<first[index].size(); l++) {
                                    if (first[index][l] == '#') flag = 1;
                                    else if (map_follow[idx].find(first[index][l]) == map_follow[idx].end()) {
                                        map_follow[idx][first[index][l]] = 1;
                                        follow[idx].push_back(first[index][l]);
                                    }
                                }
                            }
                            if (!flag) break;
                        }
                        if (flag) {
                            for (int l=0; l<follow[i].size(); l++) {
                                if (map_follow[idx].find(follow[i][l]) == map_follow[idx].end()) {
                                    map_follow[idx][follow[i][l]] = 1;
                                    follow[idx].push_back(follow[i][l]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    flag_follow[idx] = 1;
    return 1;
}
int findFollow () {
    int flag  = 1;
    follow[0].push_back('$');
    int cnt = 5;
    while (flag) {
        flag = 0;
        for (int i=1; i<num_of_variables; i++) {
            if (!flag_follow[i]) {
                int ret = findFollow(i);
                if (ret == -1) flag = 1;
            }
        }
        cnt--;
        if (cnt == 0) {
            printf("Error while making the follow table.\n");
            return 0;
        }
    }
    return 1;
}
TableEntry makeEntry (int i, char ch, string str) {
    TableEntry entry;
    if (ch == '#') entry.terminal = '$';
    else entry.terminal = ch;
    entry.production.push_back(intToChar[i]);
    entry.production +=  "->";
    entry.production += str;
    return entry;
}
int makeTable () {
    for (int i=0; i<num_of_variables; i++) {
        for (int j=0; j<productions[i].size(); j++) {
            int flag = 0;
            for (int cur = 0; cur<productions[i][j].size(); cur++) {
                flag = 0;
                if (isTerminal(productions[i][j][cur])) {
                    //printf("Dhukche %d %d",i, j);
                    tableEntries[i].push_back(makeEntry(i, productions[i][j][cur], productions[i][j]));
                    table[i][productions[i][j][cur]] = makeEntry(i, productions[i][j][cur], productions[i][j]).production;
                    tableCount[i][productions[i][j][cur]]++;
                    if (tableCount[i][productions[i][j][cur]] > 1) {
                        printf("The grammar is ambiguous.\n");
                        return -1;
                    }
                }
                else if (productions[i][j][cur] == '#') {
                    printf("damndaman here\n");
                    flag = 1;
                    //tableEntries[i].push_back(makeEntry(i, '$', productions[i][j]));
                    //tableCount[i]['#']++;
                    //if (tableCount[i]['#'] > 1) {
                      //  printf("The grammar is ambiguous.\n");
                       // return -1;
                    //}
                    break;
                }
                else {
                    //printf("ekhane %d %d", i, j);
                    int current = charToInt[productions[i][j][cur]];
                    for (int k = 0; k<first[current].size(); k++) {
                        //cout << productions[i][j][cur] << " damndamndamn " << first[current][k] << endl;
                        if (first[current][k] == '#') flag = 1;
                        else {
                            //cout << intToChar[i] << " - " << productions[i][j][cur] << ": " << first[current][k] << endl;
                            tableEntries[i].push_back(makeEntry(i, first[current][k], productions[i][j]));
                            table[i][first[current][k]] = makeEntry(i, first[current][k], productions[i][j]).production;
                            tableCount[i][first[current][k]]++;
                            if (tableCount[i][first[current][k]] > 1) {
                                printf("The grammar is ambiguous.\n");
                                return -1;
                            }
                        }
                    }
                }
                if (!flag) break;
            }
            if (flag) {
                for (int l=0; l<follow[i].size(); l++) {
                    tableEntries[i].push_back(makeEntry(i, follow[i][l], productions[i][j]));
                    table[i][follow[i][l]] = makeEntry(i, follow[i][l], productions[i][j]).production;
                    tableCount[i][follow[i][l]]++;
                    if (tableCount[i][follow[i][l]] > 1) {
                        printf("The grammar is ambiguous.\n");
                        return -1;
                    }
                }
            }
        }
    }
    return 1;
}
int parse (string str) {
    int ptr = 0;
    str.push_back('$');
    stack < char > S;
    S.push('$');
    S.push(intToChar[0]);
    while (!S.empty()) {
       // cout << str << " " << ptr << " " << S.top() << endl;
        char cur = S.top();
        if (str[ptr] == cur) {
            S.pop();
            ptr++;
        }
        else if (cur >= 'A' && cur <= 'Z') {
            if (tableCount[charToInt[cur]].find(str[ptr]) == tableCount[charToInt[cur]].end()) {
                printf("Syntax error.\n");
                return -1;
            }
            else {
                string currentProd = table[charToInt[cur]][str[ptr]];
                cout << currentProd << endl;
                S.pop();
                for (int i=currentProd.size()-1; currentProd[i] != '>'; i--) {
                    if (currentProd[i] != '#') S.push(currentProd[i]);
                }
            }
         }
         else {
            printf("Error\n");
            return -1;
         }
    }
    return 1;
}
int main () {
    //freopen("test.txt", "r", stdin);
    while (1) {
        printf("Enter -1 to quit.\n");
        printf("Number of productions:");
        scanf("%d", &num_of_prod);
        string str;
        for (int i=0; i<num_of_prod; i++) {
            cin >> str;
            if (addProduction(str) < 0) {
                printf("There's some error in this production, try again.\n");
                i--;
            }
        }
        for (int i=0; i<num_of_variables; i++) printf("Variable #%d: %c\n", i, intToChar[i]);
        //printf("here");
        findFirst();
        findFollow();
        makeTable();
        for (int i=0; i<num_of_variables; i++) {
            printf("First(%c): ", intToChar[i]);
            for (int j=0; j<first[i].size(); j++) {
                cout << first[i][j];
                if (j == first[i].size() - 1) printf("\n");
                else printf(", ");
            }
        }
        for (int i=0; i<num_of_variables; i++) {
            printf("Follow(%c): ", intToChar[i]);
            for (int j=0; j<follow[i].size(); j++) {
                cout << follow[i][j];
                if (j == follow[i].size() - 1) printf("\n");
                else printf(", ");
            }
        }
        int cnt  = 0;
        for (int i=0; i<num_of_variables; i++) {
            for (int j=0; j<tableEntries[i].size(); j++) {
                cout << ++cnt << ". " << intToChar[i] << " " <<
                    tableEntries[i][j].terminal << " " <<
                        tableEntries[i][j].production << "\n";
            }
        }

        while (1) {
            printf("Enter quit to continue.\n");
            printf("Or enter your string.\n");
            cin >> str;
            if (str == "quit") break;
            else parse(str);
        }
    }
    return 0;
}
