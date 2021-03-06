#ifndef _MODELUTIL_H_
#define _MODELUTIL_H_

#include <vector>
#include "base/SimpleMatrix.h"
#include "libsrc/MathMatrix.h"
#include "libsrc/MathVector.h"

inline void copy(const std::vector<double>& in, Vector* o) {
  Vector& out = *o;
  out.Dimension(in.size());
  int n = in.size();
  for (int i = 0; i < n; ++i) {
    out[i] = in[i];
  }
}

inline void copyPhenotype(Matrix& in, Vector* o) {
  Vector& out = *o;
  out.Dimension(in.rows);
  for (int i = 0; i < in.rows; ++i) {
    out[i] = in[i][0];
  }
}

inline void copyPhenotype(const SimpleMatrix& in, Vector* o) {
  Vector& out = *o;
  const int nr = in.nrow();
  out.Dimension(nr);
  for (int i = 0; i < nr; ++i) {
    out[i] = in[i][0];
  }
}

inline void copyVectorToMatrixColumn(Vector& v, SimpleMatrix* out, int column) {
  SimpleMatrix& o = *out;
  const int n = v.Length();
  assert(o.nrow() == n);
  assert(o.ncol() > column);

  for (int i = 0; i < n; ++i) {
    o[i][column] = v[i];
  }
}

/**
 * copy @param in to @param o, with first column being intercept
 */
inline void copyGenotypeWithIntercept(Matrix& in, Matrix* o) {
  Matrix& out = *o;
  out.Dimension(in.rows, in.cols + 1);
  for (int i = 0; i < in.rows; ++i) {
    out[i][0] = 1.0;
    for (int j = 0; j < in.cols; ++j) {
      out[i][j + 1] = in[i][j];
    }
  }
  out.SetColumnLabel(0, "Intercept");
  for (int i = 0; i < in.cols; ++i)
    out.SetColumnLabel(i + 1, in.GetColumnLabel(i));
}

/**
 * copy vector of one, @param in and @param cov to @param o (essentially: o =
 * cbind(1, in, cov))
 */
inline void copyGenotypeWithCovariateAndIntercept(Matrix& in, Matrix& cov,
                                                  Matrix* o) {
  Matrix& out = *o;
  out.Dimension(in.rows, 1 + in.cols + cov.cols);

  for (int i = 0; i < in.rows; ++i) {
    out[i][0] = 1.0;
  }
  out.SetColumnLabel(0, "Intercept");

  for (int j = 0; j < in.cols; ++j) {
    for (int i = 0; i < in.rows; ++i) {
      out[i][1 + j] = in[i][j];
    }
    out.SetColumnLabel(1 + j, in.GetColumnLabel(j));
  }

  for (int j = 0; j < cov.cols; ++j) {
    for (int i = 0; i < cov.rows; ++i) {
      out[i][1 + j + in.cols] = cov[i][j];
    }
    out.SetColumnLabel(1 + j + in.cols, cov.GetColumnLabel(j));
  }
}

/**
 * copy intercept and @param cov to @param o with its first column equaling to
 * 1.0
 */
inline void copyCovariateAndIntercept(int n, Matrix& cov, Matrix* o) {
  if (cov.cols == 0) {
    (*o).Dimension(n, 1);
    for (int i = 0; i < n; ++i) {
      (*o)[i][0] = 1.0;
    }
    (*o).SetColumnLabel(0, "Intercept");
    return;
  }

  (*o).Dimension(n, 1 + cov.cols);
  for (int i = 0; i < n; ++i) {
    (*o)[i][0] = 1.0;
    for (int j = 0; j < cov.cols; ++j) {
      (*o)[i][j + 1] = cov[i][j];
    }
  }
  (*o).SetColumnLabel(0, "Intercept");
  for (int j = 0; j < cov.cols; ++j) {
    (*o).SetColumnLabel(j + 1, cov.GetColumnLabel(j));
  }
  return;
}

inline void copyCovariateAndIntercept(int n, SimpleMatrix& cov, Matrix* o) {
  const int nr = cov.nrow();
  const int nc = cov.ncol();

  if (nc == 0) {
    (*o).Dimension(n, 1);
    for (int i = 0; i < n; ++i) {
      (*o)[i][0] = 1.0;
    }

    (*o).SetColumnLabel(0, "Intercept");
    return;
  }
  if (n != nr) {
    assert(false);
    return;
  }

  (*o).Dimension(n, 1 + nc);
  for (int i = 0; i < n; ++i) {
    (*o)[i][0] = 1.0;
    for (int j = 0; j < nc; ++j) {
      (*o)[i][j + 1] = cov[i][j];
    }
  }
  (*o).SetColumnLabel(0, "Intercept");
  for (int j = 0; j < cov.ncol(); ++j) {
    (*o).SetColumnLabel(j + 1, cov.getColName()[j].c_str());
  }
  return;
}

#endif /* _MODELUTIL_H_ */
