#include <iostream>
#include <algorithm>  // For sorting
using namespace std;

void reverseArray(int arr[], int size) {
    cout << "Reversed array: ";
    for (int i = size - 1; i >= 0; i--) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
void findSecondLargestAndSmallest(int arr[], int size) {
    if (size < 2) {
        cout << "Array must have at least two elements to find second largest and second smallest." << endl;
        return;
    }
    sort(arr, arr + size);
    int secondSmallest = -1, secondLargest = -1;
    for (int i = 1; i < size; i++) {
        if (arr[i] != arr[0]) {
            secondSmallest = arr[i];
            break;
        }
    }
    for (int i = size - 2; i >= 0; i--) {
        if (arr[i] != arr[size - 1]) {
            secondLargest = arr[i];
            break;
        }
    }
    if (secondSmallest != -1)
        cout << "Second smallest element: " << secondSmallest << endl;
    else
        cout << "No second smallest element (all elements are the same)." << endl;
    if (secondLargest != -1)
        cout << "Second largest element: " << secondLargest << endl;
    else
        cout << "No second largest element (all elements are the same)." << endl;
}
int main() {
    int size;
    cout << "Enter the size of the array: ";
    cin >> size;

    if (size <= 0) {
        cout << "Invalid size. Please enter a positive integer." << endl;
        return 1;
    }
    int arr[size];
    cout << "Enter " << size << " elements: ";
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }
    reverseArray(arr, size);
    findSecondLargestAndSmallest(arr, size);
    return 0;
}