#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int steps; // global counter

// ===== Bottom-Up Heapify =====
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    // Find largest among parent and children
    if (left < n) { steps++; if (arr[left] > arr[largest]) largest = left; }
    if (right < n) { steps++; if (arr[right] > arr[largest]) largest = right; }

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

// ===== Build Heap (Bottom-Up) =====
void buildHeap(int arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

// ===== Heap Sort =====
void heapSort(int arr[], int n) {
    buildHeap(arr, n);

    for (int i = n-1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
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
    heapSort(arr, N);

    printf("Sorted Array: ");
    for (int i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\nSteps taken = %d\n", steps);
}

// ===== Analysis =====
void analysis() {
    FILE *fp = fopen("heap_data.txt", "w");
    if (!fp) { printf("Cannot create data file\n"); return; }

    srand(time(0));

    printf("N\tBest\tWorst\n");
    for (int N = 10; N <= 200; N += 10) {
        int arr[N], i, best, worst;

        // === Best Case: Already sorted ===
        for (i = 0; i < N; i++) arr[i] = i;
        steps = 0;
        heapSort(arr, N);
        best = steps;

        // === Worst Case: Reverse sorted ===
        for (i = 0; i < N; i++) arr[i] = N-i;
        steps = 0;
        heapSort(arr, N);
        worst = steps;

        printf("%d\t%d\t%d\n", N, best, worst);
        fprintf(fp, "%d %d %d\n", N, best, worst);
    }
    fclose(fp);

    // ===== Create gnuplot script =====
    FILE *gp = fopen("heap_plot.gnu", "w");
    if (!gp) { printf("Cannot create gnuplot script\n"); return; }

    fprintf(gp,
        "set title \"Heap Sort (Bottom-Up): Best vs Worst Case\"\n"
        "set xlabel \"Array Size (N)\"\n"
        "set ylabel \"Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"heap_data.txt\" using 1:2 title \"Best Case\" lc rgb \"blue\", \\\n"
        "     \"heap_data.txt\" using 1:3 title \"Worst Case\" lc rgb \"red\"\n"
        "set term png size 1000,600\n"
        "set output \"heap_sort_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    // Run gnuplot automatically
    system("gnuplot heap_plot.gnu");
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
