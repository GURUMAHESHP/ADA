#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

int graph[MAX][MAX], visited[MAX], stack[MAX];
int n, steps, top;

// ===== DFS for Topological Sort =====
void dfsTopo(int v) {
    visited[v] = 1;
    for (int i = 0; i < n; i++) {
        steps++;
        if (graph[v][i] && !visited[i]) {
            dfsTopo(i);
        }
    }
    stack[top++] = v; // push onto stack after visiting all neighbors
}

// ===== Topological Sort =====
void topoSort() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    top = 0;
    steps = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) dfsTopo(i);
    }

    printf("Topological Order: ");
    for (int i = top - 1; i >= 0; i--) printf("%d ", stack[i]);
    printf("\nSteps taken = %d\n", steps);
}

// ===== Tester =====
void tester() {
    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (%d x %d) for directed graph:\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    topoSort();
}

// ===== Analysis =====
void analysis() {
    FILE *fp = fopen("topo_data.txt", "w");
    if (!fp) { printf("Cannot create data file\n"); return; }

    printf("V\tBest\tWorst\n");
    for (int V = 10; V <= 80; V += 10) {
        n = V;

        // === Best Case: Linear chain (0->1->2->...->V-1) ===
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = 0;
        for (int i = 0; i < n - 1; i++) graph[i][i + 1] = 1;

        steps = 0;
        topoSort();
        int best = steps;

        // === Worst Case: Fully connected DAG (i->j for i<j) ===
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = (i < j) ? 1 : 0;

        steps = 0;
        topoSort();
        int worst = steps;

        printf("%d\t%d\t%d\n", V, best, worst);
        fprintf(fp, "%d %d %d\n", V, best, worst);
    }
    fclose(fp);

    // ===== Create gnuplot script =====
    FILE *gp = fopen("topo_plot.gnu", "w");
    if (!gp) { printf("Cannot create gnuplot script\n"); return; }

    fprintf(gp,
        "set title \"DFS Topological Sort: Best vs Worst Case\"\n"
        "set xlabel \"Vertices\"\n"
        "set ylabel \"Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"topo_data.txt\" using 1:2 title \"Best Case\" lc rgb \"blue\", \\\n"
        "     \"topo_data.txt\" using 1:3 title \"Worst Case\" lc rgb \"red\"\n"
        "set term png size 1000,600\n"
        "set output \"topo_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    system("gnuplot topo_plot.gnu");
}

// ===== Main =====
int main() {
    int choice;
    printf("1. Tester (Topological Sort)\n2. Analysis with Graph\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) tester();
    else if (choice == 2) analysis();
    else printf("Invalid choice\n");

    return 0;
}
