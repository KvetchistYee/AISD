#include <iostream>
#include <string>

using namespace std;

struct List
{
    int data;
    List* prev;
    List* next;
};

List* createList(int length)
{
    List* curr = 0,
        * nextItem = 0;
    for (int i = 1; i <= length; ++i)
    {
        curr = new List;
        curr->next = nextItem;
        if (nextItem)
            nextItem->prev = curr;
        nextItem = curr;
    }
    curr->prev = 0;
    return curr;
}

void deleteList(List* beg) {
    List* next;
    while (beg) {
        next = beg->prev;
        delete beg;
        beg = next;
    }
}

List* listItem(List* beg, int index, bool errMsg = true)
{
    while (beg && (index--))
        beg = beg->next;
    if (errMsg && !beg)
        cout << "There is no element \n";
    return beg;
}

unsigned lengthList(List* beg)
{
    int length = 0;
    while (beg)
    {
        ++length;
        beg = beg->next;
    }
    return length;
}

void delItem(List*& beg, int index)
{
    if (index >= lengthList(beg)) {
        return;
    }

    List* item;
    if (!index) {
        item = beg->next;
        delete beg;
        beg = item;
        beg->prev = nullptr;
        return;
    }
    item = listItem(beg, index - 1, 0);
    List* dItem = item->next;
    item->next = dItem->next;
    item->next->prev = item;
    delete dItem;
}

List* insItem(List*& beg, int index, int value)
{
    List* item = new List;
    item->data = value;
    if (!index || !beg) {
        beg->prev = item;
        item->prev = nullptr;
        item->next = beg;
        beg = item;
        return item;
    }
    List* predItem = beg;
    --index;
    while (predItem->next && (index--)) {
        predItem = predItem->next;
    }
    item->prev = predItem;
    item->next = predItem->next;

    if (predItem->next) {
        predItem->next->prev = item;
    }
    predItem->next = item;
    return item;
}

void printList(List* beg) {
    cout << endl << "Created list: ";
    while (beg) {
        cout << beg->data << " ";
        beg = beg->next;
    }
    cout << endl;
}

struct Stack {
    int top;
    int stackSize;
    int* stackArr;
};

Stack* createStack(int size) {
    Stack* stack = new Stack;
    stack->stackSize = size;
    stack->top = -1;
    stack->stackArr = new int[size];
    return stack;
}

bool isEmpty(Stack* stack) {
    return stack->top == -1;
}

bool isFull(Stack* stack) {
    return stack->top == stack->stackSize - 1;
}

void push(Stack* stack, int value) {
    if (isFull(stack)) {
        cout << "Stack overflow" << endl;
        return;
    }
    stack->stackArr[++stack->top] = value;
}

int pop(Stack* stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return -1;
    }
    return stack->stackArr[stack->top--];
}

int peek(Stack* stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return -1;
    }
    return stack->stackArr[stack->top];
}

void deleteStack(Stack* stack) {
    delete[] stack->stackArr;
    delete stack;
}

int* createDynArr(int size) {
    int* dinamArr = nullptr;
    return dinamArr;
}

void printDynArray(int* arr, int size) {
    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int* addElement(int* dynArr, int size, int item) {
    int* newArr = new int[size + 1];
    for (int i = 0; i < size; i++) {
        newArr[i] = dynArr[i];
    }
    newArr[size] = item;
    delete[] dynArr;
    return newArr;
}

int* removeElement(int* dynArr, int size, int index) {
    if (index < 0 || index >= size) {
        cout << "Invalid index" << endl;
        return dynArr;
    }
    int* newArr = new int[size - 1];
    int newIndex = 0;
    for (int i = 0; i < size; i++) {
        if (i != index) {
            newArr[newIndex] = dynArr[i];
            newIndex++;
        }
    }
    delete[] dynArr;
    return newArr;
}

void deleteArray(int* arr) {
    delete[] arr;
}

int priority(char op) {
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    case 's':
    case 'c':
        return 4;
    default:
        return -1;
    }
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == 's' || c == 'c';
}

bool isFunction(const string& infix, size_t i) {
    if (i + 2 < infix.size()) {
        string substr = infix.substr(i, 3);
        return substr == "sin" || substr == "cos";
    }
    return false;
}

string toPostfix(const string& infix) {
    Stack* s = createStack(99);
    string postfix;

    for (size_t i = 0; i < infix.size(); ++i) {
        char c = infix[i];

        if (isspace(c))
            continue;

        if (isFunction(infix, i)) {
            string func = infix.substr(i, 3);
            if (func == "sin") {
                push(s, 's');
                i += 2;
                continue;
            }
            else if (func == "cos") {
                push(s, 'c');
                i += 2;
                continue;
            }
        }

        if (isdigit(c)) {
            postfix += c;
            while (i + 1 < infix.size() && isdigit(infix[i + 1]))
                postfix += infix[++i];
            postfix += ' ';
        }
        else if (c == '(')
            push(s, c);
        else if (c == ')') {
            while (!isEmpty(s) && peek(s) != '(') {
                postfix += peek(s);
                postfix += ' ';
                pop(s);
            }
            if (!isEmpty(s)) {
                pop(s);
                if (!isEmpty(s) && (peek(s) == 's' || peek(s) == 'c')) {
                    postfix += peek(s);
                    postfix += ' ';
                    pop(s);
                }
            }
            else
                return "Invalid input";
        }
        else if (priority(c) > 0) {
            if (c == '^') {
                while (!isEmpty(s) && priority(peek(s)) > priority(c)) {
                    postfix += peek(s);
                    postfix += ' ';
                    pop(s);
                }
            }
            else {
                while (!isEmpty(s) && priority(peek(s)) >= priority(c)) {
                    postfix += peek(s);
                    postfix += ' ';
                    pop(s);
                }
            }
            push(s, c);
        }
        else {
            return "Invalid input";
        }
    }
    while (!isEmpty(s)) {
        if (peek(s) == '(' || peek(s) == ')')
            return "Invalid input";
        postfix += peek(s);
        postfix += ' ';
        pop(s);
    }

    return postfix;
}

int main()
{
    string expression;
    cout << "Enter expression: ";
    getline(cin, expression);
    string result = toPostfix(expression);
    cout << "Sorting station : " << result << endl;
}
