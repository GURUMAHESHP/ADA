#include <stdio.h>
#include <stdlib.h>

int steps; // count comparisons

// ===== Merge Function =====
void merge(int arr[], int L, int M, int R) {
    int A, B, C;
    int nL = M - L + 1;
    int nR = R - M;
    int Left[nL], Right[nR];

    for (A = 0; A < nL; A++) Left[A] = arr[L + A];
    for (B = 0; B < nR; B++) Right[B] = arr[M + 1 + B];

    A = B = 0;
    C = L;

    while (A < nL && B < nR) {
        steps++;  // count comparison
        if (Left[A] <= Right[B]) arr[C++] = Left[A++];
        else arr[C++] = Right[B++];
    }
    while (A < nL) arr[C++] = Left[A++];
    while (B < nR) arr[C++] = Right[B++];
}

// ===== Merge Sort =====
void mergeSort(int arr[], int L, int R) {
    if (L < R) {
        int M = (L + R) / 2;
        mergeSort(arr, L, M);
        mergeSort(arr, M + 1, R);
        merge(arr, L, M, R);
    }
}

// ===== Interactive Tester =====
void tester() {
    int A;
    printf("Enter number of elements: ");
    scanf("%d", &A);
    int arr[A];

    printf("Enter %d elements: ", A);
    for (int B = 0; B < A; B++) scanf("%d", &arr[B]);

    steps = 0;
    mergeSort(arr, 0, A - 1);

    printf("Sorted Array: ");
    for (int B = 0; B < A; B++) printf("%d ", arr[B]);
    printf("\nSteps taken = %d\n", steps);
}

// ===== Analysis & Graph =====
void analysis() {
    FILE *fp = fopen("merge_data.txt", "w");

    printf("A\tBest\tWorst\n");
    for (int A = 10; A <= 200; A += 10) {
        int arr[A], B, best, worst;

        // Best case: already sorted
        for (B = 0; B < A; B++) arr[B] = B;
        steps = 0;
        mergeSort(arr, 0, A - 1);
        best = steps;

        // Worst case: reverse sorted
        for (B = 0; B < A; B++) arr[B] = A - B;
        steps = 0;
        mergeSort(arr, 0, A - 1);
        worst = steps;

        printf("%d\t%d\t%d\n", A, best, worst);
        fprintf(fp, "%d %d %d\n", A, best, worst);
    }

    fclose(fp);

    // Auto plot using gnuplot
    system("gnuplot -persist -e \"set title 'Merge Sort Analysis'; set xlabel 'Array Size (A)'; set ylabel 'Steps'; plot 'merge_data.txt' using 1:2 with lines title 'Best', 'merge_data.txt' using 1:3 with lines title 'Worst'\"");
}

// ===== Main =====
int main() {
    int A;
    printf("1. Tester\n2. Analysis with Graph\nChoice: ");
    scanf("%d", &A);

    if (A == 1) tester();
    else if (A == 2) analysis();
    else printf("Invalid choice\n");

    return 0;
}
