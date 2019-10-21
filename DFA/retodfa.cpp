#include <bits/stdc++.h>
using namespace std;
#define MAX_NODES 117
#define MAX_SYMBOLS 128
string postrfix;
bool isOperand(char ch) {
	return (ch >= '0' && ch <= '9') ||  (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '#' || ch == '$');
}
bool isOperator(char ch) {
	return (ch == '.' || ch == '|' || ch == '*');
}

int isRightAssociative(char op) {
	if(op == '$') return true;
	return false;
}

int GetOperatorWeight(char op) {
	int weight = -1;
	switch(op) {
	case '|':
		weight = 1;
	case '.':
		weight = 2;
	case '*':
		weight = 3;
	}
	return weight;
}

int HasHigherPrecedence(char op1, char op2) {
	int op1Weight = GetOperatorWeight(op1);
	int op2Weight = GetOperatorWeight(op2);

	if(op1Weight == op2Weight) {
		if(isRightAssociative(op1)) return false;
		else return true;
	}
	return op1Weight > op2Weight ?  true: false;
}
string makePostfix(string expression) {
	stack<char> S;
	string postfix = "";
	for(int i = 0;i< expression.length();i++) {

		if(expression[i] == ' ' || expression[i] == ',') continue;
        else if(isOperator(expression[i])) {
			while(!S.empty() && S.top() != '(' && HasHigherPrecedence(S.top(),expression[i])) {
				postfix+= S.top();
				S.pop();
			}
			S.push(expression[i]);
		}
		else if(isOperand(expression[i]))
		{
			postfix +=expression[i];
		}

		else if (expression[i] == '(')
		{
			S.push(expression[i]);
		}

		else if(expression[i] == ')') {
			while(!S.empty() && S.top() !=  '(') {
				postfix += S.top();
				S.pop();
			}
			S.pop();
		}
	}

	while(!S.empty()) {
		postfix += S.top();
		S.pop();
	}

	return postfix;
}
int numberOfNodes = 0;
vector <int> mV (int i) {
    vector <int> ret;
    ret.push_back(i);
    return ret;
}
vector <int> mV () {
    vector <int> ret;
    return ret;
}
typedef struct no {
    int nodeId;
    char ch;
    vector <int> firstPos;
    vector <int> lastPos;
    vector <int> followPos;
    int index;
    bool isNullable;
} node;
node makeNode (int _nid, char _ch, vector <int> _first, vector <int> _last, vector <int> _follow, bool _iN) {
    node ret;
    ret.nodeId = _nid; ret.ch = _ch;
    ret.firstPos = _first; ret.lastPos = _last; ret.followPos = _follow; ret.isNullable = _iN;
    return ret;
}
vector <int> mergeVector (vector <int> a, vector <int> b) {
    vector <int> ret;
    map <int, bool> M;
    for (int i=0; i<a.size(); i++) {
        if (M.find(a[i]) == M.end()) {
            ret.push_back(a[i]);
            M[a[i]] = 1;
        }
    }
    for (int i=0; i<b.size(); i++) {
        if (M.find(b[i]) == M.end()) {
            ret.push_back(b[i]);
            M[b[i]] = 1;
        }
    }
    sort(ret.begin(), ret.end());
    return ret;
}
void printNode (node a) {
    printf("%c\t\t", a.ch);
    for (int i=0; i<a.firstPos.size(); i++) printf("%d ", a.firstPos[i]);
    printf("\t");
    for (int i=0; i<a.lastPos.size(); i++) printf("%d ", a.lastPos[i]);
    printf("\t");
    for (int i=0; i<a.followPos.size(); i++) printf("%d ", a.followPos[i]);
    printf("\t");
    printf("%d\n", a.isNullable);
}
node makeNode (int _nid, char _ch, node a, node b) {
    node ret;
    ret.nodeId = _nid; ret.ch = _ch;
    if (_ch == '.') {
        printf("DOT DOT Here\n");
        printNode(a);
        printNode(b);
        if (a.isNullable) ret.firstPos = mergeVector(a.firstPos, b.firstPos);
        else ret.firstPos = a.firstPos;
        if (b.isNullable) ret.lastPos = mergeVector(a.lastPos, b.lastPos);
        else ret.lastPos = b.lastPos;
        ret.isNullable = a.isNullable & b.isNullable;
    }
    else if (_ch == '|') {
        ret.firstPos = mergeVector(a.firstPos, b.firstPos);
        ret.lastPos = mergeVector(a.lastPos, b.lastPos);
        ret.isNullable = a.isNullable | b.isNullable;
    }
    else if (_ch == '*') {
        ret.firstPos = a.firstPos;
        ret.lastPos = a.lastPos;
        ret.isNullable = 1;
    }
    sort(ret.firstPos.begin(), ret.firstPos.end());
    sort(ret.lastPos.begin(), ret.lastPos.end());
    printNode(ret);
    return ret;
}
vector <node> nodes;

vector <int> graph[MAX_NODES];
string chars;
string inputSymbols;
map <char, int> iMap;
int numSymbols = 0;
int makeTree (string str) {
    stack <node> S;
    int cur = 0;
    int current = 0;
    printf("%c %d\n", str[cur], cur);
    if (str[cur] == '#') S.push(makeNode(numberOfNodes, str[cur], mV(), mV(), mV(), str[cur++] == '#'));
    else {
        S.push(makeNode(numberOfNodes, str[cur], mV(current), mV(current), mV(), str[cur] == '#'));
        chars.push_back(str[cur]);
        numSymbols++;
        inputSymbols.push_back(str[cur]);
        iMap[str[cur]] = numSymbols;
    }
    current++;

    nodes.push_back(S.top());
    numberOfNodes++;
    cur++;
    cout << str << endl;
    while (!S.empty()) {
        if (cur >= str.size()) break;
        printf("%c %d %d %c\n", S.top().ch, cur, isOperator(str[cur]), str[cur]);
        if (str[cur] == '#') {
            S.push(makeNode(numberOfNodes, str[cur], mV(), mV(), mV(), str[cur] == '#'));
            current++;
            nodes.push_back(S.top());
            numberOfNodes++;
        }
        else if (isOperand(str[cur])) {
            S.push(makeNode(numberOfNodes, str[cur], mV(current), mV(current), mV(), str[cur] == '#'));
            chars.push_back(str[cur]);
            if (iMap.find(str[cur]) == iMap.end() && str[cur] != '$') {
                numSymbols++;
                inputSymbols.push_back(str[cur]);
                iMap[str[cur]] = numSymbols;
            }
            current++;
            nodes.push_back(S.top());
            numberOfNodes++;
        }
        else if (isOperator(str[cur])) {
            printf("From here\n");
            node a = S.top(); S.pop();
            node b;
            if (str[cur] != '*') {b = S.top(); S.pop();}
            else b = a;
            S.push(makeNode(numberOfNodes, str[cur], b, a));
            nodes.push_back(S.top());
            printf("HAHAHA %d %d\n", a.nodeId, b.nodeId);

            if (str[cur] != '*') graph[numberOfNodes].push_back(b.nodeId);
            graph[numberOfNodes].push_back(a.nodeId);
            printf("asfasf %d\n", graph[numberOfNodes][0]);
            numberOfNodes++;
        }
        cur++;
    }
    printf("Stack.\n");
    printf("Stack.\n");
    printf("Stack.\n");
    printNode(S.top());
    printf("Node id: %d\n", S.top().nodeId);
    return S.top().nodeId;
}
void findFollow () {
    for (int i=0; i<numberOfNodes; i++) {
        printf("%d %c\n", i, nodes[i].ch);
        if (nodes[i].ch == '.') {
            printf("DOT DOT DOT\n");
            printf("DOT DOT DOT\n");
            int x = graph[i][0];
            int y = graph[i][1];
            for (int j=0; j<nodes[x].lastPos.size(); j++) {
                    nodes[nodes[x].lastPos[j]].followPos = mergeVector(nodes[nodes[x].lastPos[j]].followPos,
                                                                       nodes[y].firstPos);
            }
        }
        else if (nodes[i].ch == '*') {
            int x = graph[i][0];
            int y = graph[i][0];
            printf("HERE %d %d\n", x, y);
            printf("here %d %d\n", nodes[x].lastPos.size(), nodes[y].firstPos.size());
            for (int j=0; j<nodes[x].lastPos.size(); j++) {
                    nodes[nodes[x].lastPos[j]].followPos = mergeVector(nodes[nodes[x].lastPos[j]].followPos,
                                                                       nodes[y].firstPos);
                }
        }
    }
}

typedef struct no1 {
    vector <int> idx;
} dfanode;
dfanode makeDFANode (vector <int> _idx) {
    dfanode ret;
    ret.idx = _idx;
    return ret;
}
vector < dfanode > dfanodes;
vector <int> edge[MAX_NODES][MAX_SYMBOLS];
int visited[MAX_NODES];
void printDFANode (dfanode temp) {
    printf("This node: ");
    for (int i=0; i<temp.idx.size(); i++) printf("%d ", temp.idx[i]);
    printf("\n");
}
bool isEqual (vector <int> a, vector <int> b) {
    if (a.size() != b.size()) return 0;
    for (int i=0; i<a.size(); i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}
bool isEqual (dfanode a, dfanode b) {
    return isEqual(a.idx, b.idx);
}
int findDFANode (dfanode temp) {
    for (int i=0; i<dfanodes.size(); i++) {
        if (isEqual(temp, dfanodes[i])) return i;
    }
    dfanodes.push_back(temp);
    return dfanodes.size() - 1;
}
void makeDFA (int root) {
    int numNodes = 0;
    dfanodes.push_back(makeDFANode(nodes[root].firstPos));

    for (int i=0; i<dfanodes.size(); i++) {
        for (int j=0; j<numSymbols; j++) {
            dfanode tmp;
            tmp.idx = mV();
            for (int k=0; k<dfanodes[i].idx.size(); k++) {
                if (chars[dfanodes[i].idx[k]] == inputSymbols[j]) {
                    tmp.idx = mergeVector(tmp.idx, nodes[dfanodes[i].idx[k]].followPos);
                }
            }
            printf("Transition %d %c ", i, inputSymbols[j]);
            printDFANode(tmp);
            int nodeId = findDFANode(tmp);
            edge[i][j].push_back(nodeId);
        }
    }
}
string withConcat (string str) {
    string ret = "";
    int flag = 0;
    for (int i=0; i<str.size(); i++) {
        if (isOperand(str[i])) {
            if (flag) {
                flag = 1;
                ret.push_back('.');
                ret.push_back(str[i]);
            }
            else {
                ret.push_back(str[i]);
                flag = 1;
            }
        }
        else if (str[i] == '*') {
            flag = 1;
            ret.push_back(str[i]);
        }
        else {
            flag = 0;
            ret.push_back(str[i]);
        }
    }
    return ret;
}
int main () {
    while (1) {
        string str;
        cin >> str;
        cout << "new: " << withConcat(str) << endl;
        cout << makePostfix(withConcat(str)) << "\n";
        int root = makeTree(makePostfix(withConcat(str)) + "$.");
        printf("Id\tCharacter\t");
        printf("First\t");
        printf("Last\t");
        printf("Follow\t");
        printf("IsNullable\n");
        for (int i=0; i<numberOfNodes; i++) {
            printf("%d\t", i);
            printNode(nodes[i]);
        }
        printf("WHAT\n");
        printf("WHAT\n");
        printf("WHAT\n");
        printf("Number of nodes: %d\n", numberOfNodes);
        findFollow();
        printf("Id\tCharacter\t");
        printf("First\t");
        printf("Last\t");
        printf("Follow\t");
        printf("IsNullable\n");
        for (int i=0; i<numberOfNodes; i++) {
            printf("%d\t", i);
            printNode(nodes[i]);
        }
        cout << "Input symbols: " << inputSymbols << endl;
        cout << "String" << chars << endl;
        makeDFA(root);
        printf("nodes: %d\b", dfanodes.size());
    }
    return 0;
}

//
//(a|b)*.a.b.b

