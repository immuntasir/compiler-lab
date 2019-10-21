#include <bits/stdc++.h>
using namespace std;
#define DEBUG 0
#define MAX_VARIABLES 17
#define MAX_NODES 100
#define MAX_CHAR 69
int num_of_prod;
int num_of_variables;
int num_of_char;
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


typedef struct prid {
    int i, j;
} production_id;
production_id makeProductionId (int _i, int _j) {
    production_id ret;
    ret.i = _i; ret.j = _j;
    return ret;
}
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

/*
    LR(1) begins here.
*/

vector <char> findFirst (string str, vector <char> lookahead) {
    vector <char> ret;
    int valid = 1;
    for (int i=0; valid && i<str.size(); i++) {
        valid = 0;
        if (isTerminal(str[i])) {
            ret.push_back(str[i]);
        }
        else {
            for (int j=0; j<first[charToInt[str[i]]].size(); j++) {
                if (first[charToInt[str[i]]][j] == '#') valid = 1;
                else ret.push_back(first[charToInt[str[i]]][j]);
            }
        }
    }
    if (valid) ret.insert(ret.end(), lookahead.begin(), lookahead.end());
    return ret;
}

typedef struct prod {
    char left;
    string right;
    int cur;
    vector <char> lookahead;
    production_id id;
    bool operator < (const prod &a) const {
        return cur < a.cur;
    }
} lr1_item;
lr1_item makeLR1_item (char _left, string _right, int _cur, vector <char> _lookahead, production_id _id) {
    lr1_item ret;
    ret.left = _left;  ret.cur = _cur; ret.lookahead = _lookahead; ret.id = _id;
    if (_right == "#") ret.right = "";
    else ret.right = _right;
    return ret;
}
typedef struct nod {
    vector <lr1_item> productions;
    bool operator <(const nod &a)const {
        if (productions.size() != a.productions.size()) return productions.size() < a.productions.size();
        for (int i=0; i<productions.size(); i++) {
            if (productions[i].left != a.productions[i].left) return productions[i].left < a.productions[i].left;
            if (productions[i].right != a.productions[i].right) return productions[i].right < a.productions[i].right;
            if (productions[i].cur != a.productions[i].cur) return productions[i].cur < a.productions[i].cur;
            if (productions[i].id.i != a.productions[i].id.i) return productions[i].id.i < a.productions[i].id.i;
            if (productions[i].id.j != a.productions[i].id.j) return productions[i].id.j < a.productions[i].id.j;
            if (productions[i].lookahead != a.productions[i].lookahead) return productions[i].lookahead < a.productions[i].lookahead;

        }
    }
} node;

int num_of_nodes = 0;
map <lr1_item, int> lr1_itemsToInt;
map <node, int> nodesToInt;
map <int, node> intToNodes;
int visited[MAX_NODES];
void makeAugmentedGrammer (string ch) {
    addProduction("P->" + ch);
}
void findTerminals () {
    num_of_char = num_of_variables;
    for (int i=0; i<num_of_variables; i++) {
        for (int j=0; j<productions[i].size(); j++) {
            for (int k=0; k<productions[i][j].size(); k++) {
                if (isTerminal(productions[i][j][k])) {
                    if (charToInt.find(productions[i][j][k]) == charToInt.end()) {
                        intToChar[num_of_char] = productions[i][j][k];
                        charToInt[productions[i][j][k]] = num_of_char++;
                    }
                }
            }
        }
    }
    intToChar[num_of_char] = '$';
    charToInt['$'] = num_of_char++;
}
vector <node> currentNodes;
bool isEqual (vector <char> a, vector <char> b) {
    if (a.size() != b.size()) return false;
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    for (int i=0; i<a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}
bool isEqual(vector <lr1_item> a, vector <lr1_item> b) {
    if (a.size() != b.size()) return false;
    for (int i=0; i<a.size(); i++) {
        if (a[i].cur != b[i].cur) return false;
        if (a[i].left != b[i].left) return false;
        if (a[i].right != b[i].right) return false;
        if (!isEqual(a[i].lookahead, b[i].lookahead)) return false;
    }
    return true;
}

int findNode (node temp) {
    /*
    if (nodesToInt.find(temp) == nodesToInt.end()) {
        nodesToInt[temp] = ++num_of_nodes;
        intToNodes[num_of_nodes] = temp;
    }
    */
    int index = -1;
    for (int i=0; i<currentNodes.size(); i++) {
        if (isEqual(currentNodes[i].productions, temp.productions)) {
            index = i;
            break;
        }
    }
    if (DEBUG) printf("From /findNode: Index = %d\n", index);
    if (index == -1) {
        currentNodes.push_back(temp);
        index = currentNodes.size() - 1;
        nodesToInt[temp] = num_of_nodes;
        intToNodes[num_of_nodes] = temp;
        num_of_nodes++;
    }
    return index;
}
vector <char> findFirst (lr1_item item) {
    vector <char> tmp;
    if (item.cur >= item.right.size()) return item.lookahead;
    return findFirst(item.right.substr(item.cur + 1), item.lookahead);
}
node makeCanonicalCover (lr1_item item) {
    node ret;
    ret.productions.push_back(item);
    if (item.cur >= item.right.size()) {

    }
    else if (isTerminal(item.right[item.cur])) {

    }
    else {
        int variable = charToInt[item.right[item.cur]];
        for (int i=0; i<productions[variable].size(); i++) {
            node temp = makeCanonicalCover(makeLR1_item(item.right[item.cur],
                                                        productions[variable][i],
                                                        0,
                                                        findFirst(item),
                                                        makeProductionId(variable, i)));
            for (int j=0; j<temp.productions.size(); j++) {
                ret.productions.push_back(temp.productions[j]);
            }
        }
    }
    return ret;
}
void printLR1item (lr1_item item) {
    cout << item.left << "->";
    for (int i=0; i<item.right.size(); i++) {
        if (i == item.cur) printf(".");
        printf("%c", item.right[i]);
    }
    if (item.cur >= item.right.size()) printf(".");
    printf(", ");
    for (int i=0; i<item.lookahead.size(); i++) {
        printf("%c", item.lookahead[i]);
        if (i<item.lookahead.size()-1) printf(",");
    }
    cout << " ... ... ... [" << " " << intToChar[item.id.i] << "->" << productions[item.id.i][item.id.j] << " ]";
    printf("\n");
}
void printNode (node tmp) {
    printf("#%d\n", findNode(tmp));
    for (int i=0; i<tmp.productions.size(); i++) {
        printLR1item(tmp.productions[i]);
    }
}
typedef struct tE {
    string type;
    production_id prod_id;
    int node_id;
} TableEntry;
TableEntry makeTableEntry (production_id _id, int _node_id, string _type) {
    TableEntry ret;
    ret.prod_id = _id; ret.node_id = _node_id; ret.type = _type;
    return ret;
}
int nodeForwarding[MAX_NODES];
void initializeNodeForwarding () {
    for (int i=0; i<num_of_nodes; i++)
        nodeForwarding[i] = i;
}
TableEntry graph[MAX_NODES][MAX_CHAR];
int isEntry[MAX_NODES][MAX_CHAR];

int isFinal[MAX_NODES];
void makeGraph(node currentNode) {
    visited[findNode(currentNode)] = true;
    for (int i=0; i<currentNode.productions.size(); i++) {
        lr1_item curr = currentNode.productions[i];
        if (DEBUG) printf("here: ");
        if (DEBUG) printLR1item(curr);
        if  (curr.cur >= curr.right.size()) {
            if (DEBUG) printf("SHESSSSSSSSSSSSSSSSSSSSSSSSS\n");
            int node_id = findNode(currentNode);
            if (DEBUG) printf("Node id: %d\n", node_id);
            isFinal[node_id] = 1;
            int idx = charToInt[curr.left];
            for (int i=0; i<curr.lookahead.size(); i++)  {
                graph[node_id][charToInt[curr.lookahead[i]]] = makeTableEntry(curr.id,
                                                                              7,
                                                                              "reduce");
                isEntry[node_id][charToInt[curr.lookahead[i]]] = 1;
            }
            continue;
        }
        lr1_item nextItem = makeLR1_item(curr.left, curr.right, curr.cur + 1, curr.lookahead, curr.id);
        if (DEBUG) printf("Next: ");
        if (DEBUG) printLR1item(nextItem);
        node temp = makeCanonicalCover(nextItem);
        if (DEBUG) printf("Size of the node %d\n", temp.productions.size());
        if (DEBUG) printf("The node: ");
        if (DEBUG) printf(" %d:", findNode(temp));
        if (DEBUG) printNode(temp);
        if (DEBUG) printf("visited: %d\n", visited[findNode(temp)]);
        if (visited[findNode(temp)] == 0) {
            if (DEBUG) printf("Here!\n");
            if (DEBUG) printf("Moving with %c\n", curr.right[curr.cur]);

            //findNode(temp);
            //visited[findNode(currentNode)][charToInt[curr.right[curr.cur]]] = 1;
            makeGraph(temp);
        }
        if (isTerminal(curr.right[curr.cur])) {
            graph[findNode(currentNode)][charToInt[curr.right[curr.cur]]] = makeTableEntry(curr.id,
                                                                          findNode(temp),
                                                                            "shift");
            isEntry[findNode(currentNode)][charToInt[curr.right[curr.cur]]] = 1;
        }
        else {
            if (DEBUG) printf("HERE TOOO\n");
            graph[findNode(currentNode)][charToInt[curr.right[curr.cur]]] = makeTableEntry(curr.id,
                                                                            findNode(temp),
                                                                            "goto");
            isEntry[findNode(currentNode)][charToInt[curr.right[curr.cur]]] = 1;
        }
    }
}
void makeGraph () {
    num_of_nodes = 0;
    vector <char> tmp;
    tmp.push_back('$');
    lr1_item temp = makeLR1_item('P', productions[charToInt['P']][0], 0,  tmp, makeProductionId(0, 0));
    node tempNode = makeCanonicalCover(temp);
    findNode(tempNode);
    makeGraph(tempNode);
}

int findForwardedNode (int cur) {
    while (nodeForwarding[cur] != cur) {
        cur = nodeForwarding[cur];
    }
    return cur;
}
int findForwardedNode (node a) {
    int cur = findNode(a);
    while (nodeForwarding[cur] != cur) {
        cur = nodeForwarding[cur];
    }
    return cur;
}
void printNode (int i) {
    i = findForwardedNode(i);
    node tmp = currentNodes[i];
    printf("#%d\n", i);
    for (int i=0; i<tmp.productions.size(); i++) {
        printLR1item(tmp.productions[i]);
    }
}

void findFinalNodes () {

}
void makeTable(int current_node) {

}
void printTableEntry (TableEntry entry) {
    if (entry.node_id == -1) return;
    if (entry.type == "goto") {
        printf("%d", findForwardedNode(entry.node_id));
    }
    else if (entry.type == "shift") {
        printf("S%d", findForwardedNode(entry.node_id));
    }
    else if (entry.type == "reduce") {
        cout << intToChar[entry.prod_id.i] << "->" << productions[entry.prod_id.i][entry.prod_id.j];
    }
}
void printTable () {
    printf("\t");
    for (int i=0; i<num_of_char; i++) {
        printf("%c", intToChar[i]);
        printf("\t");
    }
    printf("\n");
    for (int i=0; i<num_of_nodes; i++) {
        if (findForwardedNode(i) != i) continue;
        printf("%d:\t", i);
        for (int j=0; j<num_of_char; j++) {
            printTableEntry(graph[i][j]);
            printf("\t");
        }
        printf("\n");
    }
}
bool isMergeable (node a, node b) {
    if (a.productions.size() != b.productions.size()) return false;
    for (int i=0; i<a.productions.size(); i++) {
        if (a.productions[i].cur != b.productions[i].cur) return false;
        if (a.productions[i].left != b.productions[i].left) return false;
        if (a.productions[i].right != b.productions[i].right) return false;
    }
    return true;
}
vector <char> mergeLookahead (vector <char> a, vector <char> b) {
    vector <char> ret;
    map <char, bool> flag;
    for (int i=0; i<a.size(); i++) {
        if (flag.find(a[i]) == flag.end()) {
            if (DEBUG) printf("From /mergeLookahead %c\n", a[i]);
            flag[a[i]] = true;
            ret.push_back(a[i]);
        }
    }
    for (int i=0; i<b.size(); i++) {
        if (flag.find(b[i]) == flag.end()) {
            if (DEBUG) printf("From /mergeLookahead %c\n", a[i]);
            flag[b[i]] = true;
            ret.push_back(b[i]);
        }
    }
    if (DEBUG) for (int i=0; i<ret.size(); i++) printf("Lookahead %c\n", ret[i]);
    return ret;
}
int findNodeNumber (node a) {

}
TableEntry mergeTableEntry (TableEntry a, TableEntry b) {
    TableEntry ret;
    if (DEBUG) printf("From /mergeTableEntry\n");
    if (DEBUG) printTableEntry(a);
    if (DEBUG) printTableEntry(b);
    if (a.node_id == -1) return b;
    return a;
}
void mergeNodes (node a, node b) {

    if (DEBUG) printNode(a);
    if (DEBUG) printNode(b);
    int ii = findNode(a);
    int jj = findNode(b);
    for (int j=0; j<num_of_char; j++) {
        isEntry[ii][j] = isEntry[ii][j] | isEntry[jj][j];
        if (graph[jj][j].type == "reduce") graph[ii][j] = graph[jj][j];
    }
    if (DEBUG) printf("Nodes: %d %d\n", ii, jj);
    if (DEBUG) printf("Table entry: %d %d\n", nodeForwarding[ii], nodeForwarding[jj]);

    for (int i=0; i<a.productions.size(); i++) {
        a.productions[i].lookahead = mergeLookahead(a.productions[i].lookahead, b.productions[i].lookahead);
        if (DEBUG) for (int j=0; j<a.productions[i].lookahead.size(); j++) printf("Lookahead update %c\n", a.productions[i].lookahead[j]);
    }
    //if (DEBUG) printNode(a);
    nodeForwarding[jj] = ii;
    currentNodes[ii] = a;
    //if (DEBUG) printNode(currentNodes[ii]);
    if (DEBUG) printf("After merging: %d %d\n", findForwardedNode(ii), findForwardedNode(jj));
    if (DEBUG) printNode(ii);
}
void mergeNodes () {
    for (int i=0; i<num_of_nodes-1; i++) {
        for (int j=i+1; j<num_of_nodes; j++) {
            if (isMergeable(intToNodes[i], intToNodes[j])) {
                if (DEBUG) printf("Merging %d with %d\n", i, j);
                mergeNodes(intToNodes[i], intToNodes[j]);
            }
        }
    }
}
void initialize_table () {
    for (int i=0; i<num_of_nodes; i++) {
        for (int j=0; j<num_of_char; j++) {
            graph[i][j].node_id = -1;
        }
    }
}
typedef struct sE {
    int node;
    char ch;
    string type;
} StackEntry;
StackEntry makeStackEntry (int _node, char _ch, string _type) {
    StackEntry ret;
    ret.node = _node; ret.ch = _ch; ret.type = _type;
    return ret;
}
stack <StackEntry> S;

vector <string> toPrint;
string makeString(char ch, string str) {
    string ret;
    ret.push_back(ch);
    ret = ret + "->" + str;
    return ret;
}
void printStack() {
    printf("The stack: ");
    stack <StackEntry> Scopy = S;
    while (!Scopy.empty()) {
        if (Scopy.top().type == "node") {
            printf("%d ", Scopy.top().node);
        }
        else printf("%c ", Scopy.top().ch);
        Scopy.pop();
    }
    printf("\n");
}
void parse (string str) {
    str.push_back('$');
    while (!S.empty()) S.pop();
    S.push(makeStackEntry(0, '-', "node"));
    toPrint.clear();
    int ptr = 0;
    StackEntry topEntry;
    TableEntry tableEntry;
    int currentNode = 0, currentChar = 0;
    int valid = 0;
    while (!S.empty() && !valid) {
        if (DEBUG) printf("parsing\n");
        topEntry = S.top();
        if (DEBUG) cout << topEntry.type << " " << topEntry.node << " " << str << " " << ptr  << endl;
        if (topEntry.type == "node") {
            currentNode = topEntry.node;
            currentChar = charToInt[str[ptr]];
            tableEntry = graph[currentNode][currentChar];
            if (DEBUG) cout << topEntry.node << " " << str[ptr] << " : " <<  tableEntry.type << endl;
            if (DEBUG) cout << "Node id: " << isEntry[currentNode][currentChar] << endl;
            if (!isEntry[currentNode][currentChar]) {
                printf("The string is not valid.\n");
                return;
            }
            else if (tableEntry.prod_id.i == 0 && tableEntry.prod_id.j == 0) {
                valid = 1;
                break;
            }
            else if (tableEntry.type == "shift") {
                S.push(makeStackEntry(currentNode, intToChar[currentChar], "terminal"));
                S.push(makeStackEntry(tableEntry.node_id, intToChar[currentChar], "node"));
                ptr++;
            }
            else if (tableEntry.type == "reduce") {
                string prod = productions[tableEntry.prod_id.i][tableEntry.prod_id.j];
                if (DEBUG) printf("Here\n");
                toPrint.push_back(makeString(intToChar[tableEntry.prod_id.i],
                                             productions[tableEntry.prod_id.i][tableEntry.prod_id.j]));
                if (DEBUG) printStack();
                if (DEBUG) printf("will remove: %d\n", prod.size() * 2);
                if (prod == "#") prod = "";
                for (int i=0; i<prod.size(); i++) {
                    S.pop();
                    S.pop();
                }
                S.push(makeStackEntry(S.top().node, intToChar[tableEntry.prod_id.i], "variable"));
                if (DEBUG) printStack();
            }
        }
        else if (topEntry.type == "terminal") {
            if (DEBUG) printf("BUG\nBUG\tBUG\nBUG\tBUG\tBUG\n");
        }
        else if (topEntry.type == "variable") {
            if (DEBUG) printf("Will push\n");
            currentNode = topEntry.node;
            currentChar = charToInt[topEntry.ch];
            tableEntry = graph[currentNode][currentChar];
            if (DEBUG) cout << topEntry.ch << " " << currentChar << " "  <<  currentNode << " " << tableEntry.node_id << endl;
            if (isEntry[currentNode][currentChar]) S.push(makeStackEntry(tableEntry.node_id, intToChar[currentChar], "node"));
            else {
                printf("The string is invalid.\n");
                return;
            }
        }
    }
    printf("Parsed successfully!\n");
    for (int i=0; i<toPrint.size(); i++) cout << "#" << i+1 << ": " << toPrint[i] << endl;
}
int main () {
    //freopen("in.txt", "r", stdin);
    while (1) {
        printf("Enter -1 to quit.\n");
        printf("Number of productions:");
        scanf("%d", &num_of_prod);
        //if (num_of_prod == -1) continue;
        string str;
        printf("Starting variable:");
        string ch;
        cin >> ch;
        makeAugmentedGrammer(ch);
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
        findTerminals();
        initialize_table();
        makeGraph();
        findFinalNodes();
        makeTable(0);

        printf("The grammer:\n");
        for (int i=0; i<num_of_variables; i++) {
            for (int j=0; j<productions[i].size(); j++) {
                cout << intToChar[i] << "->" << productions[i][j] << "\n";
            }
        }

        for (int i=0; i<num_of_variables; i++) {
            printf("First(%c): ", intToChar[i]);
            for (int j=0; j<first[i].size(); j++) {
                cout << first[i][j];
                if (j == first[i].size() - 1) printf("\n");
                else printf(", ");
            }
        }
        initializeNodeForwarding();

        printf("Total number of nodes: %d\n", num_of_nodes);
        for (int i=0; i<num_of_nodes; i++) {
            printNode(i);
        }
        printf("The LR(1) Table:\n");
        printTable();
        mergeNodes();

        printf("The LALR(1) Table:\n");
        printTable();
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
