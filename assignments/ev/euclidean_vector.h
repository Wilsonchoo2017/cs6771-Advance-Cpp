#ifndef ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
#define ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_

#include <exception>
#include <list>
#include <memory>
#include <string>
#include <vector>

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept{ return what_.c_str(); }
 private:
  std::string what_;
};

class EuclideanVector {
 public:
  // Constructors
  explicit EuclideanVector(int i) noexcept : EuclideanVector{i, 0.0} {};
  EuclideanVector(int n_dimension, double magnitude) noexcept;
  EuclideanVector(std::vector<double>::const_iterator it, std::vector<double>::const_iterator end) noexcept;
  EuclideanVector (const EuclideanVector &) noexcept;
  EuclideanVector(EuclideanVector&&) noexcept;
  // Destructor
  ~EuclideanVector() noexcept = default;

  // Operations
  EuclideanVector& operator=(const EuclideanVector&) noexcept ;
  EuclideanVector& operator=(EuclideanVector&&) noexcept ;
  double& operator[] (int i) noexcept ;
  double operator[] (int i) const noexcept ;
  EuclideanVector& operator+=(const EuclideanVector&);
  EuclideanVector& operator-=(const EuclideanVector&);
  EuclideanVector& operator*=(double) noexcept ;
  EuclideanVector& operator/=(double);
  explicit operator std::vector<double>() const noexcept ; //TODO INVESTIGATE WHETHER I HAVE TO DO WITHOUT CONST
  explicit operator std::list<double>() const noexcept ;

  // Methods
  double at(int) const;
  double& at(int);
  int GetNumDimensions() const noexcept ;
  double GetEuclideanNorm() const;
  EuclideanVector CreateUnitVector() const;

 private:
  int dimension_;
  std::unique_ptr<double[]> magnitudes_;

  // Friends methods
  friend bool operator==(const EuclideanVector&, const EuclideanVector&) noexcept ;
  friend bool operator!=(const EuclideanVector&, const EuclideanVector&) noexcept ;
  friend EuclideanVector operator+(const EuclideanVector&, const EuclideanVector&);
  friend EuclideanVector operator-(const EuclideanVector&, const EuclideanVector&);
  friend double operator*(const EuclideanVector&, const EuclideanVector&); // TODO wary about this
  friend EuclideanVector operator*(const EuclideanVector&, double) noexcept ;
  friend EuclideanVector operator*(double, const EuclideanVector&) noexcept ;
  friend EuclideanVector operator/(const EuclideanVector&, double);
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v);
};
// Todo methods that do not throw expcetion mark it as noexcept
#endif // ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
