#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[100];
    char sku[16];
    double price;
    int quantity;
} Item;

typedef struct {
    Item *items;
    size_t size;
    size_t capacity;
} Inventory;

// Function declaration
Inventory *create_inventory(size_t initial_capacity);
void free_inventory(Inventory *inv);
int add_item(Inventory *inv, char *sku, char *name, double price, int qty);
int remove_item_by_sku(Inventory *inv, char *sku);
void sort_by_name(Inventory *inv);
void sort_by_price(Inventory *inv);
void print_item(const Item *item);
void search_and_print(Inventory *inv, char *query);
double total_inventory_value(Inventory *inv);
double average_price(Inventory *inv);
void print_inventory(Inventory *inv);
int save_inventory(const Inventory *inv, const char *filename);
void load_inventory(Inventory *inv, const char *filename);

int main() {
    Inventory *inv = create_inventory(2);
    if (inv == NULL) {
        printf("Failed to create inventory.\n");
        return 1;
    }

    int choice;
    char sku[16], name[100];
    double price;
    int qty;

    while (1) {
        printf("\n==== INVENTORY MENU ====\n");
        printf("1. Add item\n");
        printf("2. Remove item by SKU\n");
        printf("3. Search and print item\n");
        printf("4. Print inventory\n");
        printf("5. Sort by name\n");
        printf("6. Sort by price\n");
        printf("7. Save inventory\n");
        printf("8. Load inventory\n");
        printf("9. Total inventory value\n");
        printf("10. Average price\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("Enter SKU: ");
                scanf("%15s", sku);
                printf("Enter name: ");
                getchar();
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter price: ");
                scanf("%lf", &price);
                printf("Enter quantity: ");
                scanf("%d", &qty);

                add_item(inv, sku, name, price, qty);
                break;

            case 2:
                printf("Enter SKU to remove: ");
                scanf("%15s", sku);
                remove_item_by_sku(inv, sku);
                break;

            case 3:
                printf("Enter name or SKU to search: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                search_and_print(inv, name);
                break;

            case 4:
                print_inventory(inv);
                break;

            case 5:
                sort_by_name(inv);
                break;

            case 6:
                sort_by_price(inv);
                break;

            case 7:
                save_inventory(inv, "inventory.bin");
                printf("Saved.\n");
                break;

            case 8:
                load_inventory(inv, "inventory.bin");
                printf("Loaded.\n");
                break;

            case 9:
                printf("Total value: %.2f\n", total_inventory_value(inv));
                break;

            case 10:
                printf("Average price: %.2f\n", average_price(inv));
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    free_inventory(inv);
    return 0;
}

// Creates a new inventory with a dynamic array capable of holding initial capacity
Inventory *create_inventory(size_t initial_capacity)
{
    Inventory *inv = malloc(sizeof(Inventory)); // Allocate memory for the inventory array
    if(inv == NULL){
        return NULL; // If it fails
    }

    inv->items = malloc(initial_capacity * sizeof(Item)); // Allocate memory for items
    if(inv->items == NULL) {
        free(inv); // If Allocation of the item into the inventory array fails then free the inventory array
        return NULL;
    }

    // Initialize size and capacity
    inv->size = 0;
    inv->capacity = initial_capacity;

    return inv;
}

// Releases all memory used by the inventory, including the dynamic array
void free_inventory(Inventory *inv)
{
    free(inv->items);
    free(inv);
}

// Adds a new item to the inventory, if the internal array is full, it should expand it
int add_item(Inventory *inv, char *sku, char *name, double price, int qty)
{
    if (inv->size == inv->capacity) {
        size_t new_capacity = inv->capacity * 2;
        Item *new_items = realloc(inv->items, new_capacity * sizeof(Item));
        if(new_items == NULL){
            return -1;
        }
        inv->items = new_items;
        inv->capacity = new_capacity;
    }

    strcpy(inv->items[inv->size].sku, sku);
    strcpy(inv->items[inv->size].name, name); // Fixed: store name instead of SKU
    inv->items[inv->size].price = price;
    inv->items[inv->size].quantity = qty;

    inv->size++; // Increment the size field that we are using to index upon the inventory array so next time we add a new item it will be added to the next slot
    return 0;
}

// Searches for an item by SKU and removes it. Shifts remaining items left to close the gap
int remove_item_by_sku(Inventory *inv, char *sku)
{
    for(size_t i = 0; i < inv->size; i++){
        if(strcmp(inv->items[i].sku, sku) == 0){
            for(size_t j = i; j < inv->size - 1; j++){
                inv->items[j] = inv->items[j + 1];
            }
            inv->size--;
            return 0;
        }
    }
    return -1;
}

// Searches the inventory for an item with the given SKU

// Sorts all items alphabetically by their name
void sort_by_name(Inventory *inv)
{
    for(size_t i = 0; i < inv->size - 1; i++){ // Simple bubble sort
        for(size_t j = 0; j < inv->size - i - 1; j++){
            if(strcmp(inv->items[j].name, inv->items[j + 1].name) > 0){
                Item temp = inv->items[j];
                inv->items[j] = inv->items[j + 1];
                inv->items[j + 1] = temp;
            }
        }
    }
}

// Sorts all items in ascending order by their price
void sort_by_price(Inventory *inv)
{
    for (size_t i = 0; i < inv->size - 1; i++){ // Same algorithm as in the sort by name function
        for(size_t j = 0; j < inv->size - i -1; j++){
            if(inv->items[j].price > inv->items[j + 1].price){
                Item temp = inv->items[j];
                inv->items[j] = inv->items[j + 1];
                inv->items[j + 1] = temp;
            }
        }
    }
}

// Prints a single item
void print_item(const Item *item)
{
    printf("SKU: %s\n", item->sku);
    printf("Name: %s\n", item->name);
    printf("Price: $%.2f\n", item->price);
    printf("Quantity: %d\n", item->quantity);
}

// Searches for items whose name or SKU contains the given query (partial match allowed) prints matching items
void search_and_print(Inventory *inv, char *query)
{
    for(size_t i = 0; i < inv->size; i++){
        if (strstr(inv->items[i].sku, query) ||
            strstr(inv->items[i].name, query)) {

            printf("Item:\n");
            print_item(&inv->items[i]);
            return;
        }
    }
    printf("Not Found.\n");
}

// Calculates the total value of all inventory items
double total_inventory_value(Inventory *inv)
{
    double total = 0.0;
    for(size_t i = 0; i < inv->size; i++){
        total += inv->items[i].price * inv->items[i].quantity;
    }
    return total;
}

// Calculates the average price of all items
double average_price(Inventory *inv)
{
    if(inv->size == 0){
        return 0.0;
    }
    double sum = 0.0;

    for(size_t i = 0; i < inv->size; i++){
        sum += inv->items[i].price;
    }
    return sum / inv->size;
}

// Prints all items in a clean table format: SKU, name, price, quantity, etc
void print_inventory(Inventory *inv)
{
    if(inv->size == 0){
        printf("Inventory is empty.\n");
        return;
    }

    printf("Inventory contains:\n");
    for(size_t i = 0; i < inv->size; i++){
        printf("Item %zu:\n", i + 1);
        print_item(&inv->items[i]);
        printf("\n");
    }
}

// Writes the entire inventory to a binary file
int save_inventory(const Inventory *inv, const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL) return -1;

    // Write capacity first
    if(fwrite(&inv->capacity, sizeof(size_t), 1, fp) != 1){
        fclose(fp);
        return -1;
    }

    // Write size
    if(fwrite(&inv->size, sizeof(size_t), 1, fp) != 1){
        fclose(fp);
        return -1;
    }

    // Write items
    if(fwrite(inv->items, sizeof(Item), inv->size, fp) != inv->size){
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}


// Reads inventory data from a file and loads it into the program. Replaces existing inventory contents
void load_inventory(Inventory *inv, const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    fread(&inv->capacity, sizeof(size_t), 1, fp);
    fread(&inv->size, sizeof(size_t), 1, fp);

    inv->items = malloc(inv->capacity * sizeof(Item));
    if (inv->items == NULL) {
        printf("Memory allocation failed.\n");
        fclose(fp);
        return;
    }

    fread(inv->items, sizeof(Item), inv->size, fp);

    fclose(fp);
}
