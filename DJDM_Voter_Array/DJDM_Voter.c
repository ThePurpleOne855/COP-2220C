#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// There is a 5 record limits, If you hit the limit just delete the input.bin file


typedef struct {
    char firstName[50];
    char middleInitial;
    char lastName[70];
} NAME;

typedef struct {
    NAME name;
    char gender; // M, F, O, or U for undefined
    int favoriteNumber;
    float weight;
} VOTER;

// Function to convert string to sentence case (First letter uppercase, rest lowercase)
void toSentenceCase(char *str) {
    if (str == NULL || str[0] == '\0') {
        return;
    }
    
    // Capitalize first letter
    str[0] = toupper(str[0]);
    
    // Lowercase the rest
    for (int i = 1; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to classify weight based on gender
const char* classifyWeight(char gender, float weight) {
    gender = toupper(gender);
    
    if (gender == 'M') {
        // Males
        if (weight < 125) {
            return "SKINNY";
        } else if (weight >= 125 && weight <= 225) {
            return "FIT";
        } else {
            return "FAT";
        }
    } else if (gender == 'F') {
        // Females
        if (weight < 100) {
            return "SKINNY";
        } else if (weight >= 100 && weight <= 150) {
            return "FIT";
        } else {
            return "FAT";
        }
    } else {
        // O or U (Other/Undefined)
        if (weight < 115) {
            return "SKINNY";
        } else if (weight >= 115 && weight <= 200) {
            return "FIT";
        } else {
            return "FAT";
        }
    }
}

// Write data to binary file -- Save the record from previous run
int writeToBinaryFile(int *programArraySize, int *programUsedArraySize, VOTER *voters) {
    FILE *fp = fopen("src/input.bin", "wb");
    if (!fp) {
        perror("Error opening");
        return 1;
    }
    
    fwrite(programArraySize, sizeof(int), 1, fp);
    fwrite(programUsedArraySize, sizeof(int), 1, fp);
    fwrite(voters, sizeof(VOTER), *programUsedArraySize, fp);
    fclose(fp);
    return 0;
}

// Read data from binary file
int readBinaryFile(int *programArraySize, int *programUsedArraySize, VOTER *voters) {
    FILE *fp = fopen("src/input.bin", "rb");
    if (!fp) {
        return 1; // file not found
    }
    
    size_t read1 = fread(programArraySize, sizeof(int), 1, fp);
    size_t read2 = fread(programUsedArraySize, sizeof(int), 1, fp);
    
    // Check if reads were successful
    if (read1 != 1 || read2 != 1) {
        fclose(fp);
        return 1; // read failed, treat as if file doesn't exist
    }
    
    if (*programUsedArraySize > 0) {
        fread(voters, sizeof(VOTER), *programUsedArraySize, fp);
    }
    
    fclose(fp);
    return 0;
}

int main() {
    int arraySizeMain = 0;
    int usedSizeMain = 0;
    VOTER votersMain[5];
    char selection;
    
    int numbers_lottery[500];
    
    // Seed the random number generator with current time
    srand(time(NULL));
    
    // Generate 500 random numbers from 0 to 999
    for (int i = 0; i < 500; i++) {
        numbers_lottery[i] = rand() % 1000;  // 0 to 999
    }
    
    // Print the numbers with leading zeros
    printf("Generated 500 random numbers (000-999):\n");
    for (int i = 0; i < 500; i++) {
        printf("%03d ", numbers_lottery[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }

    // reading existing data, if there is not a file it will create it.
    if (readBinaryFile(&arraySizeMain, &usedSizeMain, votersMain) != 0) {
        printf("No file found or empty file, creating a new one...\n");
        arraySizeMain = 5;
        usedSizeMain = 0;
        // Create empty file for the first time
        writeToBinaryFile(&arraySizeMain, &usedSizeMain, votersMain);
    }
    
    // Display existing records
    printf("\n=== Current State ===\n");
    printf("Array Total Size: %d, Used Size: %d\n\n", arraySizeMain, usedSizeMain);
    
    if (usedSizeMain > 0) {
        printf("Existing records:\n");
        for (int i = 0; i < usedSizeMain; i++) {
            int matches = 0;
            for(int y = 0; y < 500; y++){
                if(votersMain[i].favoriteNumber == numbers_lottery[y]){
                    matches++;
                }
            }
            
            // Get weight classification
            const char* weightClass = classifyWeight(votersMain[i].gender, votersMain[i].weight);
            
            printf("[%d] Name: %s %c. %s, Gender: %c, Favorite Number: %03d, Weight: %.2f, Classification: %s, Matches: %d\n",
                   i,
                   votersMain[i].name.firstName,
                   votersMain[i].name.middleInitial,
                   votersMain[i].name.lastName,
                   votersMain[i].gender,
                   votersMain[i].favoriteNumber,
                   votersMain[i].weight,
                   weightClass,
                   matches);
        }
    } else {
        printf("No existing records.\n");
    }
    
    // Add new record
    printf("\nDo you want to add a new record? y/n: ");
    scanf(" %c", &selection);
    
    if (selection == 'y' || selection == 'Y') {
        if (usedSizeMain >= arraySizeMain) {
            printf("Array is full! Cannot add more records.\n");
            return 0;
        }
        
        VOTER newRecord;
        printf("First Name: ");
        scanf("%49s", newRecord.name.firstName);
        toSentenceCase(newRecord.name.firstName);
        
        printf("Middle Initial: ");
        scanf(" %c", &newRecord.name.middleInitial);
        newRecord.name.middleInitial = toupper(newRecord.name.middleInitial);
        
        printf("Last Name: ");
        scanf("%69s", newRecord.name.lastName);
        toSentenceCase(newRecord.name.lastName);
        
        printf("Gender (M/F/O/U): ");
        scanf(" %c", &newRecord.gender);
        newRecord.gender = toupper(newRecord.gender);
        
        printf("Favorite Number (000-999): ");
        scanf("%d", &newRecord.favoriteNumber);
        
        if (newRecord.favoriteNumber > 999)
            newRecord.favoriteNumber = 999;
        if (newRecord.favoriteNumber < 0)
            newRecord.favoriteNumber = 0;
        
        printf("Weight: ");
        scanf("%f", &newRecord.weight);
        
        // Add new record
        votersMain[usedSizeMain] = newRecord;
        usedSizeMain++;
        
        // Save back to file
        writeToBinaryFile(&arraySizeMain, &usedSizeMain, votersMain);
        printf("\n✅ Record saved successfully!\n");
    }
    
    return 0;
}