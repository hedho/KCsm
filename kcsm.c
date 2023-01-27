#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 2048

int main(int argc, char *argv[]) {
    char paste[MAX_LENGTH];
    char filename[MAX_LENGTH];

    if (argc > 1) {
        if (strcmp(argv[1], "create") == 0) {
            printf("Enter your paste: ");
            fgets(paste, MAX_LENGTH, stdin);

            time_t t = time(NULL);
            snprintf(filename, MAX_LENGTH, "%ld.txt", t);

            FILE *fp = fopen(filename, "w");
            fprintf(fp, "%s", paste);
            fclose(fp);

            printf("Paste created: http://localhost/pastes/%s\n", filename);
        } else if (strcmp(argv[1], "search") == 0) {
            if (argc < 3) {
                printf("Error: Please enter a search term\n");
                return 1;
            }

            char *search_term = argv[2];

            printf("<html>\n<head>\n<link rel='stylesheet' href='materialize.css'>\n</head>\n<body>\n");
            printf("<form>\n<input type='text' name='search' value='%s'>\n<input type='submit' value='Search'>\n</form>\n", search_term);

            FILE *fp = popen("ls *.txt", "r");
            char line[MAX_LENGTH];
            int match_count = 0;
            while (fgets(line, MAX_LENGTH, fp)) {
                line[strlen(line) - 1] = '\0';

                FILE *fp2 = fopen(line, "r");
                char paste[MAX_LENGTH];
                fgets(paste, MAX_LENGTH, fp2);
                fclose(fp2);

                if (strstr(paste, search_term) != NULL) {
                    printf("<a href='/pastes/%s'>%s</a><br>\n", line, line);
                    match_count++;
                }
            }
            pclose(fp);

            if (match_count == 0) {
                printf("No matches found\n");
            }

            printf("</body>\n</html>\n");
        }
    } else {
        printf("Usage: pastebin create\n       pastebin search <search_term>\n");
    }

    return 0;
}
