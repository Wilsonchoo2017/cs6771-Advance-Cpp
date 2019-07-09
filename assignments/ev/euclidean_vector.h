#ifndef ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
#define ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_

#include <exception>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept { return what_.c_str(); }

 private:
  std::string what_;
};

class EuclideanVector {
 public:
  // Constructors
  explicit EuclideanVector(int i) noexcept : EuclideanVector{i, 0.0} {};
  EuclideanVector(int n_dimension, double magnitude) noexcept;
  EuclideanVector(std::vector<double>::const_iterator it,
                  std::vector<double>::const_iterator end) noexcept;
  EuclideanVector(const EuclideanVector&) noexcept;
  EuclideanVector(EuclideanVector&&) noexcept;
  // Destructor
  ~EuclideanVector() noexcept = default;

  // Operations
  EuclideanVector& operator=(const EuclideanVector&) noexcept;
  EuclideanVector& operator=(EuclideanVector&&) noexcept;
  double& operator[](int i) noexcept;
  double operator[](int i) const noexcept;
  EuclideanVector& operator+=(const EuclideanVector&);
  EuclideanVector& operator-=(const EuclideanVector&);
  EuclideanVector& operator*=(double) noexcept;
  EuclideanVector& operator/=(double);
  explicit operator std::vector<double>() const
      noexcept;  // TODO INVESTIGATE WHETHER I HAVE TO DO WITHOUT CONST
  explicit operator std::list<double>() const noexcept;

  // Methods
  double at(int) const;
  double& at(int);
  int GetNumDimensions() const noexcept;
  double GetEuclideanNorm() const;
  EuclideanVector CreateUnitVector() const;

  // Friends methods
  friend bool operator==(const EuclideanVector&, const EuclideanVector&) noexcept;
  friend bool operator!=(const EuclideanVector&, const EuclideanVector&) noexcept;
  friend EuclideanVector operator+(const EuclideanVector&, const EuclideanVector&);
  friend EuclideanVector operator-(const EuclideanVector&, const EuclideanVector&);
  friend double operator*(const EuclideanVector&, const EuclideanVector&);  // TODO wary about this
  friend EuclideanVector operator*(const EuclideanVector&, double)noexcept;
  friend EuclideanVector operator*(double, const EuclideanVector&)noexcept;
  friend EuclideanVector operator/(const EuclideanVector&, double);
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v);

 private:
  int dimension_;
  std::unique_ptr<double[]> magnitudes_;

  // ======= Definition of Friend functions ========

  friend bool operator==(const EuclideanVector& vec1, const EuclideanVector& vec2) noexcept {
    if (vec1.dimension_ != vec2.dimension_) {
      return false;
    }

    for (int i = 0; i < vec1.dimension_; ++i) {
      if (vec1.magnitudes_[i] != vec2.magnitudes_[i]) {
        return false;
      }
    }

    return true;
  }

  friend bool operator!=(const EuclideanVector& vec1, const EuclideanVector& vec2) noexcept {
    return !(vec1 == vec2);
  }

  friend EuclideanVector operator+(const EuclideanVector& vec1, const EuclideanVector& vec2) {
    if (vec1.dimension_ != vec2.dimension_) {
      throw EuclideanVectorError("Dimensions of LHS(X) and RHS(Y) do not match");
    }
    std::vector<double> tmp;
    tmp.reserve(vec1.dimension_);
    for (int i = 0; i < vec1.dimension_; ++i) {
      tmp.push_back(vec1.magnitudes_[i] + vec2.magnitudes_[i]);
    }
    return EuclideanVector(tmp.begin(), tmp.end());
  }

  friend EuclideanVector operator-(const EuclideanVector& vec1, const EuclideanVector& vec2) {
    if (vec1.dimension_ != vec2.dimension_) {
      throw EuclideanVectorError("Dimensions of LHS(X) and RHS(Y) do not match");
    }
    std::vector<double> tmp;
    tmp.reserve(vec1.dimension_);
    for (int i = 0; i < vec1.dimension_; ++i) {
      tmp.push_back(vec1.magnitudes_[i] - vec2.magnitudes_[i]);
    }
    return EuclideanVector(tmp.begin(), tmp.end());
  }

  // Dot Product
  friend double operator*(const EuclideanVector& vec1, const EuclideanVector& vec2) {
    if (vec2.dimension_ == 0) {
      throw EuclideanVectorError("Invalid vector division by 0");
    }
    std::vector<double> tmp;
    tmp.reserve(vec1.dimension_);
    double result = 0;
    for (int i = 0; i < vec1.dimension_; ++i) {
      result = result + (vec1.magnitudes_[i] + vec2.magnitudes_[i]);
    }
    return result;
  }

  friend EuclideanVector operator*(const EuclideanVector& vec1, double num) noexcept {
    std::vector<double> tmp;
    tmp.reserve(vec1.dimension_);
    for (int i = 0; i < vec1.dimension_; ++i) {
      tmp.push_back(vec1.magnitudes_[i] * num);
    }
    return EuclideanVector(tmp.begin(), tmp.end());
  }

  friend EuclideanVector operator*(double num, const EuclideanVector& vec1) noexcept {
    return vec1 * num;
  }

  friend EuclideanVector operator/(const EuclideanVector& vec1, double num) {
    // Throw Exception
    std::vector<double> tmp;
    tmp.reserve(vec1.dimension_);
    for (int i = 0; i < vec1.dimension_; ++i) {
      tmp.push_back(vec1.magnitudes_[i] / num);
    }
    return EuclideanVector(tmp.begin(), tmp.end());
  }

  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v) {
    os << '[';
    for (int i = 0; i < v.dimension_; i++) {
      os << ' ' << v.magnitudes_[i];
    }
    os << " ]" << '\n';
    return os;
  }
};

#endif  // ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
