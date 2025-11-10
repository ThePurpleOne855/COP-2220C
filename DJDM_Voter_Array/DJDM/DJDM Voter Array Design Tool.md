
Step by Step Design:

1. Create a binary file that contains the total size of the array and the current number of spaces used in the array.
2. Create a Name struct with the following fields: First name with 50 characters limit, Middle Initial with 1 character limit, last name with 70 characters limit. Make sure the first letter of the name, middle name, last name is capitalize. and if there is a middle name it must have a period at the end.
3. Create VOTERS struct.
4. VOTERS struct must have the name struct has one of its fields.
5. VOTERS struct must have gender has one of its field. If the gender is not M, F, O then it has to be set to U(undefined I guess)
6. VOTERS struct must have voters favorite number as one of its fields.
7. VOTERS struct must have voters weight in float as one of its fields.
8. VOTERS struct must have a matches fields
9. The Computer Weight Statement / Category (Fat, Fit, Skinny) when outputting the values in the list.  ( Skinny Males < 125, Skinny Females  < 100 pounds, O or U skinny weight < 115) (Fit Male  weight >= 125 and weight <= 225, Fit Female  weight >= 100 and and weight <= 150) (O or U weight weight 115 >= 115 and weight <= 200 pounds) (Fat Male weight >  225) (Fat Females weight > 150) (O or U fat weight > 200)
10. Create an array for each field in the list.
11. Create a simulation array with 500 spaces of 3 digits numbers from 000 to 999.
12. Calculate the Voter weight classication at run time and print it with the voter records
13. Run this simulation and compare to each of the voters favorite numbers and input the number of matches in the matches field.
14. Give the user a way to stop the program.




--- 


## **Step 1: Define the Data Structures**

1. **NAME struct**
    
    - Fields:
        
        - `firstName[50]` → string
            
        - `middleInitial[1]` → char (uppercase with period if exists)
            
        - `lastName[70]` → string
            
2. **VOTER struct**
    
    - Fields:
        
        - `name` → NAME struct
            
        - `gender` → char (M, F, O, or U)
            
        - `favoriteNumber` → int (0–999)
            
        - `weight` → float (10.0–999.0)
            
        - `timesWon` → int (to store how many times lottery number matched)
            
3. **Disjoint Dynamic Array**
    
    - Dynamically allocated array of `VOTER` structs
        
    - Array size and effective size tracked separately
        

---

## **Step 2: Read Binary File**

1. Open `input.bin` from the SRC folder in binary mode.
    
2. Read first 4 bytes → total size of array (number of allocated elements).
    
3. Read next 4 bytes → effective size (number of elements with data).
    
4. Allocate memory for array based on total size.
    
5. Loop through effective size and read each `VOTER` record:
    
    - Read `NAME` fields (first, middle, last)
        
    - Read `gender`
        
    - Read `favoriteNumber`
        
    - Read `weight`
        

---

## **Step 3: Input Additional Records**

1. Ask the user if they want to enter more records.
    
2. Continue adding records until:
    
    - Array is full (`effective size == total size`)
        
    - Or user chooses to stop.
        
3. Update the effective size as new records are added.
    
4. For each new record:
    
    - Validate gender (convert invalid input to `U`)
        
    - Correct names to sentence case
        

---

## **Step 4: Name and Gender Correction**

1. **Sentence case for names**:
    
    - First letter uppercase, remaining letters lowercase
        
    - Middle initial uppercase + period if provided
        
2. **Gender correction**:
    
    - If `gender` not M, F, or O → set to U
        

---

## **Step 5: Simulate Lottery**

1. Generate 500 random 3-digit numbers (000–999).
    
2. For each voter:
    
    - Check if their `favoriteNumber` matches any generated number
        
    - Count matches → store in `timesWon` field
        

---

## **Step 6: Categorize Weight**

1. For each voter, determine **weight category**:
    
    - **SKINNY**:
        
        - Males: <125
            
        - Females: <100
            
        - O/U: <115
            
    - **FIT**:
        
        - Males: 125–225
            
        - Females: 100–150
            
        - O/U: 115–200
            
    - **FAT**:
        
        - Males: >225
            
        - Females: >150
            
        - O/U: >200
            

---

## **Step 7: Output Voter Information**

1. Traverse the array and display each voter's info:
    
    - Name
        
    - Gender
        
    - Favorite Number
        
    - Times Won
        
    - Weight
        
    - Weight Statement (SKINNY/FIT/FAT)
        
2. Format output as a neat table
    

---

## **Step 8: Export to Binary File**

1. Open `input.bin` for writing in binary mode.
    
2. Write **total size** (first 4 bytes)
    
3. Write **effective size** (next 4 bytes)
    
4. Loop through effective size and write each voter record:
    
    - Name
        
    - Gender
        
    - Favorite Number
        
    - Weight
        

---

## **Step 9: Design Considerations**

1. **Memory management**: Use dynamic allocation carefully and free memory at the end.
    
2. **Input validation**: Ensure all inputs are valid (gender, numbers, weights).
    
3. **Random number generation**: Use seed (`time(NULL)`) to vary lottery draws.
    
4. **Data encapsulation**: Consider separate functions for:
    
    - Reading/writing binary file
        
    - Correcting names/gender
        
    - Simulating lottery
        
    - Categorizing weight
        
    - Displaying voter info
        

---

## **Step 10: File/Module Structure**

1. `voter.h` → contains `NAME` and `VOTER` structs, constants, and function prototypes
    
2. `voter.c` → contains functions for reading/writing files, correcting names/gender, simulating lottery, weight categorization, output
    
3. `main.c` → handles array creation, user input, calls functions in correct sequence, and frees memory