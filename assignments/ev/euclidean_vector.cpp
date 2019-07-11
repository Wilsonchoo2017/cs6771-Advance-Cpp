#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <assert.h>
#include <cmath>
#include <exception>
#include <iterator>
#include <list>
#include <utility>
#include <vector>

EuclideanVector::EuclideanVector(int n_dimension, double magnitude) noexcept
  : dimension_{n_dimension} {
  if (dimension_ > 0) {
    magnitudes_ = std::make_unique<double[]>(n_dimension);
    for (int i = 0; i < n_dimension; ++i) {
      magnitudes_[i] = magnitude;
    }
  } else {
    magnitudes_ = std::make_unique<double[]>(0);
  }
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator start,
                                 std::vector<double>::const_iterator end) noexcept {
  dimension_ = std::distance(start, end);
  if (dimension_ > 0) {
    magnitudes_ = std::make_unique<double[]>(dimension_);
    for (int i = 0; start != end; ++start, ++i) {
      magnitudes_[i] = *start;
    }
  } else {
    magnitudes_ = std::make_unique<double[]>(0);
  }
}

EuclideanVector::EuclideanVector(const EuclideanVector& v) noexcept {
  dimension_ = v.dimension_;
  if (dimension_ > 0) {
    magnitudes_ = std::make_unique<double[]>(dimension_);
    for (int i = 0; i < dimension_; ++i) {
      magnitudes_[i] = v.magnitudes_[i];
    }
  } else {
    magnitudes_ = std::make_unique<double[]>(0);
  }
}

EuclideanVector::EuclideanVector(EuclideanVector&& v) noexcept
  : dimension_{v.dimension_}, magnitudes_{std::move(v.magnitudes_)} {
  v.dimension_ = 0;
}

EuclideanVector& EuclideanVector::operator=(const EuclideanVector& v) noexcept {
  EuclideanVector tmp{v};
  std::swap(tmp, *this);
  return *this;
}

EuclideanVector& EuclideanVector::operator=(EuclideanVector&& v) noexcept {
  dimension_ = v.dimension_;
  v.dimension_ = 0;  // Clean up
  magnitudes_ = std::move(v.magnitudes_);
  return *this;
}

double& EuclideanVector::operator[](int i) noexcept {
  assert(0 <= i || i < dimension_);
  return magnitudes_[i];
}

double EuclideanVector::operator[](int i) const noexcept {
  assert(0 <= i || i < dimension_);
  return magnitudes_[i];
}

EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& v) {
  if (this->dimension_ != v.dimension_) {
    throw EuclideanVectorError("Dimensions of LHS(X) and RHS(Y) do not match");
  }
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] += v.magnitudes_[i];
  }
  return *this;
}

EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& v) {
  if (this->dimension_ != v.dimension_) {
    throw EuclideanVectorError("Dimensions of LHS(X) and RHS(Y) do not match");
  }
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
  if (num == 0) {
    throw EuclideanVectorError("Invalid vector division by 0");
  }
  for (int i = 0; i < dimension_; ++i) {
    magnitudes_[i] /= num;
  }
  return *this;
}

EuclideanVector::operator std::vector<double>() const noexcept {
  std::vector<double> vec{};
  vec.reserve(dimension_);
  for (int i = 0; i < dimension_; ++i) {
    vec.push_back(magnitudes_[i]);
  }
  return vec;
}

EuclideanVector::operator std::list<double>() const noexcept {
  std::list<double> lst{};
  for (int i = 0; i < dimension_; ++i) {
    lst.push_back(magnitudes_[i]);
  }
  return lst;
}

double EuclideanVector::at(int i) const {
  if (i < 0 || i >= dimension_) {
    throw EuclideanVectorError("Index X is not valid for this EuclideanVector object");
  }
  return magnitudes_[i];
}

double& EuclideanVector::at(int i) {
  if (i < 0 || i >= dimension_) {
    throw EuclideanVectorError("Index X is not valid for this EuclideanVector object");
  }
  return magnitudes_[i];
}

int EuclideanVector::GetNumDimensions() const noexcept {
  return dimension_;
}

double EuclideanVector::GetEuclideanNorm() const {
  if (dimension_ == 0) {
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a norm");
  }
  double result = 0;
  for (int i = 0; i < dimension_; ++i) {
    result += std::pow(magnitudes_[i], 2.0);
  }
  return std::sqrt(result);
}

EuclideanVector EuclideanVector::CreateUnitVector() const {
  if (dimension_ == 0) {
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a unit vector");
  }

  if (this->GetEuclideanNorm() == 0) {
    throw EuclideanVectorError("EuclideanVector with euclidean normal of 0 does not have a "
                               "unit vector");
  }

  EuclideanVector v{*this};
  double norm = this->GetEuclideanNorm();
  for (int i = 0; i < dimension_; ++i) {
    v.magnitudes_[i] /= norm;
  }
  return v;
}
