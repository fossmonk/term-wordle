#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ANSWER_SIZE 2315
#define GUESS_SIZE 14855
#define MAX_TRIES 6
#define C_RESET  "\e[0m"
#define C_GREEN  "\e[42m\e[30m"
#define C_YELLOW "\e[43m\e[30m"
#define C_GRAY   "\e[47m\e[30m"
#define C_BLACK  "\e[40m"

typedef enum kb_type_ {
    UNUSED,
    WRONG,
    POS,
    CORRECT
} kb_type_t;

typedef enum color {
    GRAY,
    GREEN,
    YELLOW
} color_t;

typedef struct tile {
    char c;
    color_t color;
} tile_t;


typedef struct _wordle_t {
    char kb_state[26];
    const char *answer;
    tile_t grid[6][5];
    int current_row;
} wordle_t;

const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
extern const char *answers[];
extern const char *guesses[];
const char *exclaims[] = {
    "IMPOSSIBLE",
    "UNREAL",
    "AMAZING",
    "IMPRESSIVE",
    "DECENT",
    "PHEWW"
};

void wordle(void);
void print_state(wordle_t *w);
void print_intro(void);
void print_outro(void);
void update_state(wordle_t *w, char *s);

int main() {
    srand(time(NULL));
    print_intro();
    wordle();
    print_outro();
    return 0;
}

void clearscreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int is_word_valid(char *s) {
    int ret = 0;
    for(int i = 0; i < GUESS_SIZE; i++) {
        if(strcmp(s, guesses[i]) == 0) ret = 1;
    }
    return ret;
}

char prev_guesses[6][6] = { 0 };

int previously_guessed(char *word) {
    int ret = 0;
    for(int i = 0; i < 6; i++) {
        if(strcmp(word, prev_guesses[i]) == 0) ret = 1;
    }
    return ret;
}

void wordle(void) {
    // get the random word of the day
    int idx = rand() % ANSWER_SIZE;

    // setup state
    wordle_t w;
    w.answer = answers[idx];
    w.current_row = 0;

    for(int i = 0; i < 26; i++) {
        w.kb_state[i] = UNUSED;
    }

    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 5; j++) {
            w.grid[i][j].c = ' ';
            w.grid[i][j].color = GRAY;
        }
    }

    int tries = 0;
    // in a loop, wait for input, update and and print the state
    print_state(&w);
    char input[10];
    while(1) {
        printf("Enter your guess: ");
        scanf("%s", input);
        if(!is_word_valid(input)) {
            printf("Word not valid !!!\n");
            continue;
        }
        if(previously_guessed(input)) {
            printf("Word previously guessed !!!\n");
            continue;
        }
        strncpy(prev_guesses[tries], input, 5);
        tries++;
        update_state(&w, input);
        clearscreen();
        print_state(&w);
        if(strcmp(w.answer, input) == 0) {
            printf("%s!! YOU WON !!! Answer is [%s].\n", exclaims[tries-1], w.answer);
            break;
        }
        if(tries >= MAX_TRIES) {
            printf("SORRY YOU LOST :( Answer is [%s].\n", w.answer);
            break;
        }
    }
}

int is_yellow(wordle_t *w, char *guess, int idx) {
    int ret = 1, a_sum = 0, b_sum = 0;
    char c = guess[idx];
    for(int i = 0; i < 5; i++) {
        if(c == w->answer[i]) a_sum += 1;
        if(c == guess[i]) b_sum += 1;
    }
    if(a_sum == 0 || (a_sum != b_sum)) ret = 0;
    return ret;
}

void update_state(wordle_t *w, char *s) {
    for(int i = 0; i < 5; i++) {
        w->grid[w->current_row][i].c = s[i];
        if(s[i] == w->answer[i]) {
            // green
            w->grid[w->current_row][i].color = GREEN;
            w->kb_state[s[i]-'a'] = CORRECT;
        } else {
            if(is_yellow(w, s, i)) {
                // yellow
                w->grid[w->current_row][i].color = YELLOW;
                w->kb_state[s[i]-'a'] = POS;
            } else {
                // gray
                w->grid[w->current_row][i].color = GRAY;
                w->kb_state[s[i]-'a'] = WRONG;
            }
        }
    }
    w->current_row++;
}

char *colormap[] = {
    [GRAY]   = C_GRAY,
    [GREEN]  = C_GREEN,
    [YELLOW] = C_YELLOW
};

char *kbcolormap[] = {
    [UNUSED]  = C_BLACK,
    [WRONG]   = C_GRAY,
    [CORRECT] = C_GREEN,
    [POS]     = C_YELLOW
};

void print_colored_char_tile(tile_t *t) {
    printf("%s %c |"C_RESET, colormap[t->color], toupper(t->c));
}

void print_colored_char_kb(tile_t *t) {
    printf("%s %c "C_RESET, kbcolormap[t->color], t->c);
}

void print_row(tile_t *t) {
    for(int i = 0; i < 5; i++) {
        print_colored_char_tile(&t[i]);
    }
}

void print_kb_row(wordle_t *w, int start, int end) {
    for(int i = start; i < end; i++) {
        tile_t t;
        t.c = alphabet[i];
        t.color = w->kb_state[i];
        print_colored_char_kb(&t);
    }
}

void print_state(wordle_t *w) {
    // print first row of grid
    print_row((tile_t *)&w->grid[0]);
    printf("\n");
    // print second row of grid, first row of kb
    print_row((tile_t *)&w->grid[1]);
    printf("  ");
    print_kb_row(w, 0, 7);
    printf("\n");
    // print third row of grid, second row of kb
    print_row((tile_t *)&w->grid[2]);
    printf("  ");
    print_kb_row(w, 7, 14);
    printf("\n");
    // print fourth row of grid, third row of kb
    print_row((tile_t *)&w->grid[3]);
    printf("  ");
    print_kb_row(w, 14, 21);
    printf("\n");
    // print fifth row of grid, fourth row of kb
    print_row((tile_t *)&w->grid[4]);
    printf("     ");
    print_kb_row(w, 21, 26);
    printf("\n");
    // print last row of grid
    print_row((tile_t *)&w->grid[5]);
    printf("\n");
}

void print_intro() {
    const char *wordle_art =
" _     _  _______  ______    ______   ___      _______   \n"\
"| | _ | ||       ||    _ |  |      | |   |    |       |  \n"\
"| || || ||   _   ||   | ||  |  _    ||   |    |    ___|  \n"\
"|       ||  | |  ||   |_||_ | | |   ||   |    |   |___   \n"\
"|       ||  |_|  ||    __  || |_|   ||   |___ |    ___|  \n"\
"|   _   ||       ||   |  | ||       ||       ||   |___   \n"\
"|__| |__||_______||___|  |_||______| |_______||_______|  \n";

    printf("%s\n Welcome to Wordle!\n\n\n", wordle_art);
}

void print_outro() {
    printf("\nThanks for playing Wordle!\n\n\n");
}
