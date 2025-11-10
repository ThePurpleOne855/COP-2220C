#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char name[100];
    int age;
    float weight;
    struct Person *next;
} Person;

// Add a new person in alphabetical order
Person* add(Person *head) {
    Person *p = malloc(sizeof(Person));
    if (!p) return head;

    printf("Name: ");
    fgets(p->name, sizeof(p->name), stdin);
    p->name[strcspn(p->name, "\n")] = '\0'; // remove newline

    printf("Age: "); scanf("%d", &p->age);
    printf("Weight: "); scanf("%f", &p->weight);
    getchar(); // clear buffer

    p->next = NULL;
    if (!head || strcmp(p->name, head->name) < 0) { // insert at start
        p->next = head;
        return p;
    }

    Person *cur = head;
    while (cur->next && strcmp(cur->next->name, p->name) < 0) // find position
        cur = cur->next;

    p->next = cur->next; // insert in list
    cur->next = p;
    return head;
}

// Display all records
void show(Person *head) {
    if (!head) { printf("No records.\n"); return; }
    for (Person *cur = head; cur; cur = cur->next)
        printf("%s | %d | %.2f\n", cur->name, cur->age, cur->weight);
}

// Free all memory
void freeList(Person *head) {
    while (head) {
        Person *tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main() {
    Person *head = NULL;
    int choice;

    // Menu loop
    do {
        printf("1.Add 2.Show 3.Quit: ");
        scanf("%d", &choice); getchar();
        if (choice == 1) head = add(head);
        else if (choice == 2) show(head);
    } while (choice != 3);

    freeList(head); // clean up memory
}
