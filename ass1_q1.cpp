#include <iostream>
using namespace std;

//check if a number is prime
bool isPrime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}
//next prime number greater than n
int nextPrime(int num) {
    while (true) {
        num++;  // Check the next number
        if (isPrime(num)) return num;
    }
}
//print all factors of a number
void findFactors(int num) {
    cout << "Factors of " << num << ": ";
    for (int i = 1; i <= num; i++) {
     if (num % i == 0) cout << i << " ";
    }
    cout << endl;
}
int main() {
    int n;
    cout << "Enter a positive integer: ";
    cin >> n;
    if (isPrime(n)) {
        cout << n << " is a prime number." << endl;
        cout << "Next prime number: " << nextPrime(n) << endl;
    } else {
        cout << n << " is not a prime number." << endl;
        findFactors(n);
    }
    return 0;
}