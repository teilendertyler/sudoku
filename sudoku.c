#include <stdio.h>
#include <stdbool.h>        // bool ist ein Wahrheitswert, entweder kann es oder es kann nicht
#include <stdlib.h>
#include <time.h>       // include time.h damit die random generierten Sudokus sich nicht wiederholen

#pragma warning (disable: 4996) // fscanf, scanf
#define SIZE 9

// Globale Variablen
int grid[SIZE][SIZE];      // Sudoku Gitter
bool fixed[SIZE][SIZE];    // Markierung der festen Zellen (Können nicht bearbeitet werden)

// Funktionsprototypen
void loadSudoku(const char* filename);
void displaySudoku();
bool editSudoku(int row, int col, int value);
bool solveSudoku();
bool isValid(int row, int col, int value);
bool saveSudoku(const char* filename);
void generateSudoku();
void menu();

// Backtracking-Algorithmus für das Lösen des Sudokus  
/* Backtracking ist eine algorithmische Methode, die oft verwendet wird,
 um systematisch alle möglichen Lösungen für ein Problem zu finden.In der
Programmiersprache C wird Backtracking häufig für Probleme wie das Lösen von Rätseln,
das Finden von Permutationen oder Kombinationen und das Durchsuchen von Suchbäumen verwendet.*/

bool solveSudoku() {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) { 
                for (int value = 1; value <= 9; value++) {
                    if (isValid(row, col, value)) {
                        grid[row][col] = value;
                        if (solveSudoku()) {
                            return true;
                        }
                        grid[row][col] = 0;  // um es rückgängig zu machen
                    }
                }
                return false; 
            }
        }
    }
    return true;  // Das Sudoku ist vollständig gelöst
}

// Funktion, um zu überprüfen, ob ein Wert in der Zelle gültig ist
bool isValid(int row, int col, int value) {
    // Überprüfen der Zeile und der Spalte
    for (int i = 0; i < SIZE; i++) {
        if (grid[row][i] == value || grid[i][col] == value) {
            return false;
        }
    }

    // Prüfung des 3x3 Blocks
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[startRow + i][startCol + j] == value) {
                return false;
            }
        }
    }
    return true;
}

// Funktion zum Bearbeiten eines Sudokus
bool editSudoku(int row, int col, int value) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || value < 1 || value > 9) {
        -printf("Falsche Eingabe, wähle einen Wert zwischen: 1 - 9.\n");
        return false;
    }
    if (fixed[row][col]) {
        printf("Zelle konnte nicht bearbeitet werden.\n");
        return false;
    }
    if (!isValid(row, col, value)) {
        printf("Falsche eingabe. Das verstößt gegen die Regeln!\n");
        return false;
    }

    grid[row][col] = value;
    return true;
}

// Funktion zum Laden eines Sudokus aus einer Datei mit Dateiname
void loadSudoku(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Fehler: Die Datei konnte nicht geladen werden.\n");
        return;
    }
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            fscanf(file, "%d", &grid[row][col]);
            fixed[row][col] = (grid[row][col] != 0); 
        }
    }
    fclose(file);
    printf("Erfolgreich geladen.\n");
}

// Funktion zum Speichern eines Sudokus in einer Datei
bool saveSudoku(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Fehler: Die Datei konnte nicht gefunden werden.\n");
        return false;
    }
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            fprintf(file, "%d ", grid[row][col]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Spielstand gespeichert!\n");
    return true;
}

// Funktion zum Generieren eines zufälligen und lösbaren Sudokus
void generateSudoku() {
    // Setze alle Zellen auf 0
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            grid[row][col] = 0;
            fixed[row][col] = false;
        }
    }

    srand(time(NULL));  // Initialisiere den Zufallsgenerator dafür existiert include time.h

    // Backtracking-Algorithmus zur Generierung eines Sudokus
    if (solveSudoku()) {
        // Entferne zufällige Zahlen aus ein paar Zellen
        int count = 0;
        while (count < 30) {  // Entferne bis zu 30 Zahlen
            int row = rand() % SIZE;
            int col = rand() % SIZE;

            if (grid[row][col] != 0) {
                grid[row][col] = 0;
                fixed[row][col] = false;
                count++;
            }
        }
        printf("Erfolgreich generiert, viel spass!\n");
    }
    else {
        printf("Fehler: Sudoku konnte nicht generiert werden.\n");
    }
}

// Funktion zum Anzeigen des aktuellen Sudoku-Zustands
void displaySudoku() {
    printf("\nAktueller Sudoku zustand:\n");
    for (int row = 0; row < SIZE; row++) {
        if (row % 3 == 0 && row != 0) {
            printf("---------------------\n");  // Horizontale Linie zwischen Blöcken
        }
        for (int col = 0; col < SIZE; col++) {
            if (col % 3 == 0 && col != 0) {
                printf("| ");  // Vertikale Linie zwischen Blöcken
            }
            if (grid[row][col] == 0) {
                printf(". ");  // Das ist ein Platzhalter für leere Zellen
            }
            else {
                printf("%d ", grid[row][col]);
            }
        }
        printf("\n");
    }
}
// Funktion zum Zurücksetzen des Sudokus
void resetSudoku() {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (!fixed[row][col]) {
                grid[row][col] = 0;  // Setze alle bearbeitbaren Zellen zurück
            }
        }
    }
    printf("Erfolgreich zurückgesetzt\n");
}
void startNewSudoku() {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            grid[row][col] = 0;     // Setze alle Zellen auf 0
            fixed[row][col] = false; 
        }
    }
    printf("Ein neues leeres Sudoku wurde gestartet.\n");
}

// Menü
void menu() {
    int choice;
    do {
        printf("\nSudoku Menu: \n");
        printf("1. Sudoku Anzeigen\n");
        printf("2. Play\n");
        printf("3. Sudoku Laden\n");
        printf("4. Save\n");
        printf("5. Löse Sudoku\n");
        printf("6. Generiere ein Sudoku\n");
        printf("7. Reset\n");
        printf("8. Neues Sudoku\n");
        printf("0. Beenden\n");
        printf("Gib ein: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            displaySudoku();
            break;
        case 3: {
            char filename[256];
            printf("Spielspeicherstand: \n");
            scanf("%s", filename);
            loadSudoku(filename);
            break;
        }
        case 2: {
            int row, col, value;
            while (1) {
                printf("Reihe (1-9) (0 zum abbruch): ");
                scanf("%d", &row);
                if (row == 0) break;
                printf("Spalte (1-9) (0 zum abbruch): ");
                scanf("%d", &col);
                if (col == 0) break;
                printf("Wert (1-9): ");
                scanf("%d", &value);
                if (editSudoku(row - 1, col - 1, value)) {
                    printf("Wert erfolgreich geändert.n");
                }
                else {
                    printf("Fehler: Versuch es nochmal.\n");
                }
            }
            break;
        }
        case 4: {
            char filename[256];
            printf("Spielspeicherstand: \n");
            scanf("%s", filename);
            saveSudoku(filename);
            break;
        }
        case 5:
            if (solveSudoku()) {
                printf("Sudoku wurde gelöst, Nice!\n");
                displaySudoku();
            }
            else {
                printf("Konnte nicht gelöst werden.\n");
            }
            break;
        case 6:
            generateSudoku();
            displaySudoku();
            break;
        case 7:
            resetSudoku();
            displaySudoku();
            break;
        case 8:
            startNewSudoku();
            displaySudoku();
            break;
        case 0:
            printf("Programm wurde erfolgreich geschlossen.\n");
            break;
        default:
            printf("Falsche Eingabe.\n");
        }
    } while (choice != 0);
}

// Hauptfunktion am Ende
int main() {
    printf("Willkommen zum Sudoku!\n");
    system("chcp 1252 > NUL"); // für ÜÄÖ usw
    menu();
    return 0;
}