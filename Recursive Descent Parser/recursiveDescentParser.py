'''
Name: Muntasir Wahed
Roll: AE-075-007
'''


cursor = 0
savedCursor = 0
str = ''
def saveCursor():
    global cursor, savedCursor
    savedCursor = cursor
    return 1
def backtrack():
    global cursor, savedCursor
    cursor = savedCursor
    return 1
def getNextToken():
    global cursor
    if (cursor >= len(str)):
        return '$' 
    while (str[cursor] == ' '):
        cursor = cursor + 1
    nextToken = str[cursor]
    cursor = cursor + 1
    if (nextToken >= '0' and nextToken <= '9'):
        return 'num'
    elif (nextToken >= 'a' and nextToken <= 'z'):
        return 'id'
    return nextToken;
def isToken(expected):
    return int(getNextToken() == expected)


# In[93]:


# goal → expr
def goal():
    return expr()


# In[94]:


# expr → term expr2
def expr():
    return term() and expr2()


# In[95]:


# expr2 → + term expr2 | - term expr2 | ε
def expr2():
    return (saveCursor() and (isToken('+') or isToken('-')) and term() and expr2())  or (backtrack() and 1) 


# In[96]:


# term → factor term2
def term():
    return factor() and term2()


# In[97]:


# term2 → * factor term2 | / factor term2 | ε
def term2():
    return (saveCursor() and (isToken('*') or isToken('/')) and factor and term2())  or (backtrack() and 1) 


# In[98]:


# factor → number | identifier | ( expr )
def factor () :
    return (saveCursor() and (isToken('num') or isToken('id'))) or (backtrack() and isToken('(') and expr() and isToken(')'))


# In[99]:


def parse(_str):
    global str
    str = _str
    global cursor, savedCursor
    cursor = 0
    savedCursor = 0
    return (goal() and cursor == len(str))


while (True):
    str = input()
    if (str == "exit"):
        break
    else:
        print(parse(str))
