#include "hclib.h"
/**
 *
 * @file pdpotrf.c
 *
 *  PLASMA auxiliary routines
 *  PLASMA is a software package provided by Univ. of Tennessee,
 *  Univ. of California Berkeley and Univ. of Colorado Denver
 *
 * @version 2.6.0
 * @author Jakub Kurzak
 * @author Hatem Ltaief
 * @author Mathieu Faverge
 * @date 2010-11-15
 * @generated d Tue Jan  7 11:45:10 2014
 *
 **/
#include "common.h"
#ifdef USE_MKL
#include <mkl_lapacke.h>
#else
#include <lapacke.h>
#endif
#if defined(USE_OMPEXT)
#include <omp_ext.h>
#endif

#define A(m,n) BLKADDR(A, double, m, n)

/***************************************************************************//**
 *  Parallel tile Cholesky factorization - dynamic scheduling
 **/
typedef struct _pragma59 {
    double (*(*dA_ptr));
    int (*k_ptr);
    int (*m_ptr);
    int (*n_ptr);
    int (*ldak_ptr);
    int (*ldam_ptr);
    int (*tempkm_ptr);
    int (*tempmm_ptr);
    double (*zone_ptr);
    double (*mzone_ptr);
    PLASMA_enum (*uplo_ptr);
    PLASMA_desc (*A_ptr);
 } pragma59;

typedef struct _pragma68 {
    double (*(*dA_ptr));
    double (*(*dB_ptr));
    int (*k_ptr);
    int (*m_ptr);
    int (*n_ptr);
    int (*ldak_ptr);
    int (*ldam_ptr);
    int (*tempkm_ptr);
    int (*tempmm_ptr);
    double (*zone_ptr);
    double (*mzone_ptr);
    PLASMA_enum (*uplo_ptr);
    PLASMA_desc (*A_ptr);
 } pragma68;

typedef struct _pragma82 {
    double (*(*dA_ptr));
    double (*(*dB_ptr));
    int (*k_ptr);
    int (*m_ptr);
    int (*n_ptr);
    int (*ldak_ptr);
    int (*ldam_ptr);
    int (*tempkm_ptr);
    int (*tempmm_ptr);
    double (*zone_ptr);
    double (*mzone_ptr);
    PLASMA_enum (*uplo_ptr);
    PLASMA_desc (*A_ptr);
 } pragma82;

typedef struct _pragma96 {
    double (*(*dA_ptr));
    double (*(*dB_ptr));
    double (*(*dC_ptr));
    int (*k_ptr);
    int (*m_ptr);
    int (*n_ptr);
    int (*ldak_ptr);
    int (*ldam_ptr);
    int (*tempkm_ptr);
    int (*tempmm_ptr);
    double (*zone_ptr);
    double (*mzone_ptr);
    PLASMA_enum (*uplo_ptr);
    PLASMA_desc (*A_ptr);
 } pragma96;

static void *pragma59_hclib_async(void *____arg);
static void *pragma68_hclib_async(void *____arg);
static void *pragma82_hclib_async(void *____arg);
static void *pragma96_hclib_async(void *____arg);
void plasma_pdpotrf_quark(PLASMA_enum uplo, PLASMA_desc A)
{
    int k, m, n;
    int ldak, ldam;
    int tempkm, tempmm;

    double zone  = (double) 1.0;
    double mzone = (double)-1.0;
    /*
     *  PlasmaLower
     */
    if (uplo == PlasmaLower) {
        abort();
    }
    /*
     *  PlasmaUpper
     */
    else {
        for (k = 0; k < A.nt; k++) {
            tempkm = k == A.nt-1 ? A.n-k*A.nb : A.nb;
            ldak = BLKLDD(A, k);
            double *dA = A(k, k);
#if defined(USE_OMPEXT)
omp_set_task_priority(1);
#endif
 { 
pragma59 *new_ctx = (pragma59 *)malloc(sizeof(pragma59));
new_ctx->dA_ptr = &(dA);
new_ctx->k_ptr = &(k);
new_ctx->m_ptr = &(m);
new_ctx->n_ptr = &(n);
new_ctx->ldak_ptr = &(ldak);
new_ctx->ldam_ptr = &(ldam);
new_ctx->tempkm_ptr = &(tempkm);
new_ctx->tempmm_ptr = &(tempmm);
new_ctx->zone_ptr = &(zone);
new_ctx->mzone_ptr = &(mzone);
new_ctx->uplo_ptr = &(uplo);
new_ctx->A_ptr = &(A);
hclib_emulate_omp_task(pragma59_hclib_async, new_ctx, ANY_PLACE, 1, 1, (dA) + (0), A.mb*A.mb, (dA) + (0), A.mb*A.mb);
 } 

            for (m = k+1; m < A.nt; m++) {
                tempmm = m == A.nt-1 ? A.n-m*A.nb : A.nb;
                double *dA = A(k, k);
                double *dB = A(k, m);
 { 
pragma68 *new_ctx = (pragma68 *)malloc(sizeof(pragma68));
new_ctx->dA_ptr = &(dA);
new_ctx->dB_ptr = &(dB);
new_ctx->k_ptr = &(k);
new_ctx->m_ptr = &(m);
new_ctx->n_ptr = &(n);
new_ctx->ldak_ptr = &(ldak);
new_ctx->ldam_ptr = &(ldam);
new_ctx->tempkm_ptr = &(tempkm);
new_ctx->tempmm_ptr = &(tempmm);
new_ctx->zone_ptr = &(zone);
new_ctx->mzone_ptr = &(mzone);
new_ctx->uplo_ptr = &(uplo);
new_ctx->A_ptr = &(A);
hclib_emulate_omp_task(pragma68_hclib_async, new_ctx, ANY_PLACE, 2, 1, (dA) + (0), A.mb*A.mb, (dB) + (0), A.mb*A.mb, (dB) + (0), A.mb*A.mb);
 } ;
            }
            for (m = k+1; m < A.nt; m++) {
                tempmm = m == A.nt-1 ? A.n-m*A.nb : A.nb;
                ldam = BLKLDD(A, m);
                double *dA = A(k, m);
                double *dB = A(m, m);
 { 
pragma82 *new_ctx = (pragma82 *)malloc(sizeof(pragma82));
new_ctx->dA_ptr = &(dA);
new_ctx->dB_ptr = &(dB);
new_ctx->k_ptr = &(k);
new_ctx->m_ptr = &(m);
new_ctx->n_ptr = &(n);
new_ctx->ldak_ptr = &(ldak);
new_ctx->ldam_ptr = &(ldam);
new_ctx->tempkm_ptr = &(tempkm);
new_ctx->tempmm_ptr = &(tempmm);
new_ctx->zone_ptr = &(zone);
new_ctx->mzone_ptr = &(mzone);
new_ctx->uplo_ptr = &(uplo);
new_ctx->A_ptr = &(A);
hclib_emulate_omp_task(pragma82_hclib_async, new_ctx, ANY_PLACE, 2, 1, (dA) + (0), A.mb*A.mb, (dB) + (0), A.mb*A.mb, (dB) + (0), A.mb*A.mb);
 } 

                for (n = k+1; n < m; n++) {
                    double *dA = A(k , n);
                    double *dB = A(k , m);
                    double *dC = A(n , m);
 { 
pragma96 *new_ctx = (pragma96 *)malloc(sizeof(pragma96));
new_ctx->dA_ptr = &(dA);
new_ctx->dB_ptr = &(dB);
new_ctx->dC_ptr = &(dC);
new_ctx->k_ptr = &(k);
new_ctx->m_ptr = &(m);
new_ctx->n_ptr = &(n);
new_ctx->ldak_ptr = &(ldak);
new_ctx->ldam_ptr = &(ldam);
new_ctx->tempkm_ptr = &(tempkm);
new_ctx->tempmm_ptr = &(tempmm);
new_ctx->zone_ptr = &(zone);
new_ctx->mzone_ptr = &(mzone);
new_ctx->uplo_ptr = &(uplo);
new_ctx->A_ptr = &(A);
hclib_emulate_omp_task(pragma96_hclib_async, new_ctx, ANY_PLACE, 3, 1, (dA) + (0), A.mb*A.mb, (dB) + (0), A.mb*A.mb, (dC) + (0), A.mb*A.mb, (dC) + (0), A.mb*A.mb);
 } ;
                }
            }
        }
    }
} 
static void *pragma59_hclib_async(void *____arg) {
    pragma59 *ctx = (pragma59 *)____arg;
    hclib_start_finish();
{
                LAPACKE_dpotrf_work(LAPACK_COL_MAJOR, lapack_const(PlasmaUpper), (*(ctx->tempkm_ptr)), (*(ctx->dA_ptr)), (*(ctx->ldak_ptr)));
            } ;     ; hclib_end_finish();

    return NULL;
}


static void *pragma68_hclib_async(void *____arg) {
    pragma68 *ctx = (pragma68 *)____arg;
    hclib_start_finish();
cblas_dtrsm(
                        CblasColMajor,
                        (CBLAS_SIDE)PlasmaLeft, (CBLAS_UPLO)PlasmaUpper,
                        (CBLAS_TRANSPOSE)PlasmaTrans, (CBLAS_DIAG)PlasmaNonUnit,
                        (*(ctx->A_ptr)).mb, (*(ctx->tempmm_ptr)),
                        (*(ctx->zone_ptr)), (*(ctx->dA_ptr)), (*(ctx->ldak_ptr)),
                        (*(ctx->dB_ptr)), (*(ctx->ldak_ptr))) ;     ; hclib_end_finish();

    return NULL;
}


static void *pragma82_hclib_async(void *____arg) {
    pragma82 *ctx = (pragma82 *)____arg;
    hclib_start_finish();
{
                    cblas_dsyrk(
                            CblasColMajor,
                            (CBLAS_UPLO)PlasmaUpper, (CBLAS_TRANSPOSE)PlasmaTrans,
                            (*(ctx->tempmm_ptr)), (*(ctx->A_ptr)).mb,
                            (-1.0), (*(ctx->dA_ptr)), (*(ctx->ldak_ptr)),
                            (1.0), (*(ctx->dB_ptr)), (*(ctx->ldam_ptr)));
                } ;     ; hclib_end_finish();

    return NULL;
}


static void *pragma96_hclib_async(void *____arg) {
    pragma96 *ctx = (pragma96 *)____arg;
    hclib_start_finish();
cblas_dgemm(CblasColMajor, (CBLAS_TRANSPOSE)PlasmaTrans, (CBLAS_TRANSPOSE)PlasmaNoTrans,
                            (*(ctx->A_ptr)).mb, (*(ctx->tempmm_ptr)), (*(ctx->A_ptr)).mb,
                            (*(ctx->mzone_ptr)), (*(ctx->dA_ptr)), (*(ctx->ldak_ptr)),
                            (*(ctx->dB_ptr)), (*(ctx->ldak_ptr)),
                            (*(ctx->zone_ptr)), (*(ctx->dC_ptr)), (*(ctx->A_ptr)).mb) ;     ; hclib_end_finish();

    return NULL;
}


