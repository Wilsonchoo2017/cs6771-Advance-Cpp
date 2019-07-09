#include "assignments/ev/euclidean_vector.h"

#include <assert.h>
#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <cmath>
#include <iterator>
#include <list>
#include <vector>

EuclideanVector::EuclideanVector(int n_dimension, double magnitude) noexcept : dimension_{n_dimension} {
  magnitudes_ = std::make_unique<double[]>(n_dimension);
  for (int i = 0; i < n_dimension; ++i) {
    magnitudes_[i] = magnitude;
  }
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator start,
    std::vector<double>::const_iterator end) noexcept {
  dimension_ = std::distance(start, end);
  magnitudes_ = std::make_unique<double[]>(dimension_);
  for (int i = 0; start != end; ++start, ++i) {
    magnitudes_[i] = *start;
  }
}

EuclideanVector::EuclideanVector(const EuclideanVector & v) noexcept {
  dimension_ = v.dimension_;
  magnitudes_ = std::make_unique<double[]>(dimension_);
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] = v.magnitudes_[i];
  }
}

EuclideanVector::EuclideanVector(EuclideanVector&& v) noexcept :
dimension_{v.dimension_}, magnitudes_{std::move(v.magnitudes_)} {
    v.dimension_ = 0;
}

std::ostream& operator<<(std::ostream& os, const EuclideanVector& v) {
  os << '[';
  for (int i = 0; i < v.dimension_; i++) {
    os << ' ' << v.magnitudes_[i];
  }
  os << " ]" << '\n';
  return os;
}

EuclideanVector& EuclideanVector::operator=(const EuclideanVector& v) noexcept {
  dimension_ = v.dimension_;
  magnitudes_ = std::make_unique<double[]>(dimension_);
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] = v.magnitudes_[i];
  }
  return *this;
}

EuclideanVector& EuclideanVector::operator=(EuclideanVector&& v) noexcept {
  dimension_ = v.dimension_;
  v.dimension_ = 0; // Clean up
  magnitudes_ = std::move(v.magnitudes_);
  return *this;
}

double& EuclideanVector::operator[](int i) noexcept {
  assert(i < 0 || i >= dimension_);
  return magnitudes_[i];
}

double EuclideanVector::operator[](int i) const noexcept {
  assert(i < 0 || i >= dimension_);
  return magnitudes_[i];
}

EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& v) {
  // ToDo Throw Exception
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] += v.magnitudes_[i];
  }
  return *this;
}

EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& v) {
  // ToDo Throw Exception
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] += v.magnitudes_[i];
  }
  return *this;
}

EuclideanVector& EuclideanVector::operator*=(const double num) noexcept {
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] *= num;
  }
  return *this;
}

EuclideanVector& EuclideanVector::operator/=(const double num) {
  // ToDo Throw Exception
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] /= num;
  }
  return *this;
}


EuclideanVector::operator std::vector<double>() const noexcept {
  std::vector<double> vec;
  vec.reserve(dimension_);
  for (int i = 0; i < dimension_; ++i) {
    vec.push_back(magnitudes_[i]);
  }
  return vec;
}

EuclideanVector::operator std::list<double>() const noexcept {
  std::list<double> lst;
  for (int i = 0; i < dimension_; ++i) {
    lst.push_back(magnitudes_[i]);
  }
  return lst;
}


double EuclideanVector::at(int i) const {
  // Todo Throw Exception
  return magnitudes_[i];
}


double& EuclideanVector::at(int i) {
  // Todo Throw Exception
  return magnitudes_[i];
}

int EuclideanVector::GetNumDimensions() const noexcept {
  return dimension_;
}

double EuclideanVector::GetEuclideanNorm() const {
  // Todo Throw Exception
  double result = 0;
  for (int i = 0; i < dimension_; ++i) {
    result += std::pow(magnitudes_[i], 2.0);
  }
  return std::sqrt(result);
}


EuclideanVector EuclideanVector::CreateUnitVector() const {
  // Todo Throw Exception
  EuclideanVector v{*this};
  double norm = this->GetEuclideanNorm();
  for (int i = 0; i < dimension_; ++i) {
    v.magnitudes_[i] /= norm;
  }
  return v;
}

