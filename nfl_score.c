#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void trim(char *s) {
    char *p = s, *q = s + strlen(s);
    while (p < q && isspace((unsigned char)*p)) p++;
    while (q > p && isspace((unsigned char)q[-1])) q--;
    size_t n = (size_t)(q - p);
    memmove(s, p, n);
    s[n] = '\0';
}

int main(void) {
    char line[256];

    for (;;) {
        printf("Enter the NFL score (Enter 1 to stop): ");
        if (!fgets(line, sizeof line, stdin)) break;   // ends the program
        trim(line);
        if (line[0] == '\0') { puts("Invalid input. Enter an integer score or 1 to stop."); continue; }
        if (strcmp(line, "1") == 0) break;             
        char *end = NULL;
        long v = strtol(line, &end, 10);
        if (end == line) { puts("Invalid input. Enter an integer."); continue; }
        while (*end && isspace((unsigned char)*end)) end++;
        if (*end) { puts("Invalid input. Enter just the number."); continue; }
        if (v < 0) { puts("Invalid input. Score cannot be negative."); continue; }
        if (v > 1000) { puts("That score is very large, results may take a while."); }

        int score = (int)v;

        printf("Possible combinations of scoring plays if a team's score is %d:\n", score);
        int count = 0;

        for (int td2 = 0; td2 <= score / 8; ++td2) {                             // TD + 2-pt (8)
            for (int td1 = 0; td1 <= (score - 8*td2) / 7; ++td1) {               // TD + 1-pt (7)
                for (int td  = 0; td  <= (score - 8*td2 - 7*td1) / 6; ++td) {    // TD (6)
                    for (int fg  = 0; fg  <= (score - 8*td2 - 7*td1 - 6*td) / 3; ++fg) { // FG (3)
                        int used = 8*td2 + 7*td1 + 6*td + 3*fg;
                        int rem  = score - used;
                        if (rem < 0) continue;
                        if (rem % 2 != 0) continue;                              // Safety (2)
                        int saf = rem / 2;

                        printf("%d TD + 2pt, %d TD + FG, %d TD, %d 3pt FG, %d Safety\n",
                               td2, td1, td, fg, saf);
                        ++count;
                    }
                }
            }
        }

        if (count == 0) {
            printf("No combinations possible for %d.\n", score);
        }
    }

    return 0;
}