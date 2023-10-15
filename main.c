#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ALPHABET_SIZE 256
#define MAX_STATES 1000

typedef struct {
    char symbol;
    int nextState;
} Transition;

typedef struct {
    char alphabet[MAX_ALPHABET_SIZE];
    int totalStates;
    int initialState;
    int totalFinalStates;
    int finalStates[MAX_STATES];
    Transition transitions[MAX_STATES][MAX_ALPHABET_SIZE];
} FiniteAutomaton;

FiniteAutomaton parseAutomaton(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        exit(1);
    }

    FiniteAutomaton fa;

    int alphabetSize;
    fscanf(file, "%d", &alphabetSize);

    for(int i = 0; i < alphabetSize; i++) {
        fscanf(file, " %c", &fa.alphabet[i]);
    }
    fa.alphabet[alphabetSize] = '\0';

    fscanf(file, "%d", &fa.totalStates);
    fscanf(file, "%d", &fa.initialState);

    fscanf(file, "%d", &fa.totalFinalStates);
    for(int i = 0; i < fa.totalFinalStates; i++) {
        fscanf(file, "%d", &fa.finalStates[i]);
    }

    for(int i = 0; i < fa.totalStates; i++) {
        for(int j = 0; j < alphabetSize; j++) {
            fa.transitions[i][j].symbol = 0;
        }
    }

    while(!feof(file)) {
        int s;
        char a;
        int s_prime;

        fscanf(file, "%d %c %d", &s, &a, &s_prime);
        int index = strchr(fa.alphabet, a) - fa.alphabet;

        fa.transitions[s][index].symbol = a;
        fa.transitions[s][index].nextState = s_prime;
    }

    fclose(file);
    return fa;
}


void findUnusedLetters(FiniteAutomaton fa) {
    bool isUsed[MAX_ALPHABET_SIZE] = {false};

    for(int i = 0; i < fa.totalStates; i++) {
        for(int j = 0; j < strlen(fa.alphabet); j++) {
            if(fa.transitions[i][j].symbol) {
                isUsed[j] = true;
            }
        }
    }

    printf("The following letters are not accepted by the deterministic automaton:\n");
    for(int i = 0; i < strlen(fa.alphabet); i++) {
        if(!isUsed[i]) {
            printf("%c ", fa.alphabet[i]);
        }
    }
    printf("\n");
}

int main() {
    FiniteAutomaton fa = parseAutomaton("automaton.txt");
    findUnusedLetters(fa);
    return 0;
}