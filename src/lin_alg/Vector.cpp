#include "lin_alg/Vector.hpp"
#include <cstdlib>
#include <cstdio>
#include <cmath>

YAFEL_NAMESPACE_OPEN

Vector::Vector() {
  data = new double[default_capacity];
  capacity = default_capacity;
  length = 0;
}

Vector::Vector(unsigned len) {
  data = new double[len];
  capacity = len;
  length = len;
}

Vector::Vector(unsigned len, double val) {
  data = new double[len];
  capacity = len;
  length = len;
  for(unsigned i=0; i<length; ++i) {
    data[i] = val;
  }
}

Vector::Vector(const Vector & src) {
  length = src.length;
  capacity = src.capacity;
  data = new double[capacity];
  for(unsigned i=0; i<length; ++i) {
    data[i] = src.data[i];
  }
}


Vector::Vector(Vector && src) {
  // take resources
  length = src.length;
  capacity = src.capacity;
  data = src.data;
  
  //set src.data to nullptr
  src.data = nullptr;
}


Vector::~Vector() {
  delete[] data;
}

Vector & Vector::operator=(const Vector & rhs) {
  
  if(this == &rhs) {
    return *this;
  }
  
  double *temp_data = new double[rhs.getLength()];
  for(unsigned i=0; i<rhs.getLength(); ++i) {
    temp_data[i] = rhs(i);
  }
  
  delete [] data;
  data = temp_data;
  
  length = rhs.getLength();
  capacity = rhs.getLength();
  
  return *this;
}

Vector & Vector::operator=(Vector &&rhs) {
  
  if(this == &rhs)
    return *this;
  
  //free this->data
  delete [] data;
  
  //steal resources
  length = rhs.length;
  capacity = rhs.capacity;
  data = rhs.data;
  
  //reset rhs
  rhs.data = nullptr;

  return *this;
}


double& Vector::operator()(unsigned i) const {
#ifndef _OPTIMIZED
  if(i >= length || i<0) {
    printf("Attempted to index outside of vector\n");
    exit(1);
  }
#endif
  return data[i];
}

void Vector::append(double val) {
  
  if(length==capacity) {
    resize();
  }

  data[length++] = val;
}

Vector & Vector::operator*=(double a) {

#pragma omp for  
  for(unsigned i=0; i<length; ++i) {
    data[i] *= a;
  }
  return *this;
}

Vector Vector::operator*(double a) const {
  Vector ret(*this);
  ret *= a;
  return ret;
}

Vector operator*(double a, const Vector & rhs) {
  Vector ret(rhs);
  ret *= a;
  return ret;
}

Vector & Vector::operator+=(const Vector & rhs) {
#ifndef _OPTIMIZED
  if(length != rhs.getLength()) {
    perror("Vector::operator+= length mismatch");
    exit(1);
  }
#endif

#pragma omp for
  for(unsigned i=0; i<length; ++i) {
    data[i] += rhs(i);
  }
  
  return *this;
}

Vector Vector::operator+(const Vector & rhs) const {
#ifndef _OPTIMIZED
  if(length != rhs.getLength()) {
    perror("Vector::operator+ length mismatch");
    exit(1);
  }
#endif  

  Vector ret(*this);
  ret += rhs;

  return ret;
}

Vector & Vector::operator-=(const Vector & rhs) {
  *this += -1*rhs;
  return *this;
}

Vector Vector::operator-(const Vector & rhs) const {
#ifndef _OPTIMIZED
  if(length != rhs.getLength()) {
    perror("Vector::operator- length mismatch");
    exit(1);
  }
#endif  

  Vector ret(*this);
  ret -= rhs;
  return ret;
}

void Vector::resize() {
  
  double *temp = data;
  data = new double[2*capacity];
  for(unsigned i=0; i<length; ++i) {
    data[i] = temp[i];
  }
  capacity *= 2;
  delete[] temp;
}

double Vector::dot(const Vector & rhs) const {
#ifndef _OPTIMIZED
  if(this->getLength() != rhs.getLength() ) {
    perror("Vector::dot() dimension mismatch");
    exit(1);
  }
#endif

  double retval = 0.0;

#pragma omp parallel for reduction(+:retval)
  for(unsigned i=0; i<length; ++i) {
    retval += data[i]*rhs(i);
  }
  
  return retval;
}

double Vector::norm() const {

  return std::sqrt((*this).dot(*this));
}

void Vector::print() {
  
  for(unsigned i=0; i<length; ++i) {
    printf("%f\n", data[i]);
  }
  
}

YAFEL_NAMESPACE_CLOSE
