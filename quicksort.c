#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int steps; // global counter for comparisons

// ===== Partition Function =====
int partition(int arr[], int L, int R) {
    int pivot = arr[R];  // pivot as last element
    int i = L - 1;

    for (int j = L; j < R; j++) {
        steps++;  // count comparison
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[R];
    arr[R] = temp;

    return i + 1;
}

// ===== Quick Sort =====
void quickSort(int arr[], int L, int R) {
    if (L < R) {
        int P = partition(arr, L, R);
        quickSort(arr, L, P - 1);
        quickSort(arr, P + 1, R);
    }
}

// ===== Tester (Interactive) =====
void tester() {
    int N;
    printf("Enter number of elements: ");
    scanf("%d", &N);
    int arr[N];

    printf("Enter %d elements: ", N);
    for (int i = 0; i < N; i++) scanf("%d", &arr[i]);

    steps = 0;
    quickSort(arr, 0, N - 1);

    printf("Sorted Array: ");
    for (int i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\nSteps taken = %d\n", steps);
}

// ===== Analysis & Graph =====
void analysis() {
    FILE *fp = fopen("quick_data.txt", "w");
    if (!fp) { printf("Cannot create data file\n"); return; }

    srand(time(0));

    printf("N\tBest\tWorst\n");
    for (int N = 10; N <= 200; N += 10) {
        int arr[N], i, best, worst;

        // === Best Case: Randomized array ===
        for (i = 0; i < N; i++) arr[i] = rand() % 1000;
        steps = 0;
        quickSort(arr, 0, N - 1);
        best = steps;

        // === Worst Case: Already sorted ===
        for (i = 0; i < N; i++) arr[i] = i;
        steps = 0;
        quickSort(arr, 0, N - 1);
        worst = steps;

        printf("%d\t%d\t%d\n", N, best, worst);
        fprintf(fp, "%d %d %d\n", N, best, worst);
    }
    fclose(fp);

    // ===== Create gnuplot script =====
    FILE *gp = fopen("quick_plot.gnu", "w");
    if (!gp) { printf("Cannot create gnuplot script\n"); return; }

    fprintf(gp,
        "set title \"Quick Sort: Best vs Worst Case\"\n"
        "set xlabel \"Array Size (N)\"\n"
        "set ylabel \"Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"quick_data.txt\" using 1:2 title \"Best Case\" lc rgb \"blue\", \\\n"
        "     \"quick_data.txt\" using 1:3 title \"Worst Case\" lc rgb \"red\"\n"
        "set term png size 1000,600\n"
        "set output \"quick_sort_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    // Run gnuplot automatically
    system("gnuplot quick_plot.gnu");
}

// ===== Main =====
int main() {
    int choice;
    printf("1. Tester\n2. Analysis with Graph\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) tester();
    else if (choice == 2) analysis();
    else printf("Invalid choice\n");

    return 0;
}