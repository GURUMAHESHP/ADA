#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

int graph[MAX][MAX], indegree[MAX];
int n, steps;

// ===== Source Removal Topological Sort =====
void topoSourceRemoval() {
    int topo[MAX], top = 0;
    int queue[MAX], front = 0, rear = 0;

    // Calculate indegrees
    for (int i = 0; i < n; i++) {
        indegree[i] = 0;
        for (int j = 0; j < n; j++) {
            steps++;
            if (graph[j][i]) indegree[i]++;
        }
    }

    // Enqueue all source vertices (indegree 0)
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) queue[rear++] = i;
    }

    while (front < rear) {
        int v = queue[front++];
        topo[top++] = v;

        for (int i = 0; i < n; i++) {
            steps++;
            if (graph[v][i]) {
                indegree[i]--;
                if (indegree[i] == 0) queue[rear++] = i;
            }
        }
    }

    if (top != n) {
        printf("Graph has a cycle, topological order not possible\n");
        return;
    }

    printf("Topological Order: ");
    for (int i = 0; i < n; i++) printf("%d ", topo[i]);
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

    steps = 0;
    topoSourceRemoval();
}

// ===== Analysis =====
void analysis() {
    FILE *fp = fopen("source_removal_data.txt", "w");
    if (!fp) { printf("Cannot create data file\n"); return; }

    printf("V\tBest\tWorst\n");
    for (int V = 10; V <= 80; V += 10) {
        n = V;

        // === Best Case: Linear chain 0->1->2->...->V-1 ===
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = 0;
        for (int i = 0; i < n - 1; i++) graph[i][i + 1] = 1;

        steps = 0;
        topoSourceRemoval();
        int best = steps;

        // === Worst Case: Fully connected DAG (i->j for i<j) ===
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = (i < j) ? 1 : 0;

        steps = 0;
        topoSourceRemoval();
        int worst = steps;

        printf("%d\t%d\t%d\n", V, best, worst);
        fprintf(fp, "%d %d %d\n", V, best, worst);
    }
    fclose(fp);

    // ===== Create gnuplot script =====
    FILE *gp = fopen("source_removal_plot.gnu", "w");
    if (!gp) { printf("Cannot create gnuplot script\n"); return; }

    fprintf(gp,
        "set title \"Source Removal Topological Sort: Best vs Worst Case\"\n"
        "set xlabel \"Vertices\"\n"
        "set ylabel \"Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"source_removal_data.txt\" using 1:2 title \"Best Case\" lc rgb \"blue\", \\\n"
        "     \"source_removal_data.txt\" using 1:3 title \"Worst Case\" lc rgb \"red\"\n"
        "set term png size 1000,600\n"
        "set output \"source_removal_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    system("gnuplot source_removal_plot.gnu");
}

// ===== Main =====
int main() {
    int choice;
    printf("1. Tester (Source Removal Topological Sort)\n2. Analysis with Graph\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) tester();
    else if (choice == 2) analysis();
    else printf("Invalid choice\n");

    return 0;
}
