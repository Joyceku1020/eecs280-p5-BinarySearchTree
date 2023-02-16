// Project UID db1f506d06d84ab787baf250c265e24e

#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include "csvstream.h"
#include <cmath>

using namespace std;

// EFFECTS: Return a set of unique whitespace delimited words.x
set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word)
  {
    words.insert(word);
  }
  return words;
}

class Classifier
{
private:
  map<string, int> label_count;
  map<string, int> word_count;
  map<pair<string, string>, int> word_count_given_label;
  pair<string, string> label_word;
  int numPosts = 0;
  int numUniqueWords = 0;
  double max_prob_score = INT_MIN;
  double cur_prob_score = 0;

public:
  Classifier() {}

  void process_train_file(const string & train_file, bool debug) {
    map<string, string> row;
    string label = "";
    string content = "";
    csvstream csvin(train_file);

    if (debug) {
        cout << "training data:" << endl;
    }

    while (csvin >> row) {
        label = row["tag"];
        content = row["content"];
        label_count[label]++; // 4
        set<string> s;
        s = unique_words(row["content"]);
        set<string>::iterator itr;

        if (debug) {
            cout << "  label = " << label << ", content = " << content << endl;
        }

        for (itr = s.begin(); itr != s.end(); itr++)
        {
            word_count[*itr]++; // 3

            label_word.first = label;
            label_word.second = *itr;
            word_count_given_label[label_word]++; // 5
        }
      numPosts++; // 1
    }

    numUniqueWords = word_count.size();

    cout << "trained on " << numPosts << " samples" << endl;

    if (debug) { 
        cout << "vocabulary size = " << numUniqueWords << endl; 
    }

    cout << endl;

  }

  void run_test(const string &test_file, bool debug) {
    csvstream csvin_test(test_file);
    string correct_label = "";
    string predicted_label = "";
    map<string, string> row;
    int num_correct = 0;
    int total_posts = 0;

    if (debug) {
        cout << "classes:" << endl;
        print_classes();
        cout << "classifier parameters:" << endl;
    }

    if (debug) {
        print_classifier_parameters();
        cout << endl;
    }
    cout << "test data:" << endl;
    
    while (csvin_test >> row) {
        correct_label = row["tag"];

        set<string> test_words;
        test_words = unique_words(row["content"]);

        map<string, int>::iterator it;
        for (it = label_count.begin(); it != label_count.end(); it++) {
            cur_prob_score = calculate_probability_score(it, test_words, debug);
            if(cur_prob_score > max_prob_score) {
              max_prob_score = cur_prob_score;
              predicted_label = it -> first;
            }
        }

        cout << "  correct = " << correct_label << ", predicted = " << predicted_label 
          << ", log-probability score = " << max_prob_score << endl;
        cout << "  content = " << row["content"] << endl << endl;

        if (correct_label == predicted_label) {
            num_correct++;
        }
        total_posts++;

    }
    cout << "performance: " << num_correct << " / " << total_posts << " posts predicted correctly\n" << endl;
  }

  double calculate_probability_score(map<string, int>::iterator it, set<string> test_words, bool debug) {
    string label = it -> first;
    double labelcount = it -> second;
    double logprior_prob = log(labelcount/numPosts);
    double loglikelihood_prob = 0;
    double numerator = 0;
    double denominator = 0;
    set<string>::iterator itr;
    pair<string, string> pair;
    

    for (itr = test_words.begin(); itr != test_words.end(); itr++) {
        pair.first = label;
        pair.second = *itr;

        if (word_count.find(*itr) == word_count.end()) {
            numerator = 1;
            denominator = numPosts;

        }
        else if (word_count_given_label.find(pair) == word_count_given_label.end()) {
            numerator = word_count.find(*itr) -> second;
            denominator = numPosts;
        }
        else {
            numerator = (word_count_given_label.find(pair)) -> second;
            denominator = (label_count.find(label)) -> second;
        }

        loglikelihood_prob += log(numerator/denominator);
    }

    return logprior_prob + loglikelihood_prob;
 
  }

  void print_classes() {
    map<string, int>::iterator it;
    double logprior = 0;
    double numerator = 0;

    for (it = label_count.begin(); it != label_count.end(); it++) { 
        numerator = it -> second;
        logprior = log(numerator/numPosts);
        cout << "  " << it -> first << ", " << it -> second << 
          " examples, log-prior = " << logprior << endl;
    }
  }

  void print_classifier_parameters() {
    map<pair<string, string>, int>::iterator itr;
    string label = "";
    string word = "";
    double loglikelihood_prob = 0;
    double numerator = 0;
    double denominator = 0;

    for (itr = word_count_given_label.begin(); itr != word_count_given_label.end(); itr++) {
        label = (itr -> first).first;
        word = (itr -> first).second;

        numerator = itr -> second;
        denominator = (label_count.find(label)) -> second;
        

        loglikelihood_prob = log(numerator/denominator);

        cout << "  " << label << ":" << word << ", count = " 
              << itr -> second << ", log-likelihood = " << loglikelihood_prob << endl;
    }
  }

};

int main(int argc, char **argv)
{
    cout.precision(3);
    cout << "-----------------START-------------------" << endl;

    if ((!(argc == 3) && !(argc == 4)))
    {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
    string arg(argv[3]);
    if (argc == 4) {
        if (arg != "--debug") {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1; 
        }
    }
    string train_file = argv[1];
    string test_file = argv[2];
    bool debug = (argc == 4);
    Classifier cfr;
    cfr.process_train_file(train_file, debug);
    cfr.run_test(test_file, debug);

    
}

