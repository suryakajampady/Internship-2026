#include<stdio.h>
int main()
{
    int a[10][10], b[10][10], c[10][10], d[10][10], i, j, k, n;
    float det = 0;

    printf("Enter the order of the matrix: ");
    scanf("%d", &n);

    printf("Enter the elements of the matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &a[i][j]);

    // Calculate determinant
    for (i = 0; i < n; i++)
        det += (a[0][i] * (a[1][(i + 1) % n] * a[2][(i + 2) % n] - a[1][(i + 2) % n] * a[2][(i + 1) % n]));

    if (det == 0)
    {
        printf("Inverse not possible as determinant is zero.\n");
        return 0;
    }

    // Calculate inverse using adjoint method
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            // Calculate cofactor
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            b[i][j] = sign * (a[(i + 1) % n][(j + 1) % n] * a[(i + 2) % n][(j + 2) % n] - a[(i + 1) % n][(j + 2) % n] * a[(i + 2) % n][(j + 1) % n]);
        }
    }

    // Transpose of cofactor matrix to get adjoint
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            c[j][i] = b[i][j];

    // Divide by determinant to get inverse
    printf("Inverse of the matrix is:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%f ", c[i][j] / det);
        printf("\n");
    }
    return 0;
}       