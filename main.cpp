//11327132李晨希 11327141羅元廷
//ds第五次作業
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <queue>

using namespace std;

// **************** Utility Functions ************************

void ShowMenu() {
  cout << "*** (^_^) Data Structure (^o^) ***" << endl;
  cout << "** Binary Search Tree on Pokemon *" << endl;
  cout << "* 0. QUIT                        *" << endl;
  cout << "* 1. Read a file to build HP BST *" << endl;
  cout << "* 2. Range search on HP field    *" << endl;
  cout << "* 3. Delete the min on HP field  *" << endl;
  cout << "* 4. Rebuild the balanced HP BST *" << endl;
  cout << "**********************************" << endl;
}
void HandleInvalidInput(const string &message) {
  cout << message << endl;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
string GetFileNumber() {
  string input;
    
  while (true) {
    getline(cin, input);
    size_t start = input.find_first_not_of(" \t\r\n");
    size_t end = input.find_last_not_of(" \t\r\n");
        
    if (start != string::npos) {
      input = input.substr(start, end - start + 1);
        return input;  // 有內容就回傳
    }
  }
}
int GetValidCommand() {
  string line;

  while (true) {
    getline(cin, line);
    size_t start = line.find_first_not_of(" \t\r\n");

    // 如果只按 Enter 直接忽略重讀
    if (start == string::npos) {
      continue;
    }

    size_t end = line.find_last_not_of(" \t\r\n");
    string input = line.substr(start, end - start + 1);

    // 必須是單一數字 0~4
    if (input.size() == 1 &&
        input[0] >= '0' &&
        input[0] <= '4') {
      return input[0] - '0';
    }

    HandleInvalidInput("Command does not exist!");
    ShowMenu();
  }
}
void GetNonNegativeInts(int& smaller, int& larger) {
  int a, b;

  // 讀第一個數字
  while (true) {
    cout << "Input a non-negative integer: ";
    cin >> a;
    if (a >= 0) break;
    cout << "### It is NOT a non-negative integer. ###" << endl;
  }

  // 讀第二個數字
  while (true) {
    cout << "Input a non-negative integer: ";
    cin >> b;
    if (b >= 0) break;
    cout << "### It is NOT a non-negative integer. ###" << endl;
  }

  // 排序
  if (a <= b) {
    smaller = a;
    larger = b;
  } else {
    smaller = b;
    larger = a;
  }
}

// ***********************************************************
struct Pokemon {
  int number;          // 編號
  string name;         // 名稱
  string type1;        // 類型1
  string type2;        // 類型2
  int total;           // 總和
  int hp;              // 生命值
  int attack;          // 攻擊
  int defense;         // 防禦
  int spAtk;           // 特攻
  int spDef;           // 特防
  int speed;           // 速度
  int generation;      // 世代
  string legendary;    // 傳說與否
};

struct TreeNode {
  int hp;                    // 生命
  vector<int> ids;           // 相同HP的所有編號（依原始順序）
  TreeNode* left;
  TreeNode* right;
    
  TreeNode(int h, int id) : hp(h), left(nullptr), right(nullptr) {
      ids.push_back(id);
  }
};

class BST {
 private:
  TreeNode* root;
  bool deleteMinNext;  // true:刪最小, false:刪最大
    
  // 計算樹高（遞迴）
  int getHeight(TreeNode* node) {
    if (!node) return 0;
    return 1 + max(getHeight(node->left), getHeight(node->right));
  }
    
  // 刪除最小節點（遞迴）
  TreeNode* deleteMin(TreeNode* node, vector<int>& deletedIds) {
    if (!node->left) {
      // 找到最小節點
      deletedIds = node->ids;  // 保存被刪除的ID
      TreeNode* rightChild = node->right;
      delete node;
      return rightChild;
    }
    node->left = deleteMin(node->left, deletedIds);
    return node;
  }
    
    // 刪除最大節點（遞迴）
  TreeNode* deleteMax(TreeNode* node, vector<int>& deletedIds) {
    if (!node->right) {
    // 找到最大節點
      deletedIds = node->ids;  // 保存被刪除的ID
      TreeNode* leftChild = node->left;
      delete node;
      return leftChild;
    }
    node->right = deleteMax(node->right, deletedIds);
    return node;
  }
    
    // 中序遍歷收集所有節點
  void inorderCollect(TreeNode* node, vector<TreeNode*>& nodes) {
    if (!node) return;
      inorderCollect(node->left, nodes);
      nodes.push_back(node);
      inorderCollect(node->right, nodes);
  }
    
    // 用排序好的節點陣列建立平衡BST
  TreeNode* buildBalanced(vector<TreeNode*>& nodes, int start, int end) {
    if (start > end) return nullptr;
      
    int mid = start + (end - start) / 2;
    TreeNode* node = nodes[mid];
        
    // 遞迴建立左右子樹
    node->left = buildBalanced(nodes, start, mid - 1);
    node->right = buildBalanced(nodes, mid + 1, end);
        
    return node;
  }
    
    // 層序遍歷顯示樹結構
  void printLevelOrder() {
    if (!root) return;
        
    queue<pair<TreeNode*, int>> q;  // (節點, 層級)
    q.push({root, 1});
    int currentLevel = 1;
        
    while (!q.empty()) {
      auto [node, level] = q.front();
      q.pop();
            
      if (level > currentLevel) {
        cout << endl;
        currentLevel = level;
      }
            
      // 顯示節點內容：<level N> (HP, ID1|ID2|...)
      cout << "<level " << level << "> (" << node->hp;
      for (size_t i = 0; i < node->ids.size(); i++) {
        if (i == 0) cout << ", ";
        else cout << "|";
        cout << node->ids[i];
      }
      cout << ")";
            
      if (!q.empty() && q.front().second == level) {
        cout << " ";
      }
            
    // 加入子節點
    if (node->left) q.push({node->left, level + 1});
      if (node->right) q.push({node->right, level + 1});
    }
    cout << endl;
  }
    
  // 範圍查詢（遞迴+剪枝）
  void rangeSearchHelper(TreeNode* node, int minHP, int maxHP,
                          vector<int>& result, int& visitCount) {
    if (node == nullptr) return;
        
      visitCount++;  // 記錄走訪節點數
        
      if (node->hp < minHP) {
        // 當前節點太小，只需往右找
        
        rangeSearchHelper(node->right, minHP, maxHP, result, visitCount);
      } else if (node->hp > maxHP) {
        // 當前節點太大，只需往左找
        
        rangeSearchHelper(node->left, minHP, maxHP, result, visitCount);
      } else {
        // 當前節點在範圍內
        // 先找左子樹（可能有更小但仍在範圍內的）
        // 用inorder遍歷，hp才能遞減排列
        
        rangeSearchHelper(node->right, minHP, maxHP, result, visitCount);
            
        // 加入當前節點所有ID
        for (int id : node->ids) {
          result.push_back(id);
        }
            
        rangeSearchHelper(node->left, minHP, maxHP, result, visitCount);
    }
  }
    
  // 釋放整棵樹的記憶體
  void clear(TreeNode* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
    }
    
public:
    BST() : root(nullptr), deleteMinNext(true) {}
    
    ~BST() {
        clear(root);
    }
    
    // 插入節點（迴圈實作）
    void insert(int hp, int id) {
        if (root == nullptr) {
            root = new TreeNode(hp, id);
            return;
        }
        
        TreeNode* current = root;
        while (true) {
            if (hp == current->hp) {
                // 相同HP，加入同一節點（保持原始順序）
                current->ids.push_back(id);
                return;
            } else if (hp < current->hp) {
                if (current->left == nullptr) {
                    current->left = new TreeNode(hp, id);
                    return;
                }
                current = current->left;
            } else if (hp > current->hp) {
                if (current->right == nullptr) {
                    current->right = new TreeNode(hp, id);
                    return;
                }
                current = current->right;
            }
        }
    }
    
    // 取得樹高
    int height() {
        return getHeight(root);
    }
    
    // 檢查是否為空
    bool isEmpty() {
        return root == nullptr;
    }
    
    // 範圍查詢，會回傳依 hp 遞減排列的 id 陣列
    vector<int> rangeSearch(int minHP, int maxHP, int& visitCount) {
      vector<int> result;
      visitCount = 0;
      rangeSearchHelper(root, minHP, maxHP, result, visitCount);
      return result;
    }
    
    // 刪除極值節點（交替刪除最小/最大）
    vector<int> deleteExtreme() {
        vector<int> deletedIds;
        
        if (!root) return deletedIds;
        
        if (deleteMinNext) {
            root = deleteMin(root, deletedIds);
        } else {
            root = deleteMax(root, deletedIds);
        }
        
        deleteMinNext = !deleteMinNext;  // 下次刪另一端
        return deletedIds;
    }
    
    // 重建平衡樹
    void rebuild() {
        if (!root) return;
        
        // 1. 中序遍歷收集所有節點
        vector<TreeNode*> nodes;
        inorderCollect(root, nodes);
        
        // 2. 用分治法重建平衡樹
        root = buildBalanced(nodes, 0, nodes.size() - 1);
    }
    
    // 顯示樹結構（層序遍歷）
    void display() {
        cout << "HP tree:" << endl;
        printLevelOrder();
    }
};

bool readFile(string filename, vector<Pokemon>& pokemons) {
        ifstream file(filename);
    
    if (!file.is_open()) {
        return false;
    }
  
   // 把舊資料清掉
    pokemons.clear();
  
    string line;
    
    // 讀取標題列（跳過）
    getline(file, line);
    
    // 讀取資料
    while (getline(file, line)) {
      stringstream ss(line);
      Pokemon p;
        
      ss >> p.number >> p.name >> p.type1 >> p.type2
         >> p.total >> p.hp >> p.attack >> p.defense
         >> p.spAtk >> p.spDef >> p.speed >> p.generation >> p.legendary;
        
      pokemons.push_back(p);
    }
    
    file.close();
    return true;
}

// 根據ID找到寶可夢
const Pokemon* findPokemonById(const vector<Pokemon>& pokemons, int id) {
    auto it = find_if(pokemons.begin(), pokemons.end(),
                     [id](const Pokemon& p) { return p.number == id; });
    return (it != pokemons.end()) ? &(*it) : nullptr;
}

// 任務一：讀檔並建樹
void task1(vector<Pokemon>& pokemons, BST& hpTree) {
  
  cout << "Input a file number [0: quit]: ";
  
  string fileNum = GetFileNumber();
  if (fileNum == "0") return;
  
  cout << "\n";
    
  string filename = "input" + fileNum + ".txt";
  if (!readFile(filename, pokemons)) {
    cout << "### " << filename << " does not exist! ###" << endl;
    return;
  }
    
  // 顯示前4個欄位
  cout << "\t#\tName\t\t\tType 1\t\tHP" << endl;
  for (int i = 0; i < pokemons.size(); i++) {
    cout << "[ " << setw(2) << (i + 1) << "]\t"
    << pokemons[i].number << "\t"
    << left << setw(24) << pokemons[i].name
    << setw(16) << pokemons[i].type1
    << pokemons[i].hp << "    " << endl;
  }
    
  // 建立HP二元搜尋樹
  hpTree = BST();  // 重建新樹
  for (int i = 0; i < pokemons.size(); i++) {
      hpTree.insert(pokemons[i].hp, pokemons[i].number);
  }
    
  cout << "HP tree height = " << hpTree.height() << endl;
}

// 任務二：範圍查詢
void task2(vector<Pokemon>& pokemons, BST& hpTree) {
  // 讀入兩個整數後比較大小
  int minhp, maxhp;
  GetNonNegativeInts(minhp, maxhp);
  
  int visitCount = 0;
  vector<int> result = hpTree.rangeSearch(minhp, maxhp, visitCount);
    
  if (result.empty()) {
    cout << "No record was found in the specified range." << endl;
  }
        
  // 顯示結果
  cout << "\t#\tName\t\t\tType 1\t\tTotal\tHP\tAttack\tDefense" << endl;
  
  for (size_t i = 0; i < result.size(); i++) {
    const Pokemon* p = findPokemonById(pokemons, result[i]);
    if (p != nullptr) {
      cout << "[ " << setw(2) << (i + 1) << "]\t"
           << p->number << "\t"
           << left << setw(24) << p->name
           << setw(16) << p->type1
           << p->total << "\t"
           << p->hp << "\t"
           << p->attack << "\t"
           << p->defense << endl;
          }
      }
    
    
    cout << "Number of visited nodes = " << visitCount << endl;
}

// 任務三：刪除極值節點
void task3(vector<Pokemon>& pokemons, BST& hpTree) {
  if (pokemons.empty()) {
    cout << "Please load data first (Task 1)!" << endl;
    return;
  }
    
  if (hpTree.isEmpty()) {
    cout << "Tree is empty. Please rebuild tree (Task 1)!" << endl;
    return;
  }
    
    auto deletedIds = hpTree.deleteExtreme();
    
    // 顯示被刪除的資料
    cout << "\t#\tName\t\t\tType 1\t\tTotal\tHP\tAttack\tDefense\tSp. Atk\tSp. Def" << endl;
    for (size_t i = 0; i < deletedIds.size(); i++) {
        const Pokemon* p = findPokemonById(pokemons, deletedIds[i]);
        if (p) {
            cout << "[ " << setw(2) << (i + 1) << "]\t"
                 << p->number << "\t"
                 << left << setw(24) << p->name
                 << setw(16) << p->type1
                 << p->total << "\t"
                 << p->hp << "\t"
                 << p->attack << "\t"
                 << p->defense << "\t"
                 << p->spAtk << "\t"
                 << p->spDef << endl;
        }
    }
    
    cout << "HP tree height = " << hpTree.height() << endl;
}

// 任務四：重建平衡樹
void task4(vector<Pokemon>& pokemons, BST& hpTree) {
    if (pokemons.empty()) {
        cout << "----- Execute Mission 1 first! -----" << endl;
        return;
    }
    
    hpTree.rebuild();
    hpTree.display();
}

int main() {
  vector<Pokemon> pokemons;
  BST hpTree;
  int command;
    
  while (true) {
    ShowMenu();
    int command = GetValidCommand();

    if (command == 0) return 0;

    if (command == 1) {
      task1(pokemons, hpTree);
      continue;
    }

    if (command >= 2 && command <= 4 && pokemons.empty()) {
      cout << "Please load data first (Task 1)!" << endl;
      continue;
    }

    if (command == 2) {
      task2(pokemons, hpTree);
    } else if (command == 3) {
      task3(pokemons, hpTree);
    } else if (command == 4) {
      task4(pokemons, hpTree);
    } else {
      cout << "Invalid choice!" << endl;
    }
  }
    
    return 0;
}
