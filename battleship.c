#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define GRID_SIZE 10
#define MAX_SHIPS 5
#define MAX_COORDINATES 5
#define MAX_SCORES 10
#define SAVE_FILE "savegame.txt"
#define SCORES_FILE "topTenScores.txt"

// Pair structure for coordinates
typedef struct {
    int x;
    int y;
} Pair;

// Ship structure
typedef struct {
    char name[36];
    char symbol;
    int size;
    char orientation[2];
    Pair coords[MAX_COORDINATES];
    Pair origin_coordinate;
    int hits;
    bool sunk;
} SHIP;

// Score structure for leaderboard
typedef struct {
    char playerName[50];
    int missiles;
} Score;

// Function declarations
void init_grid(char grid[GRID_SIZE][GRID_SIZE]);
void print_grid(char grid[GRID_SIZE][GRID_SIZE], bool reveal);
void random_orientation(char *orientation);
void random_coordinate(Pair *origin);
bool is_valid_position(SHIP ship, char grid[GRID_SIZE][GRID_SIZE]);
void place_ship(SHIP *ship, char grid[GRID_SIZE][GRID_SIZE]);
bool all_sunk(SHIP ships[], int count);
void display_menu();
void start_new_game();
void resume_game();
void save_game(char hiddenGrid[GRID_SIZE][GRID_SIZE], char playerGrid[GRID_SIZE][GRID_SIZE], 
               SHIP ships[], int shots);
bool load_game(char hiddenGrid[GRID_SIZE][GRID_SIZE], char playerGrid[GRID_SIZE][GRID_SIZE], 
               SHIP ships[], int *shots);
void display_top_scores();
void save_score(int missiles);
void sort_scores(Score scores[], int count);
void play_game(char hiddenGrid[GRID_SIZE][GRID_SIZE], char playerGrid[GRID_SIZE][GRID_SIZE], 
               SHIP ships[], int shots);

// Main function
int main() {
    srand(time(0));
    
    int choice;
    
    printf("╔══════════════════════════════════════╗\n");
    printf("║   VALENCIA BATTLESHIP GAME           ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
    
    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                start_new_game();
                break;
            case 2:
                resume_game();
                break;
            case 3:
                display_top_scores();
                break;
            case 4:
                printf("\nThank you for playing Valencia Battleship!\n");
                printf("Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid choice. Please try again.\n\n");
        }
    }
    
    return 0;
}

// Display main menu
void display_menu() {
    printf("\n========== MAIN MENU ==========\n");
    printf("1. Start New Game\n");
    printf("2. Resume Game\n");
    printf("3. Display Top 10 Scores\n");
    printf("4. Quit\n");
    printf("================================\n");
}

// Start a new game
void start_new_game() {
    char hiddenGrid[GRID_SIZE][GRID_SIZE];
    char playerGrid[GRID_SIZE][GRID_SIZE];
    init_grid(hiddenGrid);
    init_grid(playerGrid);
    
    // Initialize ships
    SHIP ships[MAX_SHIPS] = {
        {"Seminole State Ship", 'S', 3, {0}, {{0, 0}}, {0, 0}, 0, false},
        {"Air Force Academy", 'A', 5, {0}, {{0, 0}}, {0, 0}, 0, false},
        {"Valencia Destroyer", 'V', 4, {0}, {{0, 0}}, {0, 0}, 0, false},
        {"Eskimo University", 'E', 3, {0}, {{0, 0}}, {0, 0}, 0, false},
        {"Deland High School", 'D', 2, {0}, {{0, 0}}, {0, 0}, 0, false}
    };
    
    // Place all ships randomly
    for (int i = 0; i < MAX_SHIPS; i++) {
        random_orientation(ships[i].orientation);
        place_ship(&ships[i], hiddenGrid);
    }
    
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║   NEW GAME STARTED                   ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("\nGoal: Sink all 5 ships with the least missiles!\n");
    printf("Perfect score: 17 missiles\n\n");
    
    play_game(hiddenGrid, playerGrid, ships, 0);
}

// Resume a saved game
void resume_game() {
    char hiddenGrid[GRID_SIZE][GRID_SIZE];
    char playerGrid[GRID_SIZE][GRID_SIZE];
    SHIP ships[MAX_SHIPS];
    int shots = 0;
    
    if (load_game(hiddenGrid, playerGrid, ships, &shots)) {
        printf("\n╔══════════════════════════════════════╗\n");
        printf("║   GAME RESUMED                       ║\n");
        printf("╚══════════════════════════════════════╝\n\n");
        play_game(hiddenGrid, playerGrid, ships, shots);
    } else {
        printf("\nNo saved game found. Starting a new game...\n");
        start_new_game();
    }
}

// Main gameplay loop
void play_game(char hiddenGrid[GRID_SIZE][GRID_SIZE], char playerGrid[GRID_SIZE][GRID_SIZE], 
               SHIP ships[], int shots) {
    int x, y;
    char choice;
    
    while (!all_sunk(ships, MAX_SHIPS)) {
        printf("\n--- Missiles Fired: %d ---\n", shots);
        print_grid(playerGrid, false);
        
        printf("\nOptions: (F)ire missile, (S)ave and quit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);
        
        if (choice == 'S' || choice == 's') {
            save_game(hiddenGrid, playerGrid, ships, shots);
            printf("\nGame saved successfully!\n");
            return;
        } else if (choice == 'F' || choice == 'f') {
            printf("Enter coordinates (x y): ");
            scanf("%d %d", &x, &y);
            
            // Validate coordinates
            if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
                printf("Invalid coordinates. Try again.\n");
                continue;
            }
            
            // Check if already fired at this position
            if (playerGrid[y][x] == 'H' || playerGrid[y][x] == 'M' || 
                (playerGrid[y][x] != 'X' && playerGrid[y][x] != '~')) {
                printf("You already fired at this position!\n");
                continue;
            }
            
            shots++;
            
            // Check for hit or miss
            if (hiddenGrid[y][x] != 'X' && hiddenGrid[y][x] != '~') {
                char symbol = hiddenGrid[y][x];
                playerGrid[y][x] = 'H';
                printf("\n💥 HIT!\n");
                
                // Update ship status
                for (int i = 0; i < MAX_SHIPS; i++) {
                    if (ships[i].symbol == symbol) {
                        ships[i].hits++;
                        if (ships[i].hits == ships[i].size) {
                            ships[i].sunk = true;
                            printf("🎯 You sunk the %s!\n", ships[i].name);
                            
                            // Replace all H's with ship symbol
                            for (int j = 0; j < ships[i].size; j++) {
                                playerGrid[ships[i].coords[j].y][ships[i].coords[j].x] = symbol;
                            }
                        }
                        break;
                    }
                }
            } else {
                playerGrid[y][x] = 'M';
                printf("\n💨 MISS!\n");
            }
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    
    // Game completed
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║   🏆 CONGRATULATIONS! 🏆             ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("\nYou sunk all ships in %d missiles!\n", shots);
    
    if (shots == 17) {
        printf("⭐ PERFECT GAME! ⭐\n");
    }
    
    printf("\nFinal Board:\n");
    print_grid(playerGrid, true);
    
    // Save score
    save_score(shots);
    
    // Remove save file after completion
    remove(SAVE_FILE);
}

// Initialize grid with water
void init_grid(char grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = '~';  // Water symbol
}

// Print the game grid
void print_grid(char grid[GRID_SIZE][GRID_SIZE], bool reveal) {
    printf("\n   ");
    for (int x = 0; x < GRID_SIZE; x++) 
        printf("%d  ", x);
    printf("\n");
    
    for (int y = 0; y < GRID_SIZE; y++) {
        printf("%d  ", y);
        for (int x = 0; x < GRID_SIZE; x++) {
            char cell = grid[y][x];
            // Hide ships if not revealed
            if (!reveal && cell != '~' && cell != 'H' && cell != 'M' && 
                cell != 'S' && cell != 'A' && cell != 'V' && cell != 'E' && cell != 'D') {
                printf("~  ");
            } else {
                printf("%c  ", cell);
            }
        }
        printf("\n");
    }
}

// Generate random orientation for ship
void random_orientation(char *orientation) {
    int dir = rand() % 2;
    orientation[0] = (dir == 0) ? 'H' : 'V';  // Horizontal or Vertical
    orientation[1] = (rand() % 2 == 0) ? 'P' : 'N';  // Positive or Negative direction
}

// Generate random starting coordinate
void random_coordinate(Pair *origin) {
    origin->x = rand() % GRID_SIZE;
    origin->y = rand() % GRID_SIZE;
}

// Check if ship placement is valid
bool is_valid_position(SHIP ship, char grid[GRID_SIZE][GRID_SIZE]) {
    int x = ship.origin_coordinate.x;
    int y = ship.origin_coordinate.y;
    
    for (int i = 0; i < ship.size; i++) {
        // Check boundaries
        if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE)
            return false;
        
        // Check if cell is empty
        if (grid[y][x] != '~')
            return false;
        
        // Move to next position
        if (ship.orientation[0] == 'H')
            x += (ship.orientation[1] == 'P') ? 1 : -1;
        else
            y += (ship.orientation[1] == 'P') ? 1 : -1;
    }
    
    return true;
}

// Place ship on the grid
void place_ship(SHIP *ship, char grid[GRID_SIZE][GRID_SIZE]) {
    bool placed = false;
    
    while (!placed) {
        random_coordinate(&ship->origin_coordinate);
        
        if (is_valid_position(*ship, grid)) {
            int x = ship->origin_coordinate.x;
            int y = ship->origin_coordinate.y;
            
            for (int i = 0; i < ship->size; i++) {
                grid[y][x] = ship->symbol;
                ship->coords[i].x = x;
                ship->coords[i].y = y;
                
                if (ship->orientation[0] == 'H')
                    x += (ship->orientation[1] == 'P') ? 1 : -1;
                else
                    y += (ship->orientation[1] == 'P') ? 1 : -1;
            }
            
            placed = true;
        }
    }
}

// Check if all ships are sunk
bool all_sunk(SHIP ships[], int count) {
    for (int i = 0; i < count; i++)
        if (!ships[i].sunk)
            return false;
    return true;
}

// Save current game state
void save_game(char hiddenGrid[GRID_SIZE][GRID_SIZE], char playerGrid[GRID_SIZE][GRID_SIZE], 
               SHIP ships[], int shots) {
    FILE *file = fopen(SAVE_FILE, "w");
    if (!file) {
        printf("Error saving game!\n");
        return;
    }
    
    // Save shot count
    fprintf(file, "%d\n", shots);
    
    // Save grids
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            fprintf(file, "%c ", hiddenGrid[i][j]);
        }
        fprintf(file, "\n");
    }
    
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            fprintf(file, "%c ", playerGrid[i][j]);
        }
        fprintf(file, "\n");
    }
    
    // Save ships
    for (int i = 0; i < MAX_SHIPS; i++) {
        fprintf(file, "%s|%c|%d|%s|%d|%d|%d|%d\n",
                ships[i].name, ships[i].symbol, ships[i].size,
                ships[i].orientation, ships[i].origin_coordinate.x,
                ships[i].origin_coordinate.y, ships[i].hits,
                ships[i].sunk ? 1 : 0);
        
        for (int j = 0; j < ships[i].size; j++) {
            fprintf(file, "%d %d ", ships[i].coords[j].x, ships[i].coords[j].y);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
}

// Load saved game state
bool load_game(char hiddenGrid[GRID_SIZE][GRID_SIZE], char playerGrid[GRID_SIZE][GRID_SIZE], 
               SHIP ships[], int *shots) {
    FILE *file = fopen(SAVE_FILE, "r");
    if (!file) {
        return false;
    }
    
    // Load shot count
    fscanf(file, "%d\n", shots);
    
    // Load grids
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            fscanf(file, "%c ", &hiddenGrid[i][j]);
        }
    }
    
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            fscanf(file, "%c ", &playerGrid[i][j]);
        }
    }
    
    // Load ships
    for (int i = 0; i < MAX_SHIPS; i++) {
        int sunk_int;
        fscanf(file, "%[^|]|%c|%d|%s|%d|%d|%d|%d\n",
               ships[i].name, &ships[i].symbol, &ships[i].size,
               ships[i].orientation, &ships[i].origin_coordinate.x,
               &ships[i].origin_coordinate.y, &ships[i].hits, &sunk_int);
        
        ships[i].sunk = (sunk_int == 1);
        
        for (int j = 0; j < ships[i].size; j++) {
            fscanf(file, "%d %d ", &ships[i].coords[j].x, &ships[i].coords[j].y);
        }
    }
    
    fclose(file);
    return true;
}

// Display top 10 scores
void display_top_scores() {
    FILE *file = fopen(SCORES_FILE, "r");
    Score scores[MAX_SCORES];
    int count = 0;
    
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║       TOP 10 BEST SCORES             ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
    
    if (file) {
        while (count < MAX_SCORES && fscanf(file, "%[^:]:%d\n", 
               scores[count].playerName, &scores[count].missiles) == 2) {
            count++;
        }
        fclose(file);
    }
    
    if (count == 0) {
        printf("No scores yet. Be the first to play!\n");
    } else {
        printf("Rank  Player Name                Missiles\n");
        printf("----  -------------------------  --------\n");
        for (int i = 0; i < count; i++) {
            printf("%-4d  %-25s  %d\n", i + 1, scores[i].playerName, scores[i].missiles);
        }
    }
    
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

// Save the game
void save_score(int missiles) {
    char playerName[50];
    Score scores[MAX_SCORES];
    int count = 0;
    
    printf("\nEnter your name: ");
    scanf(" %[^\n]", playerName);
    
    // Loading the past game
    FILE *file = fopen(SCORES_FILE, "r");
    if (file) {
        while (count < MAX_SCORES && fscanf(file, "%[^:]:%d\n", 
               scores[count].playerName, &scores[count].missiles) == 2) {
            count++;
        }
        fclose(file);
    }
    
    // Add new score
    if (count < MAX_SCORES) {
        strcpy(scores[count].playerName, playerName);
        scores[count].missiles = missiles;
        count++;
    } else {
        // Replace worst score if new score is better
        int maxIdx = 0;
        for (int i = 1; i < MAX_SCORES; i++) {
            if (scores[i].missiles > scores[maxIdx].missiles) {
                maxIdx = i;
            }
        }
        
        if (missiles < scores[maxIdx].missiles) {
            strcpy(scores[maxIdx].playerName, playerName);
            scores[maxIdx].missiles = missiles;
        }
    }
    
    // Sort scores
    sort_scores(scores, count);
    
    // Save to file
    file = fopen(SCORES_FILE, "w");
    if (file) {
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s:%d\n", scores[i].playerName, scores[i].missiles);
        }
        fclose(file);
    }
    
    printf("\nScore saved!\n");
}

// Sort scores lowest to highest
void sort_scores(Score scores[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j].missiles > scores[j + 1].missiles) {
                Score temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}