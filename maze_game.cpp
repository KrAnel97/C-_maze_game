#include <fstream>
#include <ctime>
#include <cstdlib>
#include <curses.h> 
using namespace std;

class Game {
protected:
    int rows, cols;
    char maze[30][50];
    int malfoy_row, malfoy_col;
    int babis_row, babis_col;
    int gem_row, gem_col;
    bool game_over;
    int malfoy_moves;
    int babis_moves;

public:
    Game() {
        initscr();  // Αρχικοποίηση της pdcurses
        raw();  // Ενεργοποίηση της ακατέργαστης εισόδου
        keypad(stdscr, TRUE);  // Ενεργοποίηση των πλήκτρων λειτουργίας

        string fileName = "Maze1.txt"; //Μπορούμε εδώ να βάλουμε όποιον από τους 4 χάρτες έχω φτιάξει

        srand(time(NULL));  // Αρχικοποίηση του γεννήτορα τυχαίων αριθμών με βάση την τρέχουσα χρονική στιγμή

        ifstream infile(fileName);
        if (!infile) {
            printw("Σφάλμα ανοίγματος αρχείου\n");
            return;
        }

        rows = 30;  // Ορισμός του πλήθους των γραμμών του λαβυρίνθου
        cols = 50;  // Ορισμός του πλήθους των στηλών του λαβυρίνθου
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                infile >> maze[i][j];   // Ανάγνωση του στοιχείου του λαβυρίνθου στη θέση (i, j) από το αρχείο
            }
        }

        malfoy_moves = 0; // Αρχικοποίηση του μετρητή κινήσεων του χαρακτήρα Malfoy
        babis_moves = 0;  // Αρχικοποίηση του μετρητή κινήσεων του χαρακτήρα Babis
        game_over = false; // Αρχικοποίηση της μεταβλητής game_over σε false για να είναι ενεργό το παιχνίδι

        findRandomPosition(babis_row, babis_col);  // Βρες τυχαία θέση για τον Μπάμπη
        findRandomPosition(malfoy_row, malfoy_col);  // Βρες τυχαία θέση για τον Μαλφόη
        findRandomGemPosition();  // Βρες τυχαία θέση για το πετράδι
    }

    void findRandomPosition(int& row, int& col) {
        do {
            row = rand() % rows;
            col = rand() % cols;
        } while (maze[row][col] == '*');
        // Βρίσκει μια τυχαία θέση για τον χαρακτήρα (row, col)
    // εξασφαλίζοντας ότι δεν βρίσκεται σε τοίχο (*)
    }

    void findRandomGemPosition() {
        do {
            gem_row = rand() % rows;
            gem_col = rand() % cols;
        } while (maze[gem_row][gem_col] == '*' ||
                 (gem_row == malfoy_row && gem_col == malfoy_col) ||
                 (gem_row == babis_row && gem_col == babis_col));
                 // Βρίσκει μια τυχαία θέση για το πετράδι (gem_row, gem_col),
    // εξασφαλίζοντας ότι δεν βρίσκεται σε τοίχο (*)
    // ή στη θέση του Μαλφόη (malfoy_row, malfoy_col)
    // ή στη θέση του Μπάμπη (babis_row, babis_col)
    }

    void play() {
        while (!game_over) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (i == malfoy_row && j == malfoy_col) {
                        printw("L");
                    } else if (i == babis_row && j == babis_col) {
                        printw("B");
                    } else if (i == gem_row && j == gem_col) {
                        printw("G");
                    } else {
                        printw("%c", maze[i][j]);
                    }
                    printw(" ");
                }
                printw("\n");
                // Εκτυπώνει την τρέχουσα κατάσταση του παιχνιδιού,
        // όπου ο Μαλφόης συμβολίζεται με το γράμμα L,
        // ο Μπάμπης με το γράμμα B και το πετράδι με το γράμμα G,
        // ενώ οι χαρακτήρες του λαβυρίνθου εκτυπώνονται ως είναι
        // μετά από ένα κενό διάστημα.
            }

            printw("Κίνηση Μπάμπη (w/a/s/d): ");
            int babis_move = getch();

            int new_babis_row = babis_row;
            int new_babis_col = babis_col;
            switch (babis_move) {
                case 'w':
                    new_babis_row--; // Κίνηση πάνω
                    break;
                case 'a':
                    new_babis_col--; // Κίνηση αριστερά
                    break;
                case 's':
                    new_babis_row++; // Κίνηση κάτω
                    break;
                case 'd':
                    new_babis_col++; // Κίνηση δεξιά
                    break;
                default:
                    printw("Μη έγκυρη κίνηση!\n");
                    continue;
            }

            if (new_babis_row >= 0 && new_babis_row < rows && new_babis_col >= 0 && new_babis_col < cols &&
                maze[new_babis_row][new_babis_col] != '*') {
                babis_row = new_babis_row; // Ενημέρωση της νέας θέσης του χαρακτήρα Babis στον άξονα των γραμμών
                babis_col = new_babis_col; // Ενημέρωση της νέας θέσης του χαρακτήρα Babis στον άξονα των στηλών
                babis_moves++;             // Αύξηση του μετρητή κινήσεων του χαρακτήρα Babis

                if (babis_row == gem_row && babis_col == gem_col) {
                    printw("Ο Μπάμπης βρήκε το μαγικό πετράδι και κερδίζει!\n");
                    game_over = true;      // Ορισμός της μεταβλητής game_over σε true για να λήξει το παιχνίδι
                }
            } else {
                printw("Μη έγκυρη κίνηση!\n");  // Εκτύπωση μηνύματος όταν η κίνηση του Μπάμπη δεν είναι έγκυρη
            }

            int new_malfoy_row = malfoy_row;   // Δημιουργία μιας μεταβλητής για τη νέα θέση του χαρακτήρα Malfoy στον άξονα των γραμμών
            int new_malfoy_col = malfoy_col;   // Δημιουργία μιας μεταβλητής για τη νέα θέση του χαρακτήρα Malfoy στον άξονα των στηλών
            int malfoy_move = rand() % 4;      // Τυχαία επιλογή ενός αριθμού από το 0 έως το 3 για την κίνηση του χαρακτήρα Malfoy
            switch (malfoy_move) {
                case 0:
                    new_malfoy_row--; // Κίνηση πάνω
                    break;
                case 1:
                    new_malfoy_col--; // Κίνηση αριστερά
                    break;
                case 2:
                    new_malfoy_row++; // Κίνηση κάτω
                    break;
                case 3:
                    new_malfoy_col++; // Κίνηση δεξιά
                    break; 
            }

            if (new_malfoy_row >= 0 && new_malfoy_row < rows && new_malfoy_col >= 0 && new_malfoy_col < cols &&
                maze[new_malfoy_row][new_malfoy_col] != '*') {
                malfoy_row = new_malfoy_row;    // Ενημέρωση της νέας θέσης του χαρακτήρα Malfoy στον άξονα των γραμμών
                malfoy_col = new_malfoy_col;   // Ενημέρωση της νέας θέσης του χαρακτήρα Malfoy στον άξονα των στηλών
                malfoy_moves++;     // Αύξηση του μετρητή κινήσεων του χαρακτήρα Malfoy

                if (malfoy_row == babis_row && malfoy_col == babis_col) {
                    printw("Ο Μαλφόης πιάνει τον Μπάμπη και κερδίζει!\n");   // Εκτύπωση μηνύματος όταν ο Μαλφόης πιάνει τον Μπάμπη
                    game_over = true;     // Ορισμός της μεταβλητής game_over σε true για να λήξει το παιχνίδι
                }
            }

            if (malfoy_moves > 10 && babis_moves > 10) {
                printw("Κανείς δεν κερδίζει!\n");    // Εκτύπωση μηνύματος όταν παίκτες έχουν κάνει πάνω από 10 κινήσεις
                game_over = true;   // Ορισμός της μεταβλητής game_over σε true για να λήξει το παιχνίδι
            }

            refresh();   // Ανανέωση της οθόνης με τις νέες εκτυπώσεις
        }
    }
};

int main() {
    initscr();  // Αρχικοποίηση της οθόνης
    noecho();   // Απενεργοποίηση της εμφάνισης των χαρακτήρων
    curs_set(0);  // Απόκρυψη του κέρσορα

    Game game;
    game.play();

    getch();
    endwin();  // Τερματισμός της οθόνης

    return 0;
}
