#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TreeNode;
class Node {
public:
  long long int data;
  Node *nextField;
  TreeNode *self;
};

class TreeNode {
public:
  Node **keys;
  TreeNode **child;
  bool leaf;
  int numberOfKeys;
  int minimumDegree;

  TreeNode(int, bool);
  void addToNotFullTreeNode(Node *);
  void searchInSubTree(vector<Node *> *, long long int, string);
  void splitingChildOfTreeNode(TreeNode *, int);
  void takeKeyFromPrevious(int);
  void removeKeyFromSubTree(Node *);
  int searchForKey(Node *);
  void removeKeyFromLeaf(int);
  void removeKeyFromNotLeaf(int);
  void takeKeyFromNext(int);
  void fillChild(int);
  Node *predecessorOfKey(int);
  Node *successorOfKey(int);
  void mergeWithBrother(int);

  friend class Tree;
};

class Tree {
public:
  TreeNode *root;
  int minimumDegree;

  Tree(int);
  void searchInTree(vector<Node *> *, long long int, string);
  void insertNewKey(Node *);
  void removeKeyFromTree(Node *);
};

class LinkedList {
public:
  Node *head;
  LinkedList();

  void insertToLinkedList(Node *);
  vector<Node *> getLinkedList();
};

class DataTable {
public:
  string tableName;
  int numberOfColumns;
  vector<Tree> columns;
  vector<string> nameOfColumns;
  vector<string> typeOfColumns;
  static const string INTEGERTYPE;
  static const string STRINGTYPE;
  static const string TIMESTAMPTYPE;
  const string idColumnName = "id";
  const string idColumnType = INTEGERTYPE;
  vector<LinkedList> records;

  DataTable(vector<string>, vector<string>, string);
  void initialColumns();
  int getColumnIndex(string);
  void insertRecord(vector<string>);
  int findRecordIndex(Node *);
  void updateRecord(string, string, string, vector<string>);
  void removeRecord(string, string, string);
  void selectFields(vector<string>, string, string, string);
};

namespace Hash {
long long int code(string);
string decode(long long int);
} // namespace Hash

int getDataTableIndex(vector<DataTable>, string);
long long int timestampToInt(string);
string intToTimestamp(long long int);
long long int codeValue(string, string);
string decodeValue(long long int, string);

int main() {
  vector<DataTable> tables;
  const char po = '(', pc = ')', nl = '\n', com = ',', sp = ' ', cot = '\"',  eq = '=', gr = '>', sm = '<', st = '*';
  
  int n;
  cin >> n;
  for (int line = 0; line < n; line++) {
    string querytType, nameOfTable, skip;
    cin >> querytType;
    if (querytType == "CREATE") {
      vector<string> namesOfColumns;
      vector<string> typesOfColumns;
      cin >> skip >> nameOfTable;
      string inputValue = "";
      int counter = 1;
      char currentChar, prevChar = nl;
      cin.get(currentChar);
      while (currentChar != pc && currentChar != nl) {
        if ((currentChar != sp && currentChar != po && currentChar != com) && (prevChar == sp || prevChar == po || prevChar == com)) {
          if (currentChar == cot) {
            string str = "";
            char tempChar;
            cin.get(tempChar);
            while (tempChar != cot) {
              str += tempChar;
              cin.get(tempChar);
            }
            inputValue = str;
            if (counter % 2 == 0)
              typesOfColumns.push_back(inputValue);
            else
              namesOfColumns.push_back(inputValue);
            counter++;
            inputValue = "";
            prevChar = cot;
            cin.get(currentChar);
            continue;
          }
          while (currentChar != pc && currentChar != com && currentChar != sp) {
            inputValue += currentChar;
            prevChar = currentChar;
            cin.get(currentChar);
          }
          if (counter % 2 == 0)
            typesOfColumns.push_back(inputValue);
          else
            namesOfColumns.push_back(inputValue);
          counter++;
          inputValue = "";
        }
        if (currentChar == pc)
          break;
        prevChar = currentChar;
        cin.get(currentChar);
      }
      DataTable table(namesOfColumns, typesOfColumns, nameOfTable);
      tables.push_back(table);
    } else if (querytType == "INSERT") {
      vector<string> values;
      cin >> skip >> nameOfTable >> skip;
      string inputValue = "";
      char currentChar, prevChar = nl;
      cin.get(currentChar);
      while (currentChar != pc && currentChar != nl) {
        if ((prevChar == po || prevChar == sp || prevChar == com) &&
            (currentChar != po && currentChar != sp && currentChar != com)) {
          if (currentChar == cot) {
            string str = "";
            char tempChar;
            cin.get(tempChar);
            while (tempChar != cot) {
              str += tempChar;
              cin.get(tempChar);
            }
            inputValue = str;
            values.push_back(inputValue);
            inputValue = "";
            prevChar = cot;
            cin.get(currentChar);
            continue;
          }
          while (currentChar != sp && currentChar != com && currentChar != pc) {
            inputValue += currentChar;
            prevChar = currentChar;
            cin.get(currentChar);
          }
          values.push_back(inputValue);
          inputValue = "";
        }
        if (currentChar == pc)
          break;
        prevChar = currentChar;
        cin.get(currentChar);
      }
      int tIndex = getDataTableIndex(tables, nameOfTable);
      tables[tIndex].insertRecord(values);
    } else if (querytType == "DELETE") {
      cin >> skip >> nameOfTable >> skip;
      string column, term, f, inp = "";
      char ch;
      cin.get(ch);
      while (ch != sm && ch != eq && ch != gr) {
        if (ch != sp)
          inp += ch;
        cin.get(ch);
      }
      column = inp;
      inp = "";
      if (ch == eq) {
        inp = "==";
        cin.get();
      } else {
        inp.push_back(ch);
      }
      term = inp;
      inp = "";
      cin.get(ch);
      while (ch != nl) {
        if (ch == cot) {
          string str = "";
          char tempChar;
          cin.get(tempChar);
          while (tempChar != cot) {
            str += tempChar;
            cin.get(tempChar);
          }
          inp = str;
          cin.get(ch);
          continue;
        }
        if (ch != sp)
          inp += ch;
        cin.get(ch);
      }
      f = inp;
      int tIndex = getDataTableIndex(tables, nameOfTable);
      tables[tIndex].removeRecord(column, term, f);
    } else if (querytType == "UPDATE") {
      cin >> nameOfTable >> skip;
      char currentChar, prevChar = nl;
      string inputValue = "";
      cin.get(currentChar);
      vector<string> values;
      while (currentChar != nl && currentChar != pc) {
        if ((prevChar == com || prevChar == po || prevChar == sp) &&
            (currentChar != com && currentChar != po && currentChar != sp)) {
          if (currentChar == cot) {
            string str = "";
            char tempChar;
            cin.get(tempChar);
            while (tempChar != cot) {
              str += tempChar;
              cin.get(tempChar);
            }
            inputValue = str;
            values.push_back(inputValue);
            inputValue = "";
            prevChar = cot;
            cin.get(currentChar);
            continue;
          }
          while (currentChar != sp && currentChar != pc && currentChar != com) {
            inputValue += currentChar;
            prevChar = currentChar;
            cin.get(currentChar);
          }
          values.push_back(inputValue);
          inputValue = "";
        }
        if (currentChar == pc)
          break;
        prevChar = currentChar;
        cin.get(currentChar);
      }
      cin >> skip;
      string column, term, f, inp = "";
      char ch;
      cin.get(ch);
      while (ch != eq && ch != sm && ch != gr) {
        if (ch != sp)
          inp += ch;
        cin.get(ch);
      }
      column = inp;
      inp = "";
      if (ch == eq) {
        inp = "==";
        cin.get();
      } else {
        inp.push_back(ch);
      }
      term = inp;
      inp = "";
      cin.get(ch);
      while (ch != nl) {
        if (ch == cot) {
          string str = "";
          char tempChar;
          cin.get(tempChar);
          while (tempChar != cot) {
            str += tempChar;
            cin.get(tempChar);
          }
          inp = str;
          cin.get(ch);
          continue;
        }
        if (ch != sp)
          inp += ch;
        cin.get(ch);
      }
      f = inp;
      int tIndex = getDataTableIndex(tables, nameOfTable);
      tables[tIndex].updateRecord(column, term, f, values);
    } else if (querytType == "SELECT") {
      vector<string> columnsN;
      string inputValue = "";
      char currentChar, prevChar = nl;
      cin.get(currentChar);
      while (currentChar != pc && currentChar != nl) {
        if (currentChar == st) {
          columnsN.push_back("*");
          break;
          ;
        }
        if ((prevChar == po || prevChar == sp || prevChar == com) &&
            (currentChar != po && currentChar != sp && currentChar != com)) {
          if (currentChar == cot) {
            string str = "";
            char tempChar;
            cin.get(tempChar);
            while (tempChar != cot) {
              str += tempChar;
              cin.get(tempChar);
            }
            inputValue = str;
            columnsN.push_back(inputValue);
            inputValue = "";
            prevChar = cot;
            cin.get(currentChar);
            continue;
          }
          while (currentChar != com && currentChar != sp && currentChar != pc) {
            inputValue += currentChar;
            prevChar = currentChar;
            cin.get(currentChar);
          }
          columnsN.push_back(inputValue);
          inputValue = "";
        }
        if (currentChar == pc)
          break;
        prevChar = currentChar;
        cin.get(currentChar);
      }
      cin >> skip >> nameOfTable >> skip;
      string column, term, f, inp = "";
      char ch;
      cin.get(ch);
      while (ch != eq && ch != sm && ch != gr) {
        if (ch != sp)
          inp += ch;
        cin.get(ch);
      }
      column = inp;
      inp = "";
      if (ch == eq) {
        inp = "==";
        cin.get();
      } else {
        inp.push_back(ch);
      }
      term = inp;
      inp = "";
      cin.get(ch);
      while (ch != nl) {
        if (ch == cot) {
          string str = "";
          char tempChar;
          cin.get(tempChar);
          while (tempChar != cot) {
            str += tempChar;
            cin.get(tempChar);
          }
          inp = str;
          cin.get(ch);
          continue;
        }
        if (ch != sp)
          inp += ch;
        cin.get(ch);
      }
      f = inp;
      int tIndex = getDataTableIndex(tables, nameOfTable);
      tables[tIndex].selectFields(columnsN, column, term, f);
    }
  }

  return 0;
}

long long int timestampToInt(string timestamp) {
  vector<string> parts;
  string sep = "/";
  int start = 0;
  int end = timestamp.find(sep);
  while (end != -1) {
    parts.push_back(timestamp.substr(start, end - start));
    start = end + sep.size();
    end = timestamp.find(sep, start);
  }
  parts.push_back(timestamp.substr(start, end - start));
  long long int v = 0;
  v = stoi(parts[0]);
  if (parts[1].size() == 1)
    parts[1] = '0' + parts[1];
  if (parts[2].size() == 1)
    parts[2] = '0' + parts[2];
  v *= 100;
  v += stoi(parts[1]);
  v *= 100;
  v += stoi(parts[2]);
  return v;
}

string intToTimestamp(long long int v) {
  int year = v / 10000;
  int month = (v % 10000) / 100;
  int day = (v % 10000) % 100;
  return to_string(year) + '/' + to_string(month) + '/' + to_string(day);
}

long long int Hash::code(string str) {
  for (auto &c : str)
    c = toupper(c);
  long long int toint = 0;
  for (auto &c : str) {
    toint *= 100;
    toint += (int)c;
  }
  return toint;
}

string Hash::decode(long long int v) {
  string str = "";
  int c = v % 100;
  str.push_back((char)c);
  v = v / 100;
  while (v > 0) {
    c = v % 100;
    str.push_back((char)c);
    v = v / 100;
  }
  for (auto &c : str)
    c = tolower(c);
  reverse(str.begin(), str.end());
  return str;
}

long long int codeValue(string value, string type) {
  if (type == DataTable::STRINGTYPE)
    return Hash::code(value);
  if (type == DataTable::TIMESTAMPTYPE)
    return timestampToInt(value);
  if (type == DataTable::INTEGERTYPE)
    return stoi(value);
  return LLONG_MAX;
}

string decodeValue(long long int value, string type) {
  if (type == DataTable::STRINGTYPE)
    return Hash::decode(value);
  if (type == DataTable::TIMESTAMPTYPE)
    return intToTimestamp(value);
  if (type == DataTable::INTEGERTYPE)
    return to_string(value);
  return "";
}

int getDataTableIndex(vector<DataTable> dataTables, string tableName) {
  for (int i = 0; i < dataTables.size(); i++)
    if (dataTables[i].tableName == tableName)
      return i;
  cout << "\nerror in getDataTableIndex\n\n";
  return -1;
}

TreeNode::TreeNode(int md, bool l) {
  minimumDegree = md;
  keys = new Node *[2 * minimumDegree - 1];
  child = new TreeNode *[2 * minimumDegree];
  leaf = l;
  numberOfKeys = 0;
}

void TreeNode::addToNotFullTreeNode(Node *key) {
  int i = numberOfKeys - 1;
  if (leaf == false) {
    while (i >= 0 && keys[i]->data > key->data)
      i--;
    if (child[i + 1]->numberOfKeys == 2 * minimumDegree - 1) {
      splitingChildOfTreeNode(child[i + 1], i + 1);
      if (keys[i + 1]->data < key->data)
        i++;
    }
    child[i + 1]->addToNotFullTreeNode(key);
  } else {
    while (i >= 0 && keys[i]->data > key->data) {
      keys[i + 1] = keys[i];
      i--;
    }
    numberOfKeys++;
    keys[i + 1] = key;
    keys[i + 1]->self = this;
  }
}

void TreeNode::searchInSubTree(vector<Node *> *matches, long long int k, string pattern) {
  if (pattern == "==") {
    int i;
    for (i = 0; i < numberOfKeys; i++) {
      if (leaf == false)
        child[i]->searchInSubTree(matches, k, pattern);
      if (keys[i]->data == k)
        matches->push_back(keys[i]);
    }
    if (leaf == false)
      child[i]->searchInSubTree(matches, k, pattern);

  } else if (pattern == "<") {
    int i;
    for (i = 0; i < numberOfKeys; i++) {
      if (leaf == false)
        child[i]->searchInSubTree(matches, k, pattern);
      if (keys[i]->data < k)
        matches->push_back(keys[i]);
    }
    if (leaf == false)
      child[i]->searchInSubTree(matches, k, pattern);

  } else if (pattern == ">") {
    int i;
    for (i = 0; i < numberOfKeys; i++) {
      if (leaf == false)
        child[i]->searchInSubTree(matches, k, pattern);
      if (keys[i]->data > k)
        matches->push_back(keys[i]);
    }
    if (leaf == false)
      child[i]->searchInSubTree(matches, k, pattern);
  }
}

void TreeNode::splitingChildOfTreeNode(TreeNode *c1, int x) {
  TreeNode *c2 = new TreeNode(c1->minimumDegree, c1->leaf);
  c2->numberOfKeys = minimumDegree - 1;
  
  for (int i = 0; i < minimumDegree - 1; i++)
    c2->keys[i] = c1->keys[i + minimumDegree];

  if (c1->leaf == false)
    for (int i = 0; i < minimumDegree; i++)
      c2->child[i] = c1->child[i + minimumDegree];

  c1->numberOfKeys = minimumDegree - 1;
  for (int i = numberOfKeys; i >= x + 1; i--)
    child[i + 1] = child[i];
    
  child[x + 1] = c2;
  for (int j = numberOfKeys - 1; j >= x; j--)
    keys[j + 1] = keys[j];

  keys[x] = c1->keys[minimumDegree - 1];
  numberOfKeys++;
}

void TreeNode::takeKeyFromPrevious(int index) {
  TreeNode *current = child[index];
  TreeNode *brother = child[index - 1];
  for (int i = current->numberOfKeys - 1; i >= 0; --i)
    current->keys[i + 1] = current->keys[i];
  if (current->leaf == false) {
    for (int i = current->numberOfKeys; i >= 0; --i)
      current->child[i + 1] = current->child[i];
  }
  current->keys[0] = keys[index - 1];
  if (current->leaf == false)
    current->child[0] = brother->child[brother->numberOfKeys];
  keys[index - 1] = brother->keys[brother->numberOfKeys - 1];
  (current->numberOfKeys)++;
  (brother->numberOfKeys)--;
}

void TreeNode::removeKeyFromSubTree(Node *key) {
  int index = searchForKey(key);
  if (numberOfKeys > index && keys[index] == key) {
    if (leaf)
      removeKeyFromLeaf(index);
    else
      removeKeyFromNotLeaf(index);
  } 
  else {
    if (leaf)
      return;

    bool present = ((index == numberOfKeys) ? true : false);
    if (minimumDegree > child[index]->numberOfKeys)
      fillChild(index);
    if (present && numberOfKeys < index)
      child[index - 1]->removeKeyFromSubTree(key);
    else
      child[index]->removeKeyFromSubTree(key);
  }
}

int TreeNode::searchForKey(Node *key) {
  int index = 0;
  while (numberOfKeys > index && keys[index]->data < key->data)
    index++;
  return index;
}

void TreeNode::removeKeyFromLeaf(int index) {
  for (int i = index + 1; i < numberOfKeys; ++i)
    keys[i - 1] = keys[i];
  numberOfKeys--;
}

void TreeNode::removeKeyFromNotLeaf(int index) {
  Node *key = keys[index];
  if (minimumDegree <= child[index]->numberOfKeys) {
    keys[index] = predecessorOfKey(index);
    child[index]->removeKeyFromSubTree(predecessorOfKey(index));
  } 
  else if (minimumDegree <= child[index + 1]->numberOfKeys) {
    keys[index] = successorOfKey(index);
    child[index + 1]->removeKeyFromSubTree(successorOfKey(index));
  } 
  else {
    mergeWithBrother(index);
    child[index]->removeKeyFromSubTree(key);
  }
}

void TreeNode::takeKeyFromNext(int index) {
  TreeNode *current = child[index];
  TreeNode *brother = child[index + 1];
  current->keys[(current->numberOfKeys)] = keys[index];
  if (current->leaf == false)
    current->child[(current->numberOfKeys) + 1] = brother->child[0];

  keys[index] = brother->keys[0];
  for (int i = 1; i < brother->numberOfKeys; ++i)
    brother->keys[i - 1] = brother->keys[i];

  if (brother->leaf == false) {
    for (int i = 1; i <= brother->numberOfKeys; ++i)
      brother->child[i - 1] = brother->child[i];
  }
  (current->numberOfKeys)++;
  (brother->numberOfKeys)--;
}

void TreeNode::fillChild(int index) {
  if (index != 0 && minimumDegree <= child[index - 1]->numberOfKeys) {
    takeKeyFromPrevious(index);
  } 
  else if (index != numberOfKeys && minimumDegree <= child[index + 1]->numberOfKeys) {
    takeKeyFromNext(index);
  } 
  else {
    if (index != numberOfKeys) {
      mergeWithBrother(index);
    } else {
      mergeWithBrother(index - 1);
    }
  }
}

Node *TreeNode::predecessorOfKey(int index) {
  TreeNode *current = child[index];
  while (current->leaf == false)
    current = current->child[current->numberOfKeys];
  return current->keys[current->numberOfKeys - 1];
}

Node *TreeNode::successorOfKey(int index) {
  TreeNode *current = child[index + 1];
  while (current->leaf == false)
    current = current->child[0];
  return current->keys[0];
}

void TreeNode::mergeWithBrother(int index) {
  TreeNode *current = child[index];
  TreeNode *brother = child[index + 1];
  current->keys[minimumDegree - 1] = keys[index];
  for (int i = 0; i < brother->numberOfKeys; ++i)
    current->keys[i + minimumDegree] = brother->keys[i];
  if (current->leaf == false) {
    for (int i = 0; i <= brother->numberOfKeys; ++i)
      current->child[i + minimumDegree] = brother->child[i];
  }
  for (int i = index + 1; i < numberOfKeys; ++i)
    keys[i - 1] = keys[i];
  for (int i = index + 2; i <= numberOfKeys; ++i)
    child[i - 1] = child[i];
  (current->numberOfKeys) += ((brother->numberOfKeys) + 1);
  numberOfKeys--;
  delete (brother);
}

Tree::Tree(int md) {
  minimumDegree = md;
  root = NULL;
}

void Tree::searchInTree(vector<Node *> *matches, long long int k,
                        string pattern) {
  if (root != NULL)
    root->searchInSubTree(matches, k, pattern);
}

void Tree::insertNewKey(Node *key) {
  if (root == NULL) {
    root = new TreeNode(minimumDegree, true);
    root->numberOfKeys = 1;
    root->keys[0] = key;
    root->keys[0]->self = root;
  } else {
    if (root->numberOfKeys == 2 * minimumDegree - 1) {
      TreeNode *temp = new TreeNode(minimumDegree, false);
      temp->child[0] = root;
      temp->splitingChildOfTreeNode(root, 0);
      int index = 0;
      if (temp->keys[0]->data < key->data)
        index++;
      temp->child[index]->addToNotFullTreeNode(key);
      root = temp;
    } else {
      root->addToNotFullTreeNode(key);
    }
  }
}

void Tree::removeKeyFromTree(Node *key) {
  root->removeKeyFromSubTree(key);
  if (root->numberOfKeys == 0) {
    if (root->leaf == false)
      root = root->child[0];
    else
      root = NULL;
  }
}

LinkedList::LinkedList() { head = NULL; }

void LinkedList::insertToLinkedList(Node *node) {
  Node *current = head;
  node->nextField = head;
  if (head != NULL) {
    while (current->nextField != head)
      current = current->nextField;
    current->nextField = node;
  } else {
    node->nextField = node;
  }
  head = node;
}

vector<Node *> LinkedList::getLinkedList() {
  vector<Node *> l;
  Node *current = head;
  do {
    l.push_back(current);
    current = current->nextField;
  } while (current != head);
  reverse(l.begin(), l.end());
  return l;
}

const string DataTable::INTEGERTYPE = "int";
const string DataTable::STRINGTYPE = "string";
const string DataTable::TIMESTAMPTYPE = "timestamp";

DataTable::DataTable(vector<string> nc, vector<string> tc, string n) {
  nameOfColumns = nc;
  nameOfColumns.insert(nameOfColumns.begin(), idColumnName);
  typeOfColumns = tc;
  typeOfColumns.insert(typeOfColumns.begin(), idColumnType);
  numberOfColumns = nameOfColumns.size();
  tableName = n;
  initialColumns();
}

void DataTable::initialColumns() {
  for (int i = 0; i < numberOfColumns; i++) {
    Tree column(3);
    columns.push_back(column);
  }
}

int DataTable::getColumnIndex(string nc) {
  for (int i = 0; i < numberOfColumns; i++)
    if (nameOfColumns[i] == nc)
      return i;
  cout << "\nerror in getColumnIndex\n\n";
  return -1;
}

void DataTable::insertRecord(vector<string> values) {
  if (values.size() != (numberOfColumns - 1)) {
    cout << "input values: " << values.size();
    cout << " and table columns: " << numberOfColumns - 1;
    cout << " not match\n";
    return;
  }

  int id;
  int x = 1;
  while (true) {
    vector<Node *> matches;
    const string pattern = "==";
    columns[0].searchInTree(&matches, x, pattern);
    if (matches.empty()) {
      id = x;
      break;
    }
    x++;
  }
  values.insert(values.begin(), to_string(id));

  LinkedList record;
  for (int i = 0; i < numberOfColumns; i++) {
    Node *key = new Node();
    key->data = codeValue(values[i], typeOfColumns[i]);
    record.insertToLinkedList(key);
    columns[i].insertNewKey(key);
  }
  records.push_back(record);
}

int DataTable::findRecordIndex(Node *key) {
  for (int i = 0; i < records.size(); i++) {
    vector<Node *> l = records[i].getLinkedList();
    for (int j = 0; j < l.size(); j++)
      if (key == l[j])
        return i;
  }
  cout << "\nerror in findRecordIndex\n" << key->data << "\n\n";
  return -1;
}

void DataTable::updateRecord(string nc, string pattern, string f,
                             vector<string> values) {
  int columnIndex = getColumnIndex(nc);
  long long int v = codeValue(f, typeOfColumns[columnIndex]);
  vector<Node *> matches;
  columns[columnIndex].searchInTree(&matches, v, pattern);

  for (int i = 0; i < matches.size(); i++) {
    int recordIndex = findRecordIndex(matches[i]);
    vector<Node *> l = records[recordIndex].getLinkedList();

    for (int j = 1; j < l.size(); j++)
      l[j]->data = codeValue(values[j - 1], typeOfColumns[j]);
  }
}

void DataTable::removeRecord(string nc, string pattern, string f) {
  int columnIndex = getColumnIndex(nc);
  long long int v = codeValue(f, typeOfColumns[columnIndex]);
  vector<Node *> matches;
  columns[columnIndex].searchInTree(&matches, v, pattern);
  for (int i = 0; i < matches.size(); i++) {
    int recordIndex = findRecordIndex(matches[i]);

    vector<Node *> l = records[recordIndex].getLinkedList();
    for (int j = 0; j < l.size(); j++)
      columns[j].removeKeyFromTree(l[j]);
    
  }
}

void DataTable::selectFields(vector<string> columnNames, string nc, string pattern, string f) {
  vector<int> selectedIndex;
  if (columnNames[0] == "*") {
    for (int i = 0; i < numberOfColumns; i++)
      selectedIndex.push_back(i);
  } 
  else {
    for (int i = 0; i < columnNames.size(); i++)
      selectedIndex.push_back(getColumnIndex(columnNames[i]));
  }
  
  int columnIndex = getColumnIndex(nc);
  long long int v = codeValue(f, typeOfColumns[columnIndex]);
  vector<vector<Node *>> unsort;
  vector<Node *> matches;
  columns[columnIndex].searchInTree(&matches, v, pattern);
  for (int i = 0; i < matches.size(); i++) {
    int recordIndex = findRecordIndex(matches[i]);
    vector<Node *> l = records[recordIndex].getLinkedList();
    unsort.push_back(l);
  }

  for (int i = 0; i < unsort.size() - 1; i++)
    for (int j = 0; j < unsort.size() - i - 1; j++)
      if (unsort[j][0]->data > unsort[j + 1][0]->data) {
        vector<Node *> temp = unsort[j];
        unsort[j] = unsort[j + 1];
        unsort[j + 1] = temp;
      }

  for (int i = 0; i < unsort.size(); i++) {
    for (int j = 0; j < selectedIndex.size(); j++)
      cout << decodeValue(unsort[i][selectedIndex[j]]->data,
                          typeOfColumns[selectedIndex[j]])
           << " ";
    cout << '\n';
  }
}