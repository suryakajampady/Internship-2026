// inverse of a 3X3 matrix
#include <stdio.h>
int main()
{
    float a[3][3], inverse[3][3];
    int i, j;
    float determinant;

    printf("Enter the elements of the 3x3 matrix:\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            scanf("%f", &a[i][j]);
        }
    }

    // Calculate the determinant
    determinant = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) -
                  a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) +
                  a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);

    if (determinant == 0)
    {
        printf("The matrix is singular and cannot be inverted.\n");
        return 0;
    }

    // Calculate the inverse
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            inverse[j][i] = ((a[(i + 1) % 3][(j + 1) % 3] * a[(i + 2) % 3][(j + 2) % 3]) -
                             (a[(i + 1) % 3][(j + 2) % 3] * a[(i + 2) % 3][(j + 1) % 3])) / determinant;
        }
    }

    printf("The inverse of the matrix is:\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            printf("%.2f ", inverse[i][j]);
        }
        printf("\n");
    }

    return 0;
}