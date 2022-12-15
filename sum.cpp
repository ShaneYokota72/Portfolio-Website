#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
  // Get the two numbers entered by the user
  int num1 = atoi(getenv("num1"));
  int num2 = atoi(getenv("num2"));

  // Calculate the sum of the two numbers
  int sum = num1 + num2;

  // Print the result to the screen
  cout << "The sum of " << num1 << " and " << num2 << " is " << sum << endl;

  return 0;
}
