#include <iostream>

using namespace std;

struct RunInfo {
    int start;
    int length;
    RunInfo() : start(0), length(0) {}  // Конструктор по умолчанию
    RunInfo(int s, int len) : start(s), length(len) {}
};

struct Stack {
    int top;
    int stackSize;
    RunInfo* stackArr;
};

Stack* createStack(int size) {
    Stack* stack = new Stack;
    stack->stackSize = size;
    stack->top = -1;
    stack->stackArr = new RunInfo[size];
    return stack;
}

bool isEmpty(Stack* stack) {
    return stack->top == -1;
}

bool isFull(Stack* stack) {
    return stack->top == stack->stackSize - 1;
}

void push(Stack* stack, RunInfo run) {
    if (isFull(stack)) {
        cout << "Stack overflow" << endl;
        return;
    }
    stack->stackArr[++stack->top] = run;
}

RunInfo pop(Stack* stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return RunInfo(-1, -1);
    }
    return stack->stackArr[stack->top--];
}

RunInfo peek(Stack* stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return RunInfo(-1, -1);
    }
    return stack->stackArr[stack->top];
}

int size(Stack* stack) {
    return stack->top + 1;
}

void deleteStack(Stack* stack) {
    delete[] stack->stackArr;
    delete stack;
}

int getMinrun(int n) {
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

struct Run {
    int start;
    int length;
    Run(int s, int len) : start(s), length(len) {}
};

void insertionSort(int arr[], int left, int right)
{
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

// Custom reverse function
void reverseArray(int arr[], int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

int findRunLength(int arr[], int start, int n) {
    if (start >= n - 1) return 1;
    
    int length = 1;
    bool ascending = arr[start] <= arr[start + 1];
    
    for (int i = start + 1; i < n - 1; i++) {
        if ((ascending && arr[i] <= arr[i + 1]) || 
            (!ascending && arr[i] >= arr[i + 1])) {
            length++;
        } else {
            break;
        }
    }
 
    if (!ascending) {
        reverseArray(arr, start, start + length);
    }
    
    return length + 1;
}

int binarySearch(int arr[], int left, int right, int target) {
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

void mergeWithGallop(int arr[], int l, int m, int r) {
    int len1 = m - l + 1, len2 = r - m;
    
    // Use dynamic arrays instead of vectors
    int* left = new int[len1];
    int* right = new int[len2];
    
    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];
    
    int i = 0, j = 0, k = l;
    int gallop_threshold = 7;
    
    while (i < len1 && j < len2) {
        int left_count = 0, right_count = 0;
 
        while (i < len1 && left[i] <= right[j] && left_count < gallop_threshold) {
            arr[k++] = left[i++];
            left_count++;
        }
 
        while (j < len2 && right[j] < left[i] && right_count < gallop_threshold) {
            arr[k++] = right[j++];
            right_count++;
        }
 
        if (left_count >= gallop_threshold) {
            int pos = binarySearch(right, j, len2, left[i]);
            while (j < pos) {
                arr[k++] = right[j++];
            }
        } else if (right_count >= gallop_threshold) {
            int pos = binarySearch(left, i, len1, right[j]);
            while (i < pos) {
                arr[k++] = left[i++];
            }
        }
 
        if (i < len1 && j < len2) {
            if (left[i] <= right[j]) {
                arr[k++] = left[i++];
            } else {
                arr[k++] = right[j++];
            }
        }
    }
    
    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];
    
    // Clean up memory
    delete[] left;
    delete[] right;
}

// Custom min function
int min(int a, int b) {
    return (a < b) ? a : b;
}

void timSort(int arr[], int n) {
    if (n < 2) return;
    
    int minrun = getMinrun(n);
    Stack* runs = createStack(n);
    
    int i = 0;
    while (i < n) {
        int runLength = findRunLength(arr, i, n);
 
        if (runLength < minrun) {
            int remaining = min(minrun, n - i);
            insertionSort(arr, i, i + remaining - 1);
            runLength = remaining;
        }
 
        push(runs, RunInfo(i, runLength));
 
        while (size(runs) >= 2) {
            RunInfo r = pop(runs);
            RunInfo l = pop(runs);
 
            if (size(runs) > 0) {
                RunInfo z = peek(runs);
                if (z.length > l.length + r.length) {
                    push(runs, l);
                    push(runs, r);
                    break;
                }
            }
 
            if (l.length > r.length) {
                push(runs, l);
                push(runs, r);
                break;
            }
 
            mergeWithGallop(arr, l.start, l.start + l.length - 1, 
                           l.start + l.length + r.length - 1);
            push(runs, RunInfo(l.start, l.length + r.length));
        }
        
        i += runLength;
    }
 
    while (size(runs) > 1) {
        RunInfo r = pop(runs);
        RunInfo l = pop(runs);
        
        mergeWithGallop(arr, l.start, l.start + l.length - 1, 
                       l.start + l.length + r.length - 1);
        push(runs, RunInfo(l.start, l.length + r.length));
    }
    
    deleteStack(runs);
}

void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << "  ";
    cout << endl;
}

int* inputArray(int& n) {
    cout << "Enter the size of array: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "ERROR: Invalid array size" << endl;
        return nullptr;
    }
    
    int* arr = new int[n];
    cout << "Enter " << n << " elements(enter through a spacebar): ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    return arr;
}

int main()
{

    int* arr;
    int n;

    arr = inputArray(n);
    if (arr == nullptr) {
        return 1;
    }
    
    cout << "\nOriginal array:" << endl;
    printArray(arr, n);

    int minrun = getMinrun(n);
    cout << "Calculated minrun = " << minrun << endl;

    timSort(arr, n);
    
    cout << "\nSorting result:" << endl;
    printArray(arr, n);
    
    delete[] arr;
    
    return 0;
}