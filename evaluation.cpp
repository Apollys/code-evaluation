// A very general, flexible, and powerful method for evaluating a candidate's
// solution to a programming problem in a fully-automated manner is to take a
// pointer to their solution function, run tests on that function, and return a
// score.  The problem-writer (who is also the evaluation-function-writer) can
// then choose to test the given function in whatever ways desired.  One who
// wants to simply run a few test cases can still do so quite easily, while
// one who wishes to do more advanced things such as scoring based on the time
// complexity of the given solution may also do so.

// Below is a sample implementation of this idea.
// The candidate (the test-taker) writes the function:
//    int solution_func(std::vector<int> input_vector)
// The test-maker writes the function:
//    std::pair<int, int> evaluate(int (*candidate_func)(std::vector<int>))
// The platform designer writes the code to call the evaluate() function,
// in this case, that is the code demonstrated in the main() method.
// In general, the candidate's solution_func has whatever declaration the
// problem specifies, and the parameter to the evaluate function simply matches
// the required solution function declaration.

// Sample problem:
// Write a function that takes a single input parameter of type
// std::vector<int>, computes the sum of the values in the input vector, and
// returns the sum as an int.

#include <iostream>
#include <vector>

// So the candidate would write a function of the form:
int solution_func(std::vector<int> input_vector);

// Then we would evaluate their function by taking a pointer to such a function:
std::pair<int, int> evaluate(int (*candidate_func)(std::vector<int>));

// And finally, this evaluate() function would be called somewhere higher up.

/*****************************************************************************/

// Here is an example of all of this in action:

// In the candidate's submission file, we have...
int solution_func(std::vector<int> input_vector) {
  return 0;
}

// In the evaluation file, we write...

// Not required, just our own helper method for evaluate()
int reference_solution(std::vector<int> input_vector) {
  int sum = 0;
  for (unsigned int i = 0; i < input_vector.size(); i++) {
    sum += input_vector[i];
  }
  return sum;
}

// Evaluation function: returns a pair of (int score, int max_score)
// In this example, we perform a very simple test: check if the candidate's
// function computes the correct output for each input.  However, the framework
// offers very powerful and flexible evaluation capabilities.  For example, we
// could measure the candidate's function's runtime across an array of inputs
// of varying sizes, interpolate the average runtime complexity of their
// algorithm, and then score the function based on its algorithmic time complexity.
std::pair<int, int> evaluate(int (*candidate_solution)(std::vector<int>)) {
  std::vector<std::vector<int>> test_vectors;
  std::vector<int> test_values;
  // Test case 1: Empty, worth 10% of total score
  test_vectors.push_back(std::vector<int>{});
  test_values.push_back(10);
  // Test case 2: Short list, worth 30% of total score
  test_vectors.push_back(std::vector<int>{1, 2, 3, 4, 5, -6, -7});
  test_values.push_back(30);
  // Test case 3: Larger list, worth 60% of total score
  std::vector<int> large_test_vector(100000, 7);
  large_test_vector.push_back(-7654321);
  test_vectors.push_back(large_test_vector);
  test_values.push_back(60);
  // Run candidate solution on each test case
  int candidate_score = 0;
  int max_score = 0;
  for (unsigned int i = 0; i < test_vectors.size(); i++) {
    std::vector<int> test_vector = test_vectors[i];
    int test_value = test_values[i];
    int pass = (candidate_solution(test_vector) == reference_solution(test_vector));
    candidate_score += pass * test_value;
    max_score += test_value;
  }
  return std::make_pair(candidate_score, max_score);
}


// To complete the picture, somewhere else you will have some code to call our
// evaluate() function on the candidate's function and report the results:
int main() {
  std::cout << "Evaluating candidate's solution function..." << std::endl;
  std::pair<int, int> score = evaluate(solution_func);
  std::cout << "Candidate's score: " << score.first << "/" << score.second << std::endl;
  return 0;
}
