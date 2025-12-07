#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

//------------------------------------
// NOTIFICATION STRUCTURE
//------------------------------------
typedef struct {
    int priority;            // Higher priority = more urgent
    char message[150];
    char category[30];       // SMS / App / System
    char timestamp[40];
} Notification;

//------------------------------------
// GLOBALS
//------------------------------------
Notification queue[MAX];   // Normal Queue
Notification heap[MAX];    // Priority Queue (Max-Heap)
Notification stack[MAX];   // Dismissed Stack

int front = -1, rear = -1;
int heapSize = 0;
int top = -1;

//------------------------------------
// UTILITY: GENERATE CURRENT TIME
//------------------------------------
void getCurrentTime(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 40, "%Y-%m-%d %H:%M:%S", t);
}

//------------------------------------
// QUEUE FUNCTIONS
//------------------------------------
int isQueueEmpty() { return front == -1; }
int isQueueFull()  { return rear == MAX - 1; }

void enqueue(Notification n) {
    if (isQueueFull()) {
        printf("\n❌ Normal Queue is FULL!\n");
        return;
    }
    if (front == -1) front = 0;
    queue[++rear] = n;
    printf("\n📩 Normal notification added.\n");
}

Notification dequeue() {
    Notification n = {-1, "", "", ""};
    if (isQueueEmpty()) {
        printf("\n⚠ No normal notifications.\n");
        return n;
    }
    n = queue[front];
    if (front == rear)
        front = rear = -1;
    else
        front++;
    return n;
}

//------------------------------------
// STACK FUNCTIONS
//------------------------------------
int isStackFull() { return top == MAX - 1; }
int isStackEmpty() { return top == -1; }

void push(Notification n) {
    if (isStackFull()) {
        printf("\n❌ Dismiss Stack FULL!\n");
        return;
    }
    stack[++top] = n;
    printf("\n🗑 Notification dismissed.\n");
}

Notification pop() {
    Notification n = {-1, "", "", ""};
    if (isStackEmpty()) {
        printf("\n⚠ No dismissed notifications.\n");
        return n;
    }
    return stack[top--];
}

//------------------------------------
// PRIORITY QUEUE (MAX-HEAP)
//------------------------------------
void swap(Notification *a, Notification *b) {
    Notification t = *a;
    *a = *b;
    *b = t;
}

void heapInsert(Notification n) {
    heap[++heapSize] = n;
    int i = heapSize;

    while (i > 1 && heap[i].priority > heap[i/2].priority) {
        swap(&heap[i], &heap[i/2]);
        i = i/2;
    }
    printf("\n🔥 High-priority notification added.\n");
}

Notification heapDelete() {
    Notification n = {-1, "", "", ""};

    if (heapSize == 0) {
        printf("\n⚠ No high priority notifications.\n");
        return n;
    }

    n = heap[1];
    heap[1] = heap[heapSize--];

    int i = 1;
    while (1) {
        int left = 2*i, right = 2*i + 1, largest = i;

        if (left <= heapSize && heap[left].priority > heap[largest].priority)
            largest = left;
        if (right <= heapSize && heap[right].priority > heap[largest].priority)
            largest = right;

        if (largest != i) {
            swap(&heap[i], &heap[largest]);
            i = largest;
        } else break;
    }

    return n;
}

//------------------------------------
// INPUT HELPER
//------------------------------------
void safeInput(char *str, int size) {
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = '\0';
}

//------------------------------------
// DISPLAY FUNCTION
//------------------------------------
void printNotification(Notification n) {
    if (n.priority == -1) return;
    printf("\n----------------------------------\n");
    printf("📌 MESSAGE   : %s\n", n.message);
    printf("📂 CATEGORY  : %s\n", n.category);
    printf("⭐ PRIORITY  : %d\n", n.priority);
    printf("⏰ TIME      : %s\n", n.timestamp);
    printf("----------------------------------\n");
}

//------------------------------------
// MAIN MENU
//------------------------------------
int main() {
    int choice;
    Notification n;

    while (1) {
        printf("\n============= NOTIFICATION MANAGER =============\n");
        printf("1. Add Normal Notification\n");
        printf("2. Add High-Priority Notification\n");
        printf("3. View Next Notification\n");
        printf("4. Dismiss a Notification\n");
        printf("5. Undo Last Dismiss\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {

        case 1:
            n.priority = 1;
            getCurrentTime(n.timestamp);

            printf("\nEnter Message: ");
            safeInput(n.message, 150);

            printf("Enter Category (SMS/App/System): ");
            safeInput(n.category, 30);

            enqueue(n);
            break;

        case 2:
            printf("\nEnter priority (2–10): ");
            scanf("%d", &n.priority);
            getchar();
            getCurrentTime(n.timestamp);

            printf("Enter Message: ");
            safeInput(n.message, 150);

            printf("Enter Category: ");
            safeInput(n.category, 30);

            heapInsert(n);
            break;

        case 3:
            if (heapSize > 0) {
                printf("\n🔥 HIGH PRIORITY NOTIFICATION:\n");
                n = heapDelete();
                printNotification(n);
            }
            else if (!isQueueEmpty()) {
                printf("\n📩 NORMAL NOTIFICATION:\n");
                n = dequeue();
                printNotification(n);
            }
            else {
                printf("\n⚠ No notifications available.\n");
            }
            break;

        case 4:
            printf("\nEnter message to dismiss: ");
            safeInput(n.message, 150);
            n.priority = 0;
            getCurrentTime(n.timestamp);
            strcpy(n.category, "Dismissed");

            push(n);
            break;

        case 5:
            n = pop();
            if (n.priority != -1) {
                printf("\n⤺ Undo successful! Restored notification:\n");
                printNotification(n);
            }
            break;

        case 6:
            printf("\nExiting Notification Manager...\n");
            exit(0);   // ✅ FIXED
            break;

        default:
            printf("\n❌ Invalid choice.\n");
        }
    }
}
