#include <stdio.h>
#include <stdlib.h>

// step counters
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
    count3++; // count the final step
    return num1;
}

int main()
{
    FILE *fp;
    fp = fopen("gcd_data.dat", "w");
    if (!fp)
    {
        printf("Error opening file!\n");
        return 1;
    }

    // header
    fprintf(fp, "#n Euclid_B Euclid_W CICM_B CICM_W Mod_B Mod_W\n");

    printf("n\tEuclid(B)\tEuclid(W)\tCICM(B)\t\tCICM(W)\t\tModified(B)\tModified(W)\n");
    printf("---------------------------------------------------------------------------------\n");

    for (int n = 10; n <= 200; n += 10)
    {
        int num1, num2;

        // Best case: numbers equal
        num1 = n; num2 = n;
        euclid(num1, num2);
        int e_best = count1;
        cicm(num1, num2);
        int c_best = count2;
        modified(num1, num2);
        int m_best = count3;

        // Worst case: consecutive numbers (n, n-1)
        num1 = n; num2 = n - 1;
        euclid(num1, num2);
        int e_worst = count1;
        cicm(num1, num2);
        int c_worst = count2;
        modified(num1, num2);
        int m_worst = count3;

        // print to terminal
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               n, e_best, e_worst, c_best, c_worst, m_best, m_worst);

        // save to file
        fprintf(fp, "%d %d %d %d %d %d %d\n", n, e_best, e_worst, c_best, c_worst, m_best, m_worst);
    }

    fclose(fp);

    // ====== GNU PLOTTER ======
    system("gnuplot -persist -e \""
           "set title 'GCD Algorithms: Best vs Worst Case';"
           "set xlabel 'Input size (n)';"
           "set ylabel 'Number of Steps';"
           "set grid;"
           "set key outside;"
           "set style data lines;"
           "plot 'gcd_data.dat' using 1:2 title 'Euclid (Best)',"
           "     'gcd_data.dat' using 1:3 title 'Euclid (Worst)',"
           "     'gcd_data.dat' using 1:4 title 'CICM (Best)',"
           "     'gcd_data.dat' using 1:5 title 'CICM (Worst)',"
           "     'gcd_data.dat' using 1:6 title 'Modified (Best)',"
           "     'gcd_data.dat' using 1:7 title 'Modified (Worst)';"
           "set terminal png size 1000,600;"
           "set output 'gcd_plot.png';"
           "replot\"");

    printf("\n✅ Graph generated! Check 'gcd_plot.png'\n");

    return 0;
}
