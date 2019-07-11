// TODO(you): Include header guards

#include <exception>
#include <memory>
#include <string>

/*
 * Done by z5157656 Choo Yee Hang
 * This file contains the interface of Euclidean Vectors
 * Friend implementation after the class definition
 * Constructors, methods and operations are located in euclidean_vector.cpp
 */

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept { return what_.c_str(); }

 private:
  std::string what_;
};

class EuclideanVector {
 public:
  explicit EuclideanVector(int i);
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v);
  // TODO(you): add more
 private:
  std::unique_ptr<double[]> magnitudes_;
  // TODO(you): add more
};
