#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
int graph[MAX][MAX], visited[MAX];
int n, steps;

// ===== DFS Function =====
void dfs(int v) {
    visited[v] = 1;
    for (int i = 0; i < n; i++) {
        steps++;
        if (graph[v][i] && !visited[i]) {
            dfs(i);
        }
    }
}

// ===== Cycle Detection =====
int isCyclicUtil(int v, int parent) {
    visited[v] = 1;
    for (int i = 0; i < n; i++) {
        steps++;
        if (graph[v][i]) {
            if (!visited[i]) {
                if (isCyclicUtil(i, v))
                    return 1;
            } else if (i != parent) {
                return 1;
            }
        }
    }
    return 0;
}

int isCyclic() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    for (int u = 0; u < n; u++) {
        if (!visited[u]) {
            if (isCyclicUtil(u, -1))
                return 1;
        }
    }
    return 0;
}

// ===== Tester =====
void tester() {
    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    for (int i = 0; i < n; i++) visited[i] = 0;
    steps = 0;

    printf("Connected Components:\n");
    int comp = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            comp++;
            printf("Component %d: ", comp);
            dfs(i);
            for (int j = 0; j < n; j++) {
                if (visited[j]) printf("%d ", j);
            }
            printf("\n");
        }
    }
    if (comp == 1) printf("Graph is connected\n");
    else printf("Graph is not connected\n");

    for (int i = 0; i < n; i++) visited[i] = 0;
    if (isCyclic())
        printf("Graph contains a cycle\n");
    else
        printf("Graph is acyclic\n");

    printf("Steps taken = %d\n", steps);
}

// ===== Analysis =====
void analysis() {
    FILE *fp = fopen("dfs_data.txt", "w");
    if (!fp) { printf("Cannot create data file\n"); return; }

    printf("V\tBest\tWorst\n");
    for (int V = 10; V <= 80; V += 10) {
        n = V;

        // === Best Case: Empty graph ===
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = 0;
        for (int i = 0; i < n; i++) visited[i] = 0;
        steps = 0;
        dfs(0);
        int best = steps;

        // === Worst Case: Complete graph ===
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = (i != j);
        for (int i = 0; i < n; i++) visited[i] = 0;
        steps = 0;
        dfs(0);
        int worst = steps;

        printf("%d\t%d\t%d\n", V, best, worst);
        fprintf(fp, "%d %d %d\n", V, best, worst);
    }
    fclose(fp);

    // Create gnuplot script
    FILE *gp = fopen("dfs_plot.gnu", "w");
    if (!gp) { printf("Cannot create gnuplot script\n"); return; }

    fprintf(gp,
        "set title \"DFS: Best vs Worst Case\"\n"
        "set xlabel \"Vertices\"\n"
        "set ylabel \"Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"dfs_data.txt\" using 1:2 title \"Best Case\" lc rgb \"blue\", \\\n"
        "     \"dfs_data.txt\" using 1:3 title \"Worst Case\" lc rgb \"red\"\n"
        "set term png size 1000,600\n"
        "set output \"dfs_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    // Run gnuplot automatically
    system("gnuplot dfs_plot.gnu");
}

// ===== Main =====
int main() {
    int choice;
    printf("1. Tester (Connectivity & Cycle)\n2. Analysis with Graph\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) tester();
    else if (choice == 2) analysis();
    else printf("Invalid choice\n");

    return 0;
}
