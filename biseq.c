#include <stdio.h>
#include <stdlib.h>

// ===== Sequential (Linear) Search =====
int sequential_search(int arr[], int n, int key, int *count) {
    *count = 0;
    for (int i = 0; i < n; i++) {
        (*count)++;
        if (arr[i] == key)
            return i; // found
    }
    return -1; // not found
}

// ===== Binary Search (Recursive) =====
int binary_search_recursive(int arr[], int low, int high, int key, int *count) {
    if (low > high)
        return -1;

    (*count)++;
    int mid = (low + high) / 2;

    if (arr[mid] == key)
        return mid;
    else if (key < arr[mid])
        return binary_search_recursive(arr, low, mid - 1, key, count);
    else
        return binary_search_recursive(arr, mid + 1, high, key, count);
}

// ===== Interactive Tester =====
void run_tester() {
    int choice, n, key, index;
    int arr[100], count;

    while (1) {
        printf("\nENTER 1. TO LINEAR SEARCH\n");
        printf("2. TO BINARY SEARCH\n");
        printf("3. TO EXIT\n");
        scanf("%d", &choice);

        if (choice == 3) break;

        printf("ENTER THE NUMBER OF ELEMENTS\n");
        scanf("%d", &n);
        printf("ENTER THE ELEMENTS OF THE ARRAY\n");
        for (int i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        printf("ENTER THE KEY ELEMENT\n");
        scanf("%d", &key);

        if (choice == 1) {
            index = sequential_search(arr, n, key, &count);
            if (index == -1)
                printf("Element not found\n");
            else
                printf("The element is present at the index %d\n", index);
        }
        else if (choice == 2) {
            // Binary search requires sorted array
            // (Assume user entered sorted array)
            index = binary_search_recursive(arr, 0, n - 1, key, &count);
            if (index == -1)
                printf("Element not found\n");
            else
                printf("The element is present at the index %d\n", index);
        }
        else
            printf("Invalid choice!\n");
    }
}

// ===== Analysis + Graph =====
void run_analysis() {
    FILE *fp = fopen("search_data.dat", "w");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "#n Seq_B Seq_W Seq_A Bin_B Bin_W Bin_A\n");

    printf("\nn\tSeq(B)\tSeq(W)\tSeq(A)\tBin(B)\tBin(W)\tBin(A)\n");
    printf("-----------------------------------------------------\n");

    for (int n = 10; n <= 200; n += 10) {
        int arr[n];
        for (int i = 0; i < n; i++)
            arr[i] = i + 1;

        int count;

        // Sequential search
        sequential_search(arr, n, arr[0], &count);
        int seq_best = count;

        sequential_search(arr, n, arr[n - 1], &count);
        int seq_worst = count;

        sequential_search(arr, n, arr[n / 2], &count);
        int seq_avg = count;

        // Binary search
        binary_search_recursive(arr, 0, n - 1, arr[n / 2], &count);
        int bin_best = count;

        binary_search_recursive(arr, 0, n - 1, arr[n - 1], &count);
        int bin_worst = count;

        binary_search_recursive(arr, 0, n - 1, arr[rand() % n], &count);
        int bin_avg = count;

        // print to terminal
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               n, seq_best, seq_worst, seq_avg, bin_best, bin_worst, bin_avg);

        // write to file
        fprintf(fp, "%d %d %d %d %d %d %d\n",
                n, seq_best, seq_worst, seq_avg, bin_best, bin_worst, bin_avg);
    }
    fclose(fp);

    // ====== GNU PLOTTER SCRIPT ======
    FILE *gp = fopen("plot_search.gnu", "w");
    fprintf(gp,
        "set title \"Sequential vs Binary Search (Comparisons)\"\n"
        "set xlabel \"Input size (n)\"\n"
        "set ylabel \"Number of Comparisons\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"search_data.dat\" using 1:2 title \"Seq (Best)\", \\\n"
        "     \"search_data.dat\" using 1:3 title \"Seq (Worst)\", \\\n"
        "     \"search_data.dat\" using 1:4 title \"Seq (Avg)\", \\\n"
        "     \"search_data.dat\" using 1:5 title \"Bin (Best)\", \\\n"
        "     \"search_data.dat\" using 1:6 title \"Bin (Worst)\", \\\n"
        "     \"search_data.dat\" using 1:7 title \"Bin (Avg)\"\n"
        "set term png size 1000,600\n"
        "set output \"search_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    system("gnuplot plot_search.gnu");

    printf("\n✅ Graph window opened and also saved as 'search_plot.png'\n");
}

int main() {
    int mode;
    printf("\n=== MENU ===\n");
    printf("1. Run Search Tester (interactive)\n");
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