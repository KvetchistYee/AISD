#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

struct RBNode {
    int data;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    bool isRed;
    RBNode(int val) : data(val), left(nullptr), right(nullptr), parent(nullptr), isRed(true) {}
};

struct Stack {
    RBNode** nodes;
    int top;
    int capacity;
};

struct Queue {
    RBNode** nodes;
    int front, rear;
    int capacity;
};

Stack* createStack(int capacity) {
    Stack* stack = new Stack;
    stack->nodes = new RBNode * [capacity];
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void push(Stack* stack, RBNode* node) {
    if (stack->top < stack->capacity - 1) {
        stack->nodes[++stack->top] = node;
    }
}

RBNode* pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->nodes[stack->top--];
    }
    return nullptr;
}

bool isEmpty(Stack* stack) {
    return stack->top == -1;
}

Queue* createQueue(int capacity) {
    Queue* queue = new Queue;
    queue->nodes = new RBNode * [capacity];
    queue->front = 0;
    queue->rear = -1;
    queue->capacity = capacity;
    return queue;
}

void enqueue(Queue* queue, RBNode* node) {
    if (queue->rear < queue->capacity - 1) {
        queue->nodes[++queue->rear] = node;
    }
}

RBNode* dequeue(Queue* queue) {
    if (queue->front <= queue->rear) {
        return queue->nodes[queue->front++];
    }
    return nullptr;
}

bool isEmpty(Queue* queue) {
    return queue->front > queue->rear;
}

bool checkBrackets(const string& s) {
    int count = 0;
    for (char c : s) {
        if (c == '(') count++;
        if (c == ')') count--;
        if (count < 0) return false;
    }
    return count == 0;
}

TreeNode* parseTree(const string& s, int& pos, bool& valid) {
    while (pos < s.length() && s[pos] == ' ') pos++;

    if (pos >= s.length() || s[pos] != '(') {
        valid = false;
        return nullptr;
    }
    pos++;

    while (pos < s.length() && s[pos] == ' ') pos++;

    if (pos >= s.length() || !(isdigit(s[pos]) || s[pos] == '-')) {
        valid = false;
        return nullptr;
    }

    int num = 0;
    bool negative = false;

    if (s[pos] == '-') {
        negative = true;
        pos++;
    }

    while (pos < s.length() && isdigit(s[pos])) {
        num = num * 10 + (s[pos] - '0');
        pos++;
    }

    if (negative) num = -num;

    TreeNode* node = new TreeNode(num);
    node->left = nullptr;
    node->right = nullptr;

    while (pos < s.length() && s[pos] == ' ') pos++;

    if (pos < s.length() && s[pos] == '(') {
        node->left = parseTree(s, pos, valid);
        if (!valid) return nullptr;
    }

    while (pos < s.length() && s[pos] == ' ') pos++;

    if (pos < s.length() && s[pos] == '(') {
        node->right = parseTree(s, pos, valid);
        if (!valid) return nullptr;
    }

    while (pos < s.length() && s[pos] == ' ') pos++;

    if (pos >= s.length() || s[pos] != ')') {
        valid = false;
        return nullptr;
    }
    pos++;

    return node;
}

void dfs(TreeNode* root) {
    if (root == nullptr) return;
    cout << root->data << " ";
    dfs(root->left);
    dfs(root->right);
}

void leftRotate(RBNode*& root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;

    if (y->left != nullptr) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void rightRotate(RBNode*& root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;

    if (x->right != nullptr) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == nullptr) {
        root = x;
    }
    else if (y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void fixInsert(RBNode*& root, RBNode* node) {
    while (node != root && node->parent->isRed) {
        if (node->parent == node->parent->parent->left) {
            RBNode* uncle = node->parent->parent->right;

            if (uncle != nullptr && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(root, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                rightRotate(root, node->parent->parent);
            }
        }
        else {
            RBNode* uncle = node->parent->parent->left;

            if (uncle != nullptr && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rightRotate(root, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                leftRotate(root, node->parent->parent);
            }
        }
    }
    root->isRed = false;
}

RBNode* insertRB(RBNode* root, int value) {
    RBNode* newNode = new RBNode(value);

    if (root == nullptr) {
        newNode->isRed = false;
        return newNode;
    }

    RBNode* parent = nullptr;
    RBNode* current = root;

    while (current != nullptr) {
        parent = current;
        if (value < current->data) {
            current = current->left;
        }
        else if (value > current->data) {
            current = current->right;
        }
        else {
            delete newNode;
            return root;
        }
    }

    newNode->parent = parent;
    if (value < parent->data) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    fixInsert(root, newNode);
    return root;
}

RBNode* findMin(RBNode* node) {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

void fixDelete(RBNode*& root, RBNode* node) {
    while (node != root && !node->isRed) {
        if (node == node->parent->left) {
            RBNode* sibling = node->parent->right;

            if (sibling->isRed) {
                sibling->isRed = false;
                node->parent->isRed = true;
                leftRotate(root, node->parent);
                sibling = node->parent->right;
            }

            if ((sibling->left == nullptr || !sibling->left->isRed) &&
                (sibling->right == nullptr || !sibling->right->isRed)) {
                sibling->isRed = true;
                node = node->parent;
            }
            else {
                if (sibling->right == nullptr || !sibling->right->isRed) {
                    if (sibling->left != nullptr) {
                        sibling->left->isRed = false;
                    }
                    sibling->isRed = true;
                    rightRotate(root, sibling);
                    sibling = node->parent->right;
                }

                sibling->isRed = node->parent->isRed;
                node->parent->isRed = false;
                if (sibling->right != nullptr) {
                    sibling->right->isRed = false;
                }
                leftRotate(root, node->parent);
                node = root;
            }
        }
        else {
            RBNode* sibling = node->parent->left;

            if (sibling->isRed) {
                sibling->isRed = false;
                node->parent->isRed = true;
                rightRotate(root, node->parent);
                sibling = node->parent->left;
            }

            if ((sibling->right == nullptr || !sibling->right->isRed) &&
                (sibling->left == nullptr || !sibling->left->isRed)) {
                sibling->isRed = true;
                node = node->parent;
            }
            else {
                if (sibling->left == nullptr || !sibling->left->isRed) {
                    if (sibling->right != nullptr) {
                        sibling->right->isRed = false;
                    }
                    sibling->isRed = true;
                    leftRotate(root, sibling);
                    sibling = node->parent->left;
                }

                sibling->isRed = node->parent->isRed;
                node->parent->isRed = false;
                if (sibling->left != nullptr) {
                    sibling->left->isRed = false;
                }
                rightRotate(root, node->parent);
                node = root;
            }
        }
    }
    if (node != nullptr) {
        node->isRed = false;
    }
}

RBNode* deleteRB(RBNode* root, int value) {
    if (root == nullptr) return nullptr;

    RBNode* nodeToDelete = root;
    RBNode* successor = nullptr;
    RBNode* child = nullptr;

    while (nodeToDelete != nullptr) {
        if (value == nodeToDelete->data) {
            break;
        }
        else if (value < nodeToDelete->data) {
            nodeToDelete = nodeToDelete->left;
        }
        else {
            nodeToDelete = nodeToDelete->right;
        }
    }

    if (nodeToDelete == nullptr) return root;

    bool originalColor = nodeToDelete->isRed;

    if (nodeToDelete->left == nullptr) {
        child = nodeToDelete->right;
        if (child != nullptr) {
            child->parent = nodeToDelete->parent;
        }

        if (nodeToDelete->parent == nullptr) {
            root = child;
        }
        else if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = child;
        }
        else {
            nodeToDelete->parent->right = child;
        }

        delete nodeToDelete;
    }
    else if (nodeToDelete->right == nullptr) {
        child = nodeToDelete->left;
        if (child != nullptr) {
            child->parent = nodeToDelete->parent;
        }

        if (nodeToDelete->parent == nullptr) {
            root = child;
        }
        else if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = child;
        }
        else {
            nodeToDelete->parent->right = child;
        }

        delete nodeToDelete;
    }
    else {
        successor = findMin(nodeToDelete->right);
        originalColor = successor->isRed;
        child = successor->right;

        if (successor->parent == nodeToDelete) {
            if (child != nullptr) {
                child->parent = successor;
            }
        }
        else {
            if (child != nullptr) {
                child->parent = successor->parent;
            }
            successor->parent->left = child;

            successor->right = nodeToDelete->right;
            if (successor->right != nullptr) {
                successor->right->parent = successor;
            }
        }

        if (nodeToDelete->parent == nullptr) {
            root = successor;
        }
        else if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = successor;
        }
        else {
            nodeToDelete->parent->right = successor;
        }

        successor->parent = nodeToDelete->parent;
        successor->left = nodeToDelete->left;
        if (successor->left != nullptr) {
            successor->left->parent = successor;
        }
        successor->isRed = nodeToDelete->isRed;

        delete nodeToDelete;

        if (!originalColor) {
            if (child != nullptr) {
                fixDelete(root, child);
            }
            else {
                RBNode* dummy = new RBNode(-1);
                dummy->parent = successor;
                dummy->isRed = false;
                fixDelete(root, dummy);
                delete dummy;
            }
        }
    }

    return root;
}

bool searchRB(RBNode* root, int value) {
    RBNode* current = root;
    while (current != nullptr) {
        if (value == current->data) {
            return true;
        }
        else if (value < current->data) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return false;
}

void printRBNode(RBNode* node) {
    if (node == nullptr) return;

    if (node->isRed) {
        cout << node->data << "R ";
    }
    else {
        cout << node->data << "B ";
    }
}

void BFS(RBNode* root) {
    if (root == nullptr) return;

    Queue* queue = createQueue(100);
    enqueue(queue, root);

    while (!isEmpty(queue)) {
        RBNode* current = dequeue(queue);
        printRBNode(current);

        if (current->left != nullptr) {
            enqueue(queue, current->left);
        }
        if (current->right != nullptr) {
            enqueue(queue, current->right);
        }
    }

    delete[] queue->nodes;
    delete queue;
}

void preorder(RBNode* root) {
    if (root == nullptr) return;

    Stack* stack = createStack(100);
    push(stack, root);

    while (!isEmpty(stack)) {
        RBNode* current = pop(stack);
        printRBNode(current);

        if (current->right != nullptr) {
            push(stack, current->right);
        }
        if (current->left != nullptr) {
            push(stack, current->left);
        }
    }

    delete[] stack->nodes;
    delete stack;
}

void inorder(RBNode* root) {
    Stack* stack = createStack(100);
    RBNode* current = root;

    while (current != nullptr || !isEmpty(stack)) {
        while (current != nullptr) {
            push(stack, current);
            current = current->left;
        }

        current = pop(stack);
        printRBNode(current);
        current = current->right;
    }

    delete[] stack->nodes;
    delete stack;
}

void postorder(RBNode* root) {
    if (root == nullptr) return;

    Stack* s1 = createStack(100);
    Stack* s2 = createStack(100);
    push(s1, root);

    while (!isEmpty(s1)) {
        RBNode* current = pop(s1);
        push(s2, current);

        if (current->left != nullptr) {
            push(s1, current->left);
        }
        if (current->right != nullptr) {
            push(s1, current->right);
        }
    }

    while (!isEmpty(s2)) {
        RBNode* current = pop(s2);
        printRBNode(current);
    }

    delete[] s1->nodes;
    delete[] s2->nodes;
    delete s1;
    delete s2;
}

RBNode* createRBFromTree(TreeNode* root) {
    if (root == nullptr) return nullptr;

    RBNode* rbRoot = nullptr;

    Stack* stack = createStack(100);
    TreeNode** treeStack = new TreeNode * [100];
    int treeTop = -1;

    treeStack[++treeTop] = root;

    while (treeTop >= 0) {
        TreeNode* current = treeStack[treeTop--];
        rbRoot = insertRB(rbRoot, current->data);

        if (current->right != nullptr) {
            treeStack[++treeTop] = current->right;
        }
        if (current->left != nullptr) {
            treeStack[++treeTop] = current->left;
        }
    }

    delete[] treeStack;
    delete[] stack->nodes;
    delete stack;

    return rbRoot;
}

void freeTree(TreeNode* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void freeRBTree(RBNode* root) {
    if (root == nullptr) return;
    freeRBTree(root->left);
    freeRBTree(root->right);
    delete root;
}

void displayMenu() {
    cout << "\n1. Display tree traversals" << endl;
    cout << "2. Insert element" << endl;
    cout << "3. Delete element" << endl;
    cout << "4. Search element" << endl;
    cout << "5. Exit" << endl;
    cout << "Choose an option (1-5): ";
}

void displayTraversals(RBNode* root) {
    cout << endl;
    cout << "1) Level-order (BFS):      ";
    BFS(root);
    cout << endl;

    cout << "2) Pre-order traversal:    ";
    preorder(root);
    cout << endl;

    cout << "3) In-order traversal:     ";
    inorder(root);
    cout << endl;

    cout << "4) Post-order traversal:   ";
    postorder(root);
    cout << endl;
}

int main() {
    string input;
    ifstream file("tree.txt");
    if (file.is_open()) {
        getline(file, input);
        file.close();
        cout << "File loaded successfully" << endl;
    }
    else {
        input = "(8 (9 (5)) (1))";
        cout << "File not found, using default example: " << input << endl;
    }

    cout << "Input string: " << input << endl;

    if (!checkBrackets(input)) {
        cout << "Error: Invalid bracket sequence" << endl;
        return 1;
    }

    int pos = 0;
    bool valid = true;
    TreeNode* tree = parseTree(input, pos, valid);

    if (!valid || tree == nullptr) {
        cout << "Parsing error: Invalid binary tree format" << endl;
        return 1;
    }
    cout << "Recursive DFS traversal: ";
    dfs(tree);
    cout << endl;
    RBNode* rbTree = createRBFromTree(tree);

    int choice;
    int value;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            displayTraversals(rbTree);
            break;

        case 2:
            cout << "Enter value to insert: ";
            cin >> value;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number." << endl;
            }
            else {
                rbTree = insertRB(rbTree, value);
                cout << "Tree after insertion - Level order: ";
                BFS(rbTree);
                cout << endl;
            }
            break;

        case 3:
            cout << "Enter value to delete: ";
            cin >> value;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number." << endl;
            }
            else {
                if (searchRB(rbTree, value)) {
                    rbTree = deleteRB(rbTree, value);
                    cout << "Tree after deletion - Level order: ";
                    BFS(rbTree);
                    cout << endl;
                }
                else {
                    cout << "Value " << value << " not found in the tree!" << endl;
                }
            }
            break;

        case 4:
            cout << "Enter value to search: ";
            cin >> value;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number." << endl;
            }
            else {
                bool found = searchRB(rbTree, value);
                cout << "Value " << value << " is " << (found ? "FOUND" : "NOT FOUND") << " in the tree" << endl;
                if (found) {
                    cout << "Current tree - Level order: ";
                    BFS(rbTree);
                    cout << endl;
                }
            }
            break;

        case 5:
            running = false;
            break;

        default:
            cout << "Invalid option! Please choose 1-5." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    return 0;
}