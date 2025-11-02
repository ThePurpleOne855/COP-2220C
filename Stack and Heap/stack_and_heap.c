#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    char name[36];
    char lastname[30];
    int age;
} Person;


int main(){
    Person arr[1000] = {{"Jose", "Santana", 19},
                    {"Jayson", "Santana", 16},
                    {"Liliam", "Valdez", 60},
                    {"Jose", "Santana", 19},
                    {"Jose", "Santana", 19},
                    {"Jose", "Santana", 19}, 
                    {"Jose", "Santana", 19},
                    {"Jose", "Santana", 19},
                    {"Jose", "Santana", 19},
                    {"Jose", "Santana", 19}};

    int memory_used = 0;

    for(int  i = 0; i < 1000; i++){
        if (arr[i].name[0] != '\0') memory_used++;
    }

    // Ask user how many elements to copy if the user does not input anything it will be set to 1 automatically
    int userCount;
    printf("Enter number of elements to store on the heap (max %d): ", memory_used);
    scanf("%d", &userCount);
    if (userCount > memory_used) userCount = memory_used;
    if (userCount < 1) userCount = 1;

    Person *arrHeapPtr = (Person *) malloc(userCount * sizeof(Person));
    if (arrHeapPtr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    memcpy(arrHeapPtr, arr, userCount * sizeof(Person)); // Copies the stack array to the heap array

    
    printf("Stack Array:\n\n");
    for(int y = 0; y < memory_used; y++){
        printf("Row #%d: ", y + 1);
        printf("%s, %s, %d\n", arr[y].name, arr[y].lastname, arr[y].age);
    }
    printf("\n\n");

    printf("Heap Array:\n\n");
    for(int y = 0; y < userCount; y++){
        printf("Row #%d: ", y + 1);
        printf("%s, %s, %d\n", arrHeapPtr[y].name, arrHeapPtr[y].lastname, arrHeapPtr[y].age);
    }

    // Writes to a BIN file
    FILE *fp = fopen("people.bin", "wb");
    if (fp != NULL) {
        fwrite(arrHeapPtr, sizeof(Person), userCount, fp);
        fclose(fp);
        printf("\nData successfully written to people.bin\n");
    } else {
        printf("\nError opening file for writing.\n");
    }

    free(arrHeapPtr);
}
