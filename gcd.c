#include <stdio.h>
#include <stdlib.h>

// Step counters for analysis
int count1, count2, count3;

// Euclid’s algorithm (division method)
int euclid(int num1, int num2)
{
    count1 = 0;
    while (num2 != 0)
    {
        count1++;
        int temp = num2;
        num2 = num1 % num2;
        num1 = temp;
    }
    if (count1 == 0) count1 = 1; // ensure at least 1 step
    return num1;
}

// Consecutive Integer Checking Method
int cicm(int num1, int num2)
{
    count2 = 0;
    int min = (num1 < num2) ? num1 : num2;
    while (min > 0)
    {
        count2++;
        if (num1 % min == 0 && num2 % min == 0)
            break;
        min--;
    }
    return min;
}

// Modified Euclid’s algorithm (subtraction method)
int modified(int num1, int num2)
{
    count3 = 0;
    while (num1 != num2)
    {
        count3++;
        if (num1 > num2)
            num1 = num1 - num2;
        else
            num2 = num2 - num1;
    }
    count3++; // final step
    return num1;
}

void run_tester()
{
    int choice, m, n, gcd;

    while (1)
    {
        printf("\nGCD\n");
        printf("1. Euclid\n");
        printf("2. Modified Euclid\n");
        printf("3. Consecutive Integer Method\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        printf("Enter the values M and N: ");
        scanf("%d %d", &m, &n);

        switch (choice)
        {
        case 1:
            gcd = euclid(m, n);
            printf("The GCD is %d\n", gcd);
            break;
        case 2:
            gcd = modified(m, n);
            printf("The GCD is %d\n", gcd);
            break;
        case 3:
            gcd = cicm(m, n);
            printf("The GCD is %d\n", gcd);
            break;
        default:
            printf("Invalid choice!\n");
        }
    }
}

void run_analysis()
{
    FILE *fp = fopen("gcd_data.dat", "w");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "#n Euclid_B Euclid_W CICM_B CICM_W Mod_B Mod_W\n");
    printf("\nn\tEuclid(B)\tEuclid(W)\tCICM(B)\tCICM(W)\tModified(B)\tModified(W)\n");
    printf("-----------------------------------------------------------------------------\n");

    for (int n = 10; n <= 200; n += 10)
    {
        int e_best, e_worst, c_best, c_worst, m_best, m_worst;

        // Best = numbers equal
        euclid(n, n);       e_best = count1;
        cicm(n, n);         c_best = count2;
        modified(n, n);     m_best = count3;

        // Worst = consecutive numbers
        euclid(n, n-1);     e_worst = count1;
        cicm(n, n-1);       c_worst = count2;
        modified(n, n-1);   m_worst = count3;

        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               n, e_best, e_worst, c_best, c_worst, m_best, m_worst);

        fprintf(fp, "%d %d %d %d %d %d %d\n", n, e_best, e_worst, c_best, c_worst, m_best, m_worst);
    }

    fclose(fp);

    // ===== gnuplot script =====
    FILE *gp = fopen("plot_gcd.gnu", "w");
    fprintf(gp,
        "set title \"GCD Algorithms: Best vs Worst Case\"\n"
        "set xlabel \"Input size (n)\"\n"
        "set ylabel \"Number of Steps\"\n"
        "set grid\n"
        "set key outside\n"
        "set style data lines\n"
        "set term qt persist\n"
        "plot \"gcd_data.dat\" using 1:2 title \"Euclid (Best)\", \\\n"
        "     \"gcd_data.dat\" using 1:3 title \"Euclid (Worst)\", \\\n"
        "     \"gcd_data.dat\" using 1:4 title \"CICM (Best)\", \\\n"
        "     \"gcd_data.dat\" using 1:5 title \"CICM (Worst)\", \\\n"
        "     \"gcd_data.dat\" using 1:6 title \"Modified (Best)\", \\\n"
        "     \"gcd_data.dat\" using 1:7 title \"Modified (Worst)\"\n"
        "set term png size 1000,600\n"
        "set output \"gcd_plot.png\"\n"
        "replot\n"
    );
    fclose(gp);

    system("gnuplot plot_gcd.gnu");
    printf("\n✅ Graph window opened and also saved as 'gcd_plot.png'\n");
}

int main()
{
    int mode;
    printf("\n=== MENU ===\n");
    printf("1. Run GCD Tester (interactive)\n");
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