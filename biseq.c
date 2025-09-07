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

int main() {
    FILE *fp = fopen("search_data.dat", "w");
    if (!fp) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(fp, "#n Seq_B Seq_W Seq_A Bin_B Bin_W Bin_A\n");

    printf("n\tSeq(B)\tSeq(W)\tSeq(A)\tBin(B)\tBin(W)\tBin(A)\n");
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

    // ====== WRITE GNUPLOT SCRIPT ======
    FILE *gp = fopen("plot_search.gnu", "w");
    fprintf(gp,
        "set title \"Sequential vs Binary Search (Comparisons)\"\n"
        "set xlabel \"Input size (n)\"\n"
        "set ylabel \"Number of Comparisons\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"   // popup graph window
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

    // ====== RUN GNUPLOT ======
    system("gnuplot plot_search.gnu");

    printf("\n✅ Graph window opened and also saved as 'search_plot.png'\n");

    return 0;
}
