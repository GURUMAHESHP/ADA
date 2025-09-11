#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Step counter for analysis
int step_count;

// Brute Force String Matching
int brute_force(char text[], char pattern[])
{
    int N = strlen(text);
    int M = strlen(pattern);
    step_count = 0;

    for (int i = 0; i <= N - M; i++) {
        int j = 0;
        while (j < M && text[i + j] == pattern[j]) {
            step_count++;
            j++;
        }
        step_count++; // mismatch / last check
        if (j == M) {
            return i; // Match found
        }
    }
    return -1; // No match
}

// ===== Interactive Tester =====
void run_tester()
{
    char text[500], pattern[100];
    int pos;

    printf("\nEnter Text: ");
    scanf("%s", text);
    printf("Enter Pattern: ");
    scanf("%s", pattern);

    pos = brute_force(text, pattern);

    if (pos != -1)
        printf("✅ Pattern found at index %d\n", pos);
    else
        printf("❌ Pattern not found\n");

    printf("Steps taken = %d\n", step_count);
}

// ===== Analysis + Graph =====
void run_analysis()
{
    FILE *fp = fopen("stringmatch_data.dat", "w");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "#n Best Worst\n");
    printf("\nn\tBest\tWorst\n");
    printf("------------------------\n");

    for (int n = 10; n <= 200; n += 10) {
        char text[500], pattern[500];
        int best, worst;

        // ===== Best Case ===== (instant match at start)
        for (int i = 0; i < n; i++) text[i] = 'a';
        text[n] = '\0';
        strcpy(pattern, "a");   // immediate match
        brute_force(text, pattern);
        best = step_count;

        // ===== Worst Case ===== (no match at all)
        for (int i = 0; i < n; i++) text[i] = 'a';
        text[n] = '\0';
        strcpy(pattern, "b");   // mismatch always
        brute_force(text, pattern);
        worst = step_count;

        printf("%d\t%d\t%d\n", n, best, worst);
        fprintf(fp, "%d %d %d\n", n, best, worst);
    }

    fclose(fp);

    // ===== gnuplot script =====
    FILE *gp = fopen("plot_stringmatch.gnu", "w");
    fprintf(gp,
        "set title \"Brute Force String Matching: Best vs Worst Case\"\n"
        "set xlabel \"Text Length (n)\"\n"
        "set ylabel \"Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"stringmatch_data.dat\" using 1:2 title \"Best Case\", \\\n"
        "     \"stringmatch_data.dat\" using 1:3 title \"Worst Case\"\n"
        "set term png size 1000,600\n"
        "set output \"stringmatch_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    system("gnuplot plot_stringmatch.gnu");
    printf("\n✅ Graph window opened and also saved as 'stringmatch_plot.png'\n");
}

// ===== Main Menu =====
int main()
{
    int mode;
    printf("\n=== MENU ===\n");
    printf("1. Run Brute Force Tester (interactive)\n");
    printf("2. Run Analysis + Generate Graph\n");
    printf("Choice: ");
    scanf("%d", &mode);

    if (mode == 1)
        run_tester();
    else if (mode == 2)
        run_analysis();
    else
        printf("Invalid choice!\n");

    return 0;
}