#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-8
#define PI 3.14159265358979323846

typedef struct {
   double a, b, c;
   double alpha, beta, gamma;
} Cell;

typedef struct {
   double m[3][3];
} Matrix3;

typedef struct {
   double v[3];
} Vector3;

/* Convert cell parameters to basis matrix (rows are basis vectors) */
void cell_to_basis(const Cell* cell, Matrix3* basis) {
   double alpha_rad = cell->alpha * PI / 180.0;
   double beta_rad = cell->beta * PI / 180.0;
   double gamma_rad = cell->gamma * PI / 180.0;

   double cos_alpha = cos(alpha_rad);
   double cos_beta = cos(beta_rad);
   double cos_gamma = cos(gamma_rad);
   double sin_gamma = sin(gamma_rad);

   /* First basis vector along x-axis */
   basis->m[0][0] = cell->a;
   basis->m[0][1] = 0.0;
   basis->m[0][2] = 0.0;

   /* Second basis vector in xy-plane */
   basis->m[1][0] = cell->b * cos_gamma;
   basis->m[1][1] = cell->b * sin_gamma;
   basis->m[1][2] = 0.0;

   /* Third basis vector */
   basis->m[2][0] = cell->c * cos_beta;
   basis->m[2][1] = cell->c * (cos_alpha - cos_beta * cos_gamma) / sin_gamma;
   basis->m[2][2] = cell->c * sqrt(1.0 - cos_beta * cos_beta -
      pow((cos_alpha - cos_beta * cos_gamma) / sin_gamma, 2));
}

/* Convert basis matrix to cell parameters */
void basis_to_cell(const Matrix3* basis, Cell* cell) {
   double a_vec[3] = { basis->m[0][0], basis->m[0][1], basis->m[0][2] };
   double b_vec[3] = { basis->m[1][0], basis->m[1][1], basis->m[1][2] };
   double c_vec[3] = { basis->m[2][0], basis->m[2][1], basis->m[2][2] };

   cell->a = sqrt(a_vec[0] * a_vec[0] + a_vec[1] * a_vec[1] + a_vec[2] * a_vec[2]);
   cell->b = sqrt(b_vec[0] * b_vec[0] + b_vec[1] * b_vec[1] + b_vec[2] * b_vec[2]);
   cell->c = sqrt(c_vec[0] * c_vec[0] + c_vec[1] * c_vec[1] + c_vec[2] * c_vec[2]);

   double dot_bc = b_vec[0] * c_vec[0] + b_vec[1] * c_vec[1] + b_vec[2] * c_vec[2];
   double dot_ac = a_vec[0] * c_vec[0] + a_vec[1] * c_vec[1] + a_vec[2] * c_vec[2];
   double dot_ab = a_vec[0] * b_vec[0] + a_vec[1] * b_vec[1] + a_vec[2] * b_vec[2];

   cell->alpha = acos(dot_bc / (cell->b * cell->c)) * 180.0 / PI;
   cell->beta = acos(dot_ac / (cell->a * cell->c)) * 180.0 / PI;
   cell->gamma = acos(dot_ab / (cell->a * cell->b)) * 180.0 / PI;
}

/* Compute metric tensor G = B^T * B */
void compute_metric(const Matrix3* basis, Matrix3* G) {
   int i, j, k;
   for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
         G->m[i][j] = 0.0;
         for (k = 0; k < 3; k++) {
            G->m[i][j] += basis->m[k][i] * basis->m[k][j];
         }
      }
   }
}

/* Matrix multiplication: result = A * B */
void matrix_mult(const Matrix3* A, const Matrix3* B, Matrix3* result) {
   int i, j, k;
   Matrix3 temp;
   for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
         temp.m[i][j] = 0.0;
         for (k = 0; k < 3; k++) {
            temp.m[i][j] += A->m[i][k] * B->m[k][j];
         }
      }
   }
   memcpy(result, &temp, sizeof(Matrix3));
}

/* Basis transformation: basis_new = T * basis_old */
void transform_basis(const Matrix3* T, Matrix3* basis) {
   Matrix3 result;
   matrix_mult(T, basis, &result);
   memcpy(basis, &result, sizeof(Matrix3));
}

/* Check if all off-diagonal elements of G are <= 0 */
int is_delaunay_reduced(const Matrix3* G) {
   if (G->m[0][1] > TOLERANCE) return 0;
   if (G->m[0][2] > TOLERANCE) return 0;
   if (G->m[1][2] > TOLERANCE) return 0;
   return 1;
}

/* Find most acute angle (largest positive off-diagonal) */
void find_most_acute(const Matrix3* G, int* max_i, int* max_j) {
   double max_val = -1e10;
   *max_i = -1;
   *max_j = -1;

   if (G->m[0][1] > max_val) {
      max_val = G->m[0][1];
      *max_i = 0;
      *max_j = 1;
   }
   if (G->m[0][2] > max_val) {
      max_val = G->m[0][2];
      *max_i = 0;
      *max_j = 2;
   }
   if (G->m[1][2] > max_val) {
      max_val = G->m[1][2];
      *max_i = 1;
      *max_j = 2;
   }

   if (max_val <= TOLERANCE) {
      *max_i = -1;
      *max_j = -1;
   }
}

/* Selling reduction step - CORRECTED */
void selling_step(Matrix3* basis, int i, int j, Matrix3* transform) {
   Matrix3 G;
   Matrix3 T;
   int k;

   compute_metric(basis, &G);

   /* Initialize T as identity */
   memset(&T, 0, sizeof(Matrix3));
   T.m[0][0] = T.m[1][1] = T.m[2][2] = 1.0;

   /* Replace shorter vector with negative sum: v_i = -(v_i + v_j) */
   if (G.m[i][i] <= G.m[j][j]) {
      for (k = 0; k < 3; k++) {
         basis->m[i][k] = -(basis->m[i][k] + basis->m[j][k]);
      }
      T.m[i][i] = -1.0;
      T.m[i][j] = -1.0;
   } else {
      for (k = 0; k < 3; k++) {
         basis->m[j][k] = -(basis->m[i][k] + basis->m[j][k]);
      }
      T.m[j][j] = -1.0;
      T.m[j][i] = -1.0;
   }

   matrix_mult(&T, transform, transform);
}

/* Delaunay reduction */
void delaunay_reduction(Matrix3* basis, Matrix3* transform) {
   Matrix3 G;
   int iteration, max_i, max_j;

   for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
      compute_metric(basis, &G);

      if (is_delaunay_reduced(&G)) {
         break;
      }

      find_most_acute(&G, &max_i, &max_j);

      if (max_i == -1) {
         break;
      }

      selling_step(basis, max_i, max_j, transform);
   }
}

/* Check if A <= B <= C */
int check_order(double A, double B, double C) {
   return (A <= B + TOLERANCE) && (B <= C + TOLERANCE);
}

/* Reorder basis to satisfy A <= B <= C */
void reorder_basis(Matrix3* basis, double A, double B, double C, Matrix3* transform) {
   double lengths[3] = { A, B, C };
   int indices[3] = { 0, 1, 2 };
   int i, j, temp_idx;
   double temp_len;
   Matrix3 T, temp_basis;

   /* Simple bubble sort */
   for (i = 0; i < 2; i++) {
      for (j = 0; j < 2 - i; j++) {
         if (lengths[j] > lengths[j + 1]) {
            temp_len = lengths[j];
            lengths[j] = lengths[j + 1];
            lengths[j + 1] = temp_len;

            temp_idx = indices[j];
            indices[j] = indices[j + 1];
            indices[j + 1] = temp_idx;
         }
      }
   }

   /* Create permutation matrix */
   memset(&T, 0, sizeof(Matrix3));
   for (i = 0; i < 3; i++) {
      T.m[i][indices[i]] = 1.0;
   }

   /* Apply permutation */
   memcpy(&temp_basis, basis, sizeof(Matrix3));
   for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
         basis->m[i][j] = temp_basis.m[indices[i]][j];
      }
   }

   matrix_mult(&T, transform, transform);
}

/* Check type consistency - CORRECTED */
int check_type(double xi, double eta, double zeta) {
   int sign_xi = (fabs(xi) > TOLERANCE) ? (xi > 0 ? 1 : -1) : 0;
   int sign_eta = (fabs(eta) > TOLERANCE) ? (eta > 0 ? 1 : -1) : 0;
   int sign_zeta = (fabs(zeta) > TOLERANCE) ? (zeta > 0 ? 1 : -1) : 0;

   int positive = (sign_xi > 0) + (sign_eta > 0) + (sign_zeta > 0);
   int negative = (sign_xi < 0) + (sign_eta < 0) + (sign_zeta < 0);

   /* All non-zero should be same sign (all positive or all negative) */
   return (positive == 0 || negative == 0);
}

/* Fix type by ensuring all angles are obtuse - CORRECTED */
void fix_type(Matrix3* basis, double xi, double eta, double zeta, Matrix3* transform) {
   Matrix3 T;
   int k;
   int sign_xi = (fabs(xi) > TOLERANCE) ? (xi > 0 ? 1 : -1) : 0;
   int sign_eta = (fabs(eta) > TOLERANCE) ? (eta > 0 ? 1 : -1) : 0;
   int sign_zeta = (fabs(zeta) > TOLERANCE) ? (zeta > 0 ? 1 : -1) : 0;

   int positive = (sign_xi > 0) + (sign_eta > 0) + (sign_zeta > 0);
   int negative = (sign_xi < 0) + (sign_eta < 0) + (sign_zeta < 0);

   memset(&T, 0, sizeof(Matrix3));
   T.m[0][0] = T.m[1][1] = T.m[2][2] = 1.0;

   /* If there are more positive than negative, negate the positive ones */
   if (positive > negative) {
      if (xi > TOLERANCE) {
         for (k = 0; k < 3; k++) basis->m[2][k] = -basis->m[2][k];
         T.m[2][2] = -1.0;
      }
      if (eta > TOLERANCE) {
         for (k = 0; k < 3; k++) basis->m[1][k] = -basis->m[1][k];
         T.m[1][1] = -1.0;
      }
      if (zeta > TOLERANCE) {
         for (k = 0; k < 3; k++) basis->m[0][k] = -basis->m[0][k];
         T.m[0][0] = -1.0;
      }
   } else {
      /* Negate the negative ones to make all positive, then apply above logic */
      if (xi < -TOLERANCE) {
         for (k = 0; k < 3; k++) basis->m[2][k] = -basis->m[2][k];
         T.m[2][2] = -1.0;
      }
      if (eta < -TOLERANCE) {
         for (k = 0; k < 3; k++) basis->m[1][k] = -basis->m[1][k];
         T.m[1][1] = -1.0;
      }
      if (zeta < -TOLERANCE) {
         for (k = 0; k < 3; k++) basis->m[0][k] = -basis->m[0][k];
         T.m[0][0] = -1.0;
      }
   }

   matrix_mult(&T, transform, transform);
}

/* Apply Niggli conditions, returns 1 if changed - CORRECTED */
int apply_niggli_conditions(Matrix3* basis, double A, double B, double C,
   double xi, double eta, double zeta, Matrix3* transform) {
   Matrix3 T;

   memset(&T, 0, sizeof(Matrix3));
   T.m[0][0] = T.m[1][1] = T.m[2][2] = 1.0;

   /* Step 3: |xi| > B or (xi = B and 2*eta < zeta) or (xi = -B and zeta < 0) */
   if (fabs(xi) > B + TOLERANCE) {
      T.m[2][1] = (xi > 0) ? -1.0 : 1.0;
      transform_basis(&T, basis);
      matrix_mult(&T, transform, transform);
      return 1;
   }

   if (fabs(fabs(xi) - B) < TOLERANCE) {
      if (xi > 0 && 2.0 * eta < zeta - TOLERANCE) {
         T.m[2][1] = -1.0;
         transform_basis(&T, basis);
         matrix_mult(&T, transform, transform);
         return 1;
      }
      if (xi < 0 && zeta < -TOLERANCE) {
         T.m[2][1] = 1.0;
         transform_basis(&T, basis);
         matrix_mult(&T, transform, transform);
         return 1;
      }
   }

   /* Step 4: |eta| > A or (eta = A and 2*xi < zeta) or (eta = -A and zeta < 0) */
   memset(&T, 0, sizeof(Matrix3));
   T.m[0][0] = T.m[1][1] = T.m[2][2] = 1.0;

   if (fabs(eta) > A + TOLERANCE) {
      T.m[2][0] = (eta > 0) ? -1.0 : 1.0;
      transform_basis(&T, basis);
      matrix_mult(&T, transform, transform);
      return 1;
   }

   if (fabs(fabs(eta) - A) < TOLERANCE) {
      if (eta > 0 && 2.0 * xi < zeta - TOLERANCE) {
         T.m[2][0] = -1.0;
         transform_basis(&T, basis);
         matrix_mult(&T, transform, transform);
         return 1;
      }
      if (eta < 0 && zeta < -TOLERANCE) {
         T.m[2][0] = 1.0;
         transform_basis(&T, basis);
         matrix_mult(&T, transform, transform);
         return 1;
      }
   }

   /* Step 5: |zeta| > A or (zeta = A and 2*xi < eta) or (zeta = -A and eta < 0) */
   memset(&T, 0, sizeof(Matrix3));
   T.m[0][0] = T.m[1][1] = T.m[2][2] = 1.0;

   if (fabs(zeta) > A + TOLERANCE) {
      T.m[1][0] = (zeta > 0) ? -1.0 : 1.0;
      transform_basis(&T, basis);
      matrix_mult(&T, transform, transform);
      return 1;
   }

   if (fabs(fabs(zeta) - A) < TOLERANCE) {
      if (zeta > 0 && 2.0 * xi < eta - TOLERANCE) {
         T.m[1][0] = -1.0;
         transform_basis(&T, basis);
         matrix_mult(&T, transform, transform);
         return 1;
      }
      if (zeta < 0 && eta < -TOLERANCE) {
         T.m[1][0] = 1.0;
         transform_basis(&T, basis);
         matrix_mult(&T, transform, transform);
         return 1;
      }
   }

   /* Step 6: xi + eta + zeta + A + B < 0 or (xi + eta + zeta + A + B = 0 and 2*(A + eta) + zeta > 0) */
   if (xi + eta + zeta + A + B < -TOLERANCE ||
      (fabs(xi + eta + zeta + A + B) < TOLERANCE && 2.0 * (A + eta) + zeta > TOLERANCE)) {
      memset(&T, 0, sizeof(Matrix3));
      T.m[0][0] = 1.0; T.m[0][2] = 1.0;
      T.m[1][1] = 1.0; T.m[1][2] = 1.0;
      T.m[2][2] = 1.0;
      transform_basis(&T, basis);
      matrix_mult(&T, transform, transform);
      return 1;
   }

   return 0;
}

/* Niggli reduction */
void niggli_reduction(Matrix3* basis, Matrix3* transform) {
   Matrix3 G;
   double A, B, C, xi, eta, zeta;
   int iteration, changed;

   for (iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
      compute_metric(basis, &G);

      A = G.m[0][0];
      B = G.m[1][1];
      C = G.m[2][2];
      xi = 2.0 * G.m[1][2];
      eta = 2.0 * G.m[0][2];
      zeta = 2.0 * G.m[0][1];

      changed = 0;

      /* Step 1: Check order */
      if (!check_order(A, B, C)) {
         reorder_basis(basis, A, B, C, transform);
         changed = 1;
         continue;
      }

      /* Step 2: Check type */
      if (!check_type(xi, eta, zeta)) {
         fix_type(basis, xi, eta, zeta, transform);
         changed = 1;
         continue;
      }

      /* Steps 3-6: Main Niggli conditions */
      if (apply_niggli_conditions(basis, A, B, C, xi, eta, zeta, transform)) {
         changed = 1;
         continue;
      }

      if (!changed) {
         break;
      }
   }
}

/* Main reduction function */
void reduce_cell(const Cell* input, Cell* output, Matrix3* transform) {
   Matrix3 basis;

   /* Initialize transform as identity */
   memset(transform, 0, sizeof(Matrix3));
   transform->m[0][0] = transform->m[1][1] = transform->m[2][2] = 1.0;

   /* Convert to basis */
   cell_to_basis(input, &basis);

   /* Apply Delaunay reduction */
   delaunay_reduction(&basis, transform);

   /* Apply Niggli reduction */
   niggli_reduction(&basis, transform);

   /* Convert back to cell parameters */
   basis_to_cell(&basis, output);
}

/* Print cell parameters */
void print_cell(const char* label, const Cell* cell) {
   printf("%s\n", label);
   printf("  a = %.6f Å\n", cell->a);
   printf("  b = %.6f Å\n", cell->b);
   printf("  c = %.6f Å\n", cell->c);
   printf("  ? = %.4f°\n", cell->alpha);
   printf("  ? = %.4f°\n", cell->beta);
   printf("  ? = %.4f°\n", cell->gamma);
}

int main() {
   Cell input, output;
   Matrix3 transform;

   /* Example: triclinic cell */
   input.a = 5.0;
   input.b = 6.0;
   input.c = 7.0;
   input.alpha = 85.0;
   input.beta = 95.0;
   input.gamma = 100.0;

   print_cell("Input Cell:", &input);

   reduce_cell(&input, &output, &transform);

   print_cell("\nReduced Cell:", &output);

   printf("\nTransformation Matrix:\n");
   printf("  [%6.3f %6.3f %6.3f]\n", transform.m[0][0], transform.m[0][1], transform.m[0][2]);
   printf("  [%6.3f %6.3f %6.3f]\n", transform.m[1][0], transform.m[1][1], transform.m[1][2]);
   printf("  [%6.3f %6.3f %6.3f]\n", transform.m[2][0], transform.m[2][1], transform.m[2][2]);

   return 0;
}

