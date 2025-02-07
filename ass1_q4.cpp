#include <iostream>
using namespace std;

void printSpiralPattern(int n) {
    int matrix[n][n] = {0}; // Create an nxn matrix initialized to 0
    int top = 0, bottom = n - 1, left = 0, right = n - 1;
    int num = 1;
    while (num <= n * n) {
        // Fill left to right (→)
        for (int i = left; i <= right && num <= n * n; i++)
            matrix[top][i] = num++;
        top++;  // Move boundary down
        // Fill top to bottom (↓)
        for (int i = top; i <= bottom && num <= n * n; i++)
            matrix[i][right] = num++;
        right--;  // Move boundary left
        // Fill right to left (←)
        for (int i = right; i >= left && num <= n * n; i--)
            matrix[bottom][i] = num++;
        bottom--;  // Move boundary up
        // Fill bottom to top (↑)
        for (int i = bottom; i >= top && num <= n * n; i--)
            matrix[i][left] = num++;
        left++;  // Move boundary right
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}
int main() {
    int n;
    cout << "Enter the size of the spiral (n x n): ";
    cin >> n;
    if (n <= 0) {
        cout << "Invalid size. Enter a positive integer." << endl;
        return 1;
    }
    printSpiralPattern(n);
    return 0;
}