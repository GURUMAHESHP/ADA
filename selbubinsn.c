#include <stdio.h>
#include <stdlib.h>

// ===== Global Counter =====
int counter;

// ===== Selection Sort =====
void selection_sort(int arr[], int n) {
    counter = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            counter++;
            if (arr[j] < arr[min])
                min = j;
        }
        int temp = arr[min];
        arr[min] = arr[i];
        arr[i] = temp;
    }
}

// ===== Bubble Sort =====
void bubble_sort(int arr[], int n) {
    counter = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            counter++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// ===== Insertion Sort =====
void insertion_sort(int arr[], int n) {
    counter = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            counter++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else break;
        }
        arr[j + 1] = key;
    }
}

// ===== Interactive Tester =====
void run_tester() {
    int choice, n;
    int arr[100];

    while (1) {
        printf("\nSORTING MENU\n");
        printf("1. Selection Sort\n");
        printf("2. Bubble Sort\n");
        printf("3. Insertion Sort\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        printf("Enter number of elements: ");
        scanf("%d", &n);
        printf("Enter %d elements: ", n);
        for (int i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        if (choice == 1)
            selection_sort(arr, n);
        else if (choice == 2)
            bubble_sort(arr, n);
        else if (choice == 3)
            insertion_sort(arr, n);
        else {
            printf("Invalid choice!\n");
            continue;
        }

        printf("Sorted Array: ");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\nSteps Count = %d\n", counter);
    }
}

// ===== Analyzer + Graph =====
void run_analysis() {
    FILE *fp = fopen("sort_data.dat", "w");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "#n Sel_B Sel_W Sel_A Bub_B Bub_W Bub_A Ins_B Ins_W Ins_A\n");

    printf("\nn\tSel(B)\tSel(W)\tSel(A)\tBub(B)\tBub(W)\tBub(A)\tIns(B)\tIns(W)\tIns(A)\n");
    printf("-------------------------------------------------------------------------------------\n");

    for (int n = 10; n <= 200; n += 10) {
        int arr[n], arr2[n], arr3[n];

        // Best Case (already sorted)
        for (int i = 0; i < n; i++) arr[i] = arr2[i] = arr3[i] = i;
        selection_sort(arr, n); int sel_best = counter;
        bubble_sort(arr2, n);   int bub_best = counter;
        insertion_sort(arr3, n);int ins_best = counter;

        // Worst Case (reverse sorted)
        for (int i = 0; i < n; i++) arr[i] = arr2[i] = arr3[i] = n - i;
        selection_sort(arr, n); int sel_worst = counter;
        bubble_sort(arr2, n);   int bub_worst = counter;
        insertion_sort(arr3, n);int ins_worst = counter;

        // Average Case (random)
        for (int i = 0; i < n; i++) arr[i] = arr2[i] = arr3[i] = rand() % n;
        selection_sort(arr, n); int sel_avg = counter;
        bubble_sort(arr2, n);   int bub_avg = counter;
        insertion_sort(arr3, n);int ins_avg = counter;

        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               n, sel_best, sel_worst, sel_avg,
               bub_best, bub_worst, bub_avg,
               ins_best, ins_worst, ins_avg);

        fprintf(fp, "%d %d %d %d %d %d %d %d %d %d\n",
                n, sel_best, sel_worst, sel_avg,
                bub_best, bub_worst, bub_avg,
                ins_best, ins_worst, ins_avg);
    }
    fclose(fp);

    // ===== gnuplot script =====
    FILE *gp = fopen("plot_sort.gnu", "w");
    fprintf(gp,
        "set title \"Sorting Algorithms: Best, Worst, Average\"\n"
        "set xlabel \"Input size (n)\"\n"
        "set ylabel \"Step Count\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"sort_data.dat\" using 1:2 title \"Selection (Best)\", \\\n"
        "     \"sort_data.dat\" using 1:3 title \"Selection (Worst)\", \\\n"
        "     \"sort_data.dat\" using 1:4 title \"Selection (Avg)\", \\\n"
        "     \"sort_data.dat\" using 1:5 title \"Bubble (Best)\", \\\n"
        "     \"sort_data.dat\" using 1:6 title \"Bubble (Worst)\", \\\n"
        "     \"sort_data.dat\" using 1:7 title \"Bubble (Avg)\", \\\n"
        "     \"sort_data.dat\" using 1:8 title \"Insertion (Best)\", \\\n"
        "     \"sort_data.dat\" using 1:9 title \"Insertion (Worst)\", \\\n"
        "     \"sort_data.dat\" using 1:10 title \"Insertion (Avg)\"\n"
        "set term png size 1000,600\n"
        "set output \"sort_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    system("gnuplot plot_sort.gnu");
    printf("\n✅ Graph window opened and also saved as 'sort_plot.png'\n");
}

int main() {
    int mode;
    printf("\n=== MENU ===\n");
    printf("1. Run Sorting Tester (interactive)\n");
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