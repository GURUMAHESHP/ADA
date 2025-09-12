#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

int graph[MAX][MAX], visited[MAX];
int n, steps;

// ===== BFS Function =====
void bfs(int start) {
    int queue[MAX], front = 0, rear = 0;
    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear) {
        int v = queue[front++];
        for (int i = 0; i < n; i++) {
            steps++;
            if (graph[v][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

// ===== Cycle Detection (BFS-based) =====
int isCyclic() {
    int parent[MAX];
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }

    for (int s = 0; s < n; s++) {
        if (!visited[s]) {
            int queue[MAX], front = 0, rear = 0;
            queue[rear++] = s;
            visited[s] = 1;

            while (front < rear) {
                int v = queue[front++];
                for (int i = 0; i < n; i++) {
                    steps++;
                    if (graph[v][i]) {
                        if (!visited[i]) {
                            visited[i] = 1;
                            queue[rear++] = i;
                            parent[i] = v;
                        } else if (parent[v] != i) {
                            return 1; // Found a back edge
                        }
                    }
                }
            }
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
            bfs(i);
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
    FILE *fp = fopen("bfs_data.txt", "w");
    if (!fp) { printf("Cannot create data file\n"); return; }

    printf("V\tBest\tWorst\n");
    for (int V = 10; V <= 80; V += 10) {
        n = V;

        // Best Case: Empty graph
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = 0;
        for (int i = 0; i < n; i++) visited[i] = 0;
        steps = 0;
        bfs(0);
        int best = steps;

        // Worst Case: Complete graph
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = (i != j);
        for (int i = 0; i < n; i++) visited[i] = 0;
        steps = 0;
        bfs(0);
        int worst = steps;

        printf("%d\t%d\t%d\n", V, best, worst);
        fprintf(fp, "%d %d %d\n", V, best, worst);
    }
    fclose(fp);

    // Create gnuplot script
    FILE *gp = fopen("bfs_plot.gnu", "w");
    if (!gp) { printf("Cannot create gnuplot script\n"); return; }

    fprintf(gp,
        "set title \"BFS: Best vs Worst Case\"\n"
        "set xlabel \"Vertices\"\n"
        "set ylabel \"Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"bfs_data.txt\" using 1:2 title \"Best Case\" lc rgb \"blue\", \\\n"
        "     \"bfs_data.txt\" using 1:3 title \"Worst Case\" lc rgb \"red\"\n"
        "set term png size 1000,600\n"
        "set output \"bfs_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    system("gnuplot bfs_plot.gnu");
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
