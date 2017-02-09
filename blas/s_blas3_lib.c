/**************************************************************************************************
*                                                                                                 *
* This file is part of BLASFEO.                                                                   *
*                                                                                                 *
* BLASFEO -- BLAS For Embedded Optimization.                                                      *
* Copyright (C) 2016-2017 by Gianluca Frison.                                                     *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* HPMPC is free software; you can redistribute it and/or                                          *
* modify it under the terms of the GNU Lesser General Public                                      *
* License as published by the Free Software Foundation; either                                    *
* version 2.1 of the License, or (at your option) any later version.                              *
*                                                                                                 *
* HPMPC is distributed in the hope that it will be useful,                                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                                            *
* See the GNU Lesser General Public License for more details.                                     *
*                                                                                                 *
* You should have received a copy of the GNU Lesser General Public                                *
* License along with HPMPC; if not, write to the Free Software                                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                  *
*                                                                                                 *
* Author: Gianluca Frison, giaf (at) dtu.dk                                                       *
*                          gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#if defined(LA_BLAS)
#if defined(REF_BLAS_OPENBLAS)
#include <f77blas.h>
#elif defined(REF_BLAS_BLIS)
#elif defined(REF_BLAS_NETLIB)
#include "s_blas.h"
#elif defined(REF_BLAS_MKL)
#include <mkl_blas.h>
#endif
#endif

#include "../include/blasfeo_common.h"
#include "../include/blasfeo_s_aux.h"



#if defined(LA_REFERENCE)



// dgemm nt
void sgemm_nt_libstr(int m, int n, int k, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, float beta, struct s_strmat *sC, int ci, int cj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	float 
		c_00, c_01,
		c_10, c_11;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pC = sC->pA + ci + cj*ldc;
	float *pD = sD->pA + di + dj*ldd;
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[(jj+0)+ldb*kk];
				c_10 += pA[(ii+1)+lda*kk] * pB[(jj+0)+ldb*kk];
				c_01 += pA[(ii+0)+lda*kk] * pB[(jj+1)+ldb*kk];
				c_11 += pA[(ii+1)+lda*kk] * pB[(jj+1)+ldb*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10 + beta * pC[(ii+1)+ldc*(jj+0)];
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01 + beta * pC[(ii+0)+ldc*(jj+1)];
			pD[(ii+1)+ldd*(jj+1)] = alpha * c_11 + beta * pC[(ii+1)+ldc*(jj+1)];
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[(jj+0)+ldb*kk];
				c_01 += pA[(ii+0)+lda*kk] * pB[(jj+1)+ldb*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01 + beta * pC[(ii+0)+ldc*(jj+1)];
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[(jj+0)+ldb*kk];
				c_10 += pA[(ii+1)+lda*kk] * pB[(jj+0)+ldb*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10 + beta * pC[(ii+1)+ldc*(jj+0)];
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[(jj+0)+ldb*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			}
		}
	return;
	}



// dgemm nn
void sgemm_nn_libstr(int m, int n, int k, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, float beta, struct s_strmat *sC, int ci, int cj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	float 
		c_00, c_01,
		c_10, c_11;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pC = sC->pA + ci + cj*ldc;
	float *pD = sD->pA + di + dj*ldd;
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			c_01 = 0.0; ;
			c_11 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[kk+ldb*(jj+0)];
				c_10 += pA[(ii+1)+lda*kk] * pB[kk+ldb*(jj+0)];
				c_01 += pA[(ii+0)+lda*kk] * pB[kk+ldb*(jj+1)];
				c_11 += pA[(ii+1)+lda*kk] * pB[kk+ldb*(jj+1)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10 + beta * pC[(ii+1)+ldc*(jj+0)];
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01 + beta * pC[(ii+0)+ldc*(jj+1)];
			pD[(ii+1)+ldd*(jj+1)] = alpha * c_11 + beta * pC[(ii+1)+ldc*(jj+1)];
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			c_01 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[kk+ldb*(jj+0)];
				c_01 += pA[(ii+0)+lda*kk] * pB[kk+ldb*(jj+1)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01 + beta * pC[(ii+0)+ldc*(jj+1)];
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[kk+ldb*(jj+0)];
				c_10 += pA[(ii+1)+lda*kk] * pB[kk+ldb*(jj+0)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10 + beta * pC[(ii+1)+ldc*(jj+0)];
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[(ii+0)+lda*kk] * pB[kk+ldb*(jj+0)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00 + beta * pC[(ii+0)+ldc*(jj+0)];
			}
		}
	return;
	}



// dtrsm_left_lower_nottransposed_unit
void strsm_llnu_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	float
		d_00, d_01,
		d_10, d_11;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda; // triangular
	float *pB = sB->pA + bi + bj*ldb;
	float *pD = sD->pA + di + dj*ldd;
#if 1
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * pB[ii+0+ldb*(jj+0)];
			d_10 = alpha * pB[ii+1+ldb*(jj+0)];
			d_01 = alpha * pB[ii+0+ldb*(jj+1)];
			d_11 = alpha * pB[ii+1+ldb*(jj+1)];
			kk = 0;
#if 0
			for(; kk<ii-1; kk+=2)
				{
				d_00 -= pA[ii+0+lda*(kk+0)] * pD[kk+ldd*(jj+0)];
				d_10 -= pA[ii+1+lda*(kk+0)] * pD[kk+ldd*(jj+0)];
				d_01 -= pA[ii+0+lda*(kk+0)] * pD[kk+ldd*(jj+1)];
				d_11 -= pA[ii+1+lda*(kk+0)] * pD[kk+ldd*(jj+1)];
				d_00 -= pA[ii+0+lda*(kk+1)] * pD[kk+ldd*(jj+0)];
				d_10 -= pA[ii+1+lda*(kk+1)] * pD[kk+ldd*(jj+0)];
				d_01 -= pA[ii+0+lda*(kk+1)] * pD[kk+ldd*(jj+1)];
				d_11 -= pA[ii+1+lda*(kk+1)] * pD[kk+ldd*(jj+1)];
				}
			if(kk<ii)
#else
			for(; kk<ii; kk++)
#endif
				{
				d_00 -= pA[ii+0+lda*kk] * pD[kk+ldd*(jj+0)];
				d_10 -= pA[ii+1+lda*kk] * pD[kk+ldd*(jj+0)];
				d_01 -= pA[ii+0+lda*kk] * pD[kk+ldd*(jj+1)];
				d_11 -= pA[ii+1+lda*kk] * pD[kk+ldd*(jj+1)];
				}
			d_10 -= pA[ii+1+lda*kk] * d_00;
			d_11 -= pA[ii+1+lda*kk] * d_01;
			pD[ii+0+ldd*(jj+0)] = d_00;
			pD[ii+1+ldd*(jj+0)] = d_10;
			pD[ii+0+ldd*(jj+1)] = d_01;
			pD[ii+1+ldd*(jj+1)] = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * pB[ii+ldb*(jj+0)];
			d_01 = alpha * pB[ii+ldb*(jj+1)];
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= pA[ii+lda*kk] * pD[kk+ldd*(jj+0)];
				d_01 -= pA[ii+lda*kk] * pD[kk+ldd*(jj+1)];
				}
			pD[ii+ldd*(jj+0)] = d_00;
			pD[ii+ldd*(jj+1)] = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * pB[ii+0+ldb*jj];
			d_10 = alpha * pB[ii+1+ldb*jj];
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= pA[ii+0+lda*kk] * pD[kk+ldd*jj];
				d_10 -= pA[ii+1+lda*kk] * pD[kk+ldd*jj];
				}
			d_10 -= pA[ii+1+lda*kk] * d_00;
			pD[ii+0+ldd*jj] = d_00;
			pD[ii+1+ldd*jj] = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * pB[ii+ldb*jj];
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= pA[ii+lda*kk] * pD[kk+ldd*jj];
				}
			pD[ii+ldd*jj] = d_00;
			}
		}
#else
	// copy
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			for(ii=0; ii<m; ii++)
				pD[ii+ldd*jj] = alpha * pB[ii+ldb*jj];
		}
	for(jj=0; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m; ii++)
			{
			d_00 = pD[ii+ldd*jj];
			for(kk=ii+1; kk<m; kk++)
				{
				pD[kk+ldd*jj] -= pA[kk+lda*ii] * d_00;
				}
			}
		}
#endif
	return;
	}



// dtrsm_left_upper_nottransposed_notunit
void strsm_lunn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk, id;
	float
		d_00, d_01,
		d_10, d_11;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda; // triangular
	float *pB = sB->pA + bi + bj*ldb;
	float *pD = sD->pA + di + dj*ldd;
	float *dA = sA->dA;
	if(!(sA->use_dA==1 & ai==0 & aj==0))
		{
		// inverte diagonal of pA
		for(ii=0; ii<m; ii++)
			dA[ii] = 1.0/pA[ii+lda*ii];
		// use only now
		sA->use_dA = 0;
		}
#if 1
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * pB[id+0+ldb*(jj+0)];
			d_10 = alpha * pB[id+1+ldb*(jj+0)];
			d_01 = alpha * pB[id+0+ldb*(jj+1)];
			d_11 = alpha * pB[id+1+ldb*(jj+1)];
			kk = id+2;
#if 0
			for(; kk<m-1; kk+=2)
				{
				d_00 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				d_10 -= pA[id+1+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				d_01 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+1)];
				d_11 -= pA[id+1+lda*(kk+0)] * pD[kk+0+ldd*(jj+1)];
				d_00 -= pA[id+0+lda*(kk+1)] * pD[kk+1+ldd*(jj+0)];
				d_10 -= pA[id+1+lda*(kk+1)] * pD[kk+1+ldd*(jj+0)];
				d_01 -= pA[id+0+lda*(kk+1)] * pD[kk+1+ldd*(jj+1)];
				d_11 -= pA[id+1+lda*(kk+1)] * pD[kk+1+ldd*(jj+1)];
				}
			if(kk<m)
#else
			for(; kk<m; kk++)
#endif
				{
				d_00 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				d_10 -= pA[id+1+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				d_01 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+1)];
				d_11 -= pA[id+1+lda*(kk+0)] * pD[kk+0+ldd*(jj+1)];
				}
			d_10 *= dA[id+1];
			d_11 *= dA[id+1];
			d_00 -= pA[id+0+lda*(id+1)] * d_10;
			d_01 -= pA[id+0+lda*(id+1)] * d_11;
			d_00 *= dA[id+0];
			d_01 *= dA[id+0];
			pD[id+0+ldd*(jj+0)] = d_00;
			pD[id+1+ldd*(jj+0)] = d_10;
			pD[id+0+ldd*(jj+1)] = d_01;
			pD[id+1+ldd*(jj+1)] = d_11;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * pB[id+0+ldb*(jj+0)];
			d_01 = alpha * pB[id+0+ldb*(jj+1)];
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				d_01 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+1)];
				}
			d_00 *= dA[id+0];
			d_01 *= dA[id+0];
			pD[id+0+ldd*(jj+0)] = d_00;
			pD[id+0+ldd*(jj+1)] = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * pB[id+0+ldb*(jj+0)];
			d_10 = alpha * pB[id+1+ldb*(jj+0)];
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				d_10 -= pA[id+1+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				}
			d_10 *= dA[id+1];
			d_00 -= pA[id+0+lda*(id+1)] * d_10;
			d_00 *= dA[id+0];
			pD[id+0+ldd*(jj+0)] = d_00;
			pD[id+1+ldd*(jj+0)] = d_10;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * pB[id+0+ldb*(jj+0)];
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= pA[id+0+lda*(kk+0)] * pD[kk+0+ldd*(jj+0)];
				}
			d_00 *= dA[id+0];
			pD[id+0+ldd*(jj+0)] = d_00;
			}
		}
#else
	// copy
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			for(ii=0; ii<m; ii++)
				pD[ii+ldd*jj] = alpha * pB[ii+ldb*jj];
		}
	// solve
	for(jj=0; jj<n; jj++)
		{
		for(ii=m-1; ii>=0; ii--)
			{
			d_00 = pD[ii+ldd*jj] * dA[ii];
			pD[ii+ldd*jj] = d_00;
			for(kk=0; kk<ii; kk++)
				{
				pD[kk+ldd*jj] -= pA[kk+lda*ii] * d_00;
				}
			}
		}
#endif
	return;
	}



// dtrsm_right_lower_transposed_unit
void strsm_rltu_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pD = sD->pA + di + dj*ldd;
	float
		f_10,
		c_00, c_01,
		c_10, c_11;
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		f_10 = pA[jj+1+lda*(jj+0)];
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = alpha * pB[ii+0+ldb*(jj+0)];
			c_10 = alpha * pB[ii+1+ldb*(jj+0)];
			c_01 = alpha * pB[ii+0+ldb*(jj+1)];
			c_11 = alpha * pB[ii+1+ldb*(jj+1)];
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= pD[ii+0+ldd*kk] * pA[jj+0+lda*kk];
				c_10 -= pD[ii+1+ldd*kk] * pA[jj+0+lda*kk];
				c_01 -= pD[ii+0+ldd*kk] * pA[jj+1+lda*kk];
				c_11 -= pD[ii+1+ldd*kk] * pA[jj+1+lda*kk];
				}
			pD[ii+0+ldd*(jj+0)] = c_00;
			pD[ii+1+ldd*(jj+0)] = c_10;
			c_01 -= c_00 * f_10;
			c_11 -= c_10 * f_10;
			pD[ii+0+ldd*(jj+1)] = c_01;
			pD[ii+1+ldd*(jj+1)] = c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = alpha * pB[ii+0+ldb*(jj+0)];
			c_01 = alpha * pB[ii+0+ldb*(jj+1)];
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= pD[ii+0+ldd*kk] * pD[jj+0+ldd*kk];
				c_01 -= pD[ii+0+ldd*kk] * pD[jj+1+ldd*kk];
				}
			pD[ii+0+ldd*(jj+0)] = c_00;
			c_01 -= c_00 * f_10;
			pD[ii+0+ldd*(jj+1)] = c_01;
			}
		}
	for(; jj<n; jj++)
		{
		// factorize diagonal
		for(ii=0; ii<m; ii++)
			{
			c_00 = alpha * pB[ii+ldb*jj];
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= pD[ii+ldd*kk] * pA[jj+lda*kk];
				}
			pD[ii+ldd*jj] = c_00;
			}
		}
	return;
	}



// dtrsm_right_lower_transposed_unit
void strsm_rltn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pD = sD->pA + di + dj*ldd;
	float *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if(sA->use_dA!=1)
			{
			for(ii=0; ii<n; ii++)
				dA[ii] = 1.0 / pA[ii+lda*ii];
			sA->use_dA = 1;
			}
		}
	else
		{
		for(ii=0; ii<n; ii++)
			dA[ii] = 1.0 / pA[ii+lda*ii];
		sA->use_dA = 0;
		}
	float
		f_00_inv, 
		f_10, f_11_inv,
		c_00, c_01,
		c_10, c_11;
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		f_00_inv = dA[jj+0];
		f_10 = pA[jj+1+lda*(jj+0)];
		f_11_inv = dA[jj+1];
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = alpha * pB[ii+0+ldb*(jj+0)];
			c_10 = alpha * pB[ii+1+ldb*(jj+0)];
			c_01 = alpha * pB[ii+0+ldb*(jj+1)];
			c_11 = alpha * pB[ii+1+ldb*(jj+1)];
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= pD[ii+0+ldd*kk] * pA[jj+0+lda*kk];
				c_10 -= pD[ii+1+ldd*kk] * pA[jj+0+lda*kk];
				c_01 -= pD[ii+0+ldd*kk] * pA[jj+1+lda*kk];
				c_11 -= pD[ii+1+ldd*kk] * pA[jj+1+lda*kk];
				}
			c_00 *= f_00_inv;
			c_10 *= f_00_inv;
			pD[ii+0+ldd*(jj+0)] = c_00;
			pD[ii+1+ldd*(jj+0)] = c_10;
			c_01 -= c_00 * f_10;
			c_11 -= c_10 * f_10;
			c_01 *= f_11_inv;
			c_11 *= f_11_inv;
			pD[ii+0+ldd*(jj+1)] = c_01;
			pD[ii+1+ldd*(jj+1)] = c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = alpha * pB[ii+0+ldb*(jj+0)];
			c_01 = alpha * pB[ii+0+ldb*(jj+1)];
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= pD[ii+0+ldd*kk] * pD[jj+0+ldd*kk];
				c_01 -= pD[ii+0+ldd*kk] * pD[jj+1+ldd*kk];
				}
			c_00 *= f_00_inv;
			pD[ii+0+ldd*(jj+0)] = c_00;
			c_01 -= c_00 * f_10;
			c_01 *= f_11_inv;
			pD[ii+0+ldd*(jj+1)] = c_01;
			}
		}
	for(; jj<n; jj++)
		{
		// factorize diagonal
		f_00_inv = dA[jj];
		for(ii=0; ii<m; ii++)
			{
			c_00 = alpha * pB[ii+ldb*jj];
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= pD[ii+ldd*kk] * pA[jj+lda*kk];
				}
			c_00 *= f_00_inv;
			pD[ii+ldd*jj] = c_00;
			}
		}
	return;
	}



// dtrsm_right_upper_transposed_notunit
void strsm_rutn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pD = sD->pA+di+dj*sD->m;
	printf("\nstrsm_rutn_libstr: feature not implemented yet\n");
	exit(1);
//	if(!(pB==pD))
//		{
//		for(jj=0; jj<n; jj++)
//			dcopy_(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
//		}
//	dtrsm_(&cr, &cu, &ct, &cn, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
	return;
	}



// dtrmm_right_upper_transposed_notunit (A triangular !!!)
void strmm_rutn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	float 
		c_00, c_01,
		c_10, c_11;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pD = sD->pA + di + dj*ldd;
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			kk = jj;
			c_00 += pB[(ii+0)+ldb*kk] * pA[(jj+0)+lda*kk];
			c_10 += pB[(ii+1)+ldb*kk] * pA[(jj+0)+lda*kk];
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[(jj+0)+lda*kk];
				c_10 += pB[(ii+1)+ldb*kk] * pA[(jj+0)+lda*kk];
				c_01 += pB[(ii+0)+ldb*kk] * pA[(jj+1)+lda*kk];
				c_11 += pB[(ii+1)+ldb*kk] * pA[(jj+1)+lda*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10;
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01;
			pD[(ii+1)+ldd*(jj+1)] = alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			kk = jj;
			c_00 += pB[(ii+0)+ldb*kk] * pA[(jj+0)+lda*kk];
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[(jj+0)+lda*kk];
				c_01 += pB[(ii+0)+ldb*kk] * pA[(jj+1)+lda*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[(jj+0)+lda*kk];
				c_10 += pB[(ii+1)+ldb*kk] * pA[(jj+0)+lda*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[(jj+0)+lda*kk];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			}
		}	
	return;
	}



// dtrmm_right_lower_nottransposed_notunit (A triangular !!!)
void strmm_rlnn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	float 
		c_00, c_01,
		c_10, c_11;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pD = sD->pA + di + dj*ldd;
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			c_01 = 0.0; ;
			c_11 = 0.0; ;
			kk = jj;
			c_00 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+0)];
			c_10 += pB[(ii+1)+ldb*kk] * pA[kk+lda*(jj+0)];
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+0)];
				c_10 += pB[(ii+1)+ldb*kk] * pA[kk+lda*(jj+0)];
				c_01 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+1)];
				c_11 += pB[(ii+1)+ldb*kk] * pA[kk+lda*(jj+1)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10;
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01;
			pD[(ii+1)+ldd*(jj+1)] = alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			c_01 = 0.0; ;
			kk = jj;
			c_00 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+0)];
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+0)];
				c_01 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+1)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			pD[(ii+0)+ldd*(jj+1)] = alpha * c_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+0)];
				c_10 += pB[(ii+1)+ldb*kk] * pA[kk+lda*(jj+0)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			pD[(ii+1)+ldd*(jj+0)] = alpha * c_10;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += pB[(ii+0)+ldb*kk] * pA[kk+lda*(jj+0)];
				}
			pD[(ii+0)+ldd*(jj+0)] = alpha * c_00;
			}
		}
	return;
	}



// dsyrk_lower_nortransposed (allowing for different factors => use dgemm !!!)
void ssyrk_ln_libstr(int m, int n, int k, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, float beta, struct s_strmat *sC, int ci, int cj, struct s_strmat *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;
	int ii, jj, kk;
	float
		c_00, c_01,
		c_10, c_11;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pC = sC->pA + ci + cj*ldc;
	float *pD = sD->pA + di + dj*ldd;
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		// diagonal
		c_00 = 0.0;
		c_10 = 0.0;
		c_11 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += pA[jj+0+lda*kk] * pB[jj+0+ldb*kk];
			c_10 += pA[jj+1+lda*kk] * pB[jj+0+ldb*kk];
			c_11 += pA[jj+1+lda*kk] * pB[jj+1+ldb*kk];
			}
		pD[jj+0+ldd*(jj+0)] = beta * pC[jj+0+ldc*(jj+0)] + alpha * c_00;
		pD[jj+1+ldd*(jj+0)] = beta * pC[jj+1+ldc*(jj+0)] + alpha * c_10;
		pD[jj+1+ldd*(jj+1)] = beta * pC[jj+1+ldc*(jj+1)] + alpha * c_11;
		// lower
		ii = jj+2;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[ii+0+lda*kk] * pB[jj+0+ldb*kk];
				c_10 += pA[ii+1+lda*kk] * pB[jj+0+ldb*kk];
				c_01 += pA[ii+0+lda*kk] * pB[jj+1+ldb*kk];
				c_11 += pA[ii+1+lda*kk] * pB[jj+1+ldb*kk];
				}
			pD[ii+0+ldd*(jj+0)] = beta * pC[ii+0+ldc*(jj+0)] + alpha * c_00;
			pD[ii+1+ldd*(jj+0)] = beta * pC[ii+1+ldc*(jj+0)] + alpha * c_10;
			pD[ii+0+ldd*(jj+1)] = beta * pC[ii+0+ldc*(jj+1)] + alpha * c_01;
			pD[ii+1+ldd*(jj+1)] = beta * pC[ii+1+ldc*(jj+1)] + alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[ii+0+lda*kk] * pB[jj+0+ldb*kk];
				c_01 += pA[ii+0+lda*kk] * pB[jj+1+ldb*kk];
				}
			pD[ii+0+ldd*(jj+0)] = beta * pC[ii+0+ldc*(jj+0)] + alpha * c_00;
			pD[ii+0+ldd*(jj+1)] = beta * pC[ii+0+ldc*(jj+1)] + alpha * c_01;
			}
		}
	for(; jj<n; jj++)
		{
		// diagonal
		c_00 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += pA[jj+lda*kk] * pB[jj+ldb*kk];
			}
		pD[jj+ldd*jj] = beta * pC[jj+ldc*jj] + alpha * c_00;
		// lower
		for(ii=jj+1; ii<m; ii++)
			{
			c_00 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += pA[ii+lda*kk] * pB[jj+ldb*kk];
				}
			pD[ii+ldd*jj] = beta * pC[ii+ldc*jj] + alpha * c_00;
			}
		}
	return;
	}



#elif defined(LA_BLAS)



// dgemm nt
void sgemm_nt_libstr(int m, int n, int k, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, float beta, struct s_strmat *sC, int ci, int cj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cn = 'n';
	char ct = 't';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pC = sC->pA+ci+cj*sC->m;
	float *pD = sD->pA+di+dj*sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	sgemm(&cn, &ct, &m, &n, &k, &alpha, pA, &(sA->m), pB, &(sB->m), &beta, pD, &(sD->m));
#else
	sgemm_(&cn, &ct, &m, &n, &k, &alpha, pA, &(sA->m), pB, &(sB->m), &beta, pD, &(sD->m));
#endif
	return;
	}



// dgemm nn
void sgemm_nn_libstr(int m, int n, int k, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, float beta, struct s_strmat *sC, int ci, int cj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cn = 'n';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pC = sC->pA+ci+cj*sC->m;
	float *pD = sD->pA+di+dj*sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	sgemm(&cn, &cn, &m, &n, &k, &alpha, pA, &(sA->m), pB, &(sB->m), &beta, pD, &(sD->m));
#else
	sgemm_(&cn, &cn, &m, &n, &k, &alpha, pA, &(sA->m), pB, &(sB->m), &beta, pD, &(sD->m));
#endif
	return;
	}



// dtrsm_left_lower_nottransposed_unit
void strsm_llnu_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cu = 'u';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pD = sD->pA+di+dj*sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	strsm(&cl, &cl, &cn, &cu, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#else
	strsm_(&cl, &cl, &cn, &cu, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#endif
	return;
	}



// dtrsm_left_upper_nottransposed_notunit
void strsm_lunn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cu = 'u';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pD = sD->pA+di+dj*sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	strsm(&cl, &cu, &cn, &cn, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#else
	strsm_(&cl, &cu, &cn, &cn, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#endif
	return;
	}



// dtrsm_right_lower_transposed_unit
void strsm_rltu_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pD = sD->pA+di+dj*sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	strsm(&cr, &cl, &ct, &cu, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#else
	strsm_(&cr, &cl, &ct, &cu, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#endif
	return;
	}



// dtrsm_right_lower_transposed_notunit
void strsm_rltn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pD = sD->pA+di+dj*sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	strsm(&cr, &cl, &ct, &cn, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#else
	strsm_(&cr, &cl, &ct, &cn, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#endif
	return;
	}



// dtrsm_right_upper_transposed_notunit
void strsm_rutn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	float *pA = sA->pA+ai+aj*sA->m;
	float *pB = sB->pA+bi+bj*sB->m;
	float *pD = sD->pA+di+dj*sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pB+jj*sB->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	strsm(&cr, &cu, &ct, &cn, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#else
	strsm_(&cr, &cu, &ct, &cn, &m, &n, &alpha, pA, &(sA->m), pD, &(sD->m));
#endif
	return;
	}



// dtrmm_right_upper_transposed_notunit (A triangular !!!)
void strmm_rutn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA+ai+aj*lda;
	float *pB = sB->pA+bi+bj*ldb;
	float *pD = sD->pA+di+dj*ldd;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
#else
			scopy_(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	strmm(&cr, &cu, &ct, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
#else
	strmm_(&cr, &cu, &ct, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
#endif
	return;
	}



// dtrmm_right_lower_nottransposed_notunit (A triangular !!!)
void strmm_rlnn_libstr(int m, int n, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	float *pA = sA->pA+ai+aj*lda;
	float *pB = sB->pA+bi+bj*ldb;
	float *pD = sD->pA+di+dj*ldd;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
#else
			scopy_(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
#endif
		}
#if defined(REF_BLAS_MKL)
	strmm(&cr, &cl, &cn, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
#else
	strmm_(&cr, &cl, &cn, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
#endif
	return;
	}



// dsyrk_lower_nortransposed (allowing for different factors => use dgemm !!!)
void ssyrk_ln_libstr(int m, int n, int k, float alpha, struct s_strmat *sA, int ai, int aj, struct s_strmat *sB, int bi, int bj, float beta, struct s_strmat *sC, int ci, int cj, struct s_strmat *sD, int di, int dj)
	{
	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	int mmn = m-n;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	float *pA = sA->pA + ai + aj*lda;
	float *pB = sB->pA + bi + bj*ldb;
	float *pC = sC->pA + ci + cj*ldc;
	float *pD = sD->pA + di + dj*ldd;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
#if defined(REF_BLAS_MKL)
			scopy(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
#else
			scopy_(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
#endif
		}
	if(pA==pB)
		{
#if defined(REF_BLAS_MKL)
		ssyrk(&cl, &cn, &n, &k, &alpha, pA, &lda, &beta, pD, &ldd);
		sgemm(&cn, &ct, &mmn, &n, &k, &alpha, pA+n, &lda, pB, &ldb, &beta, pD+n, &ldd);
#else
		ssyrk_(&cl, &cn, &n, &k, &alpha, pA, &lda, &beta, pD, &ldd);
		sgemm_(&cn, &ct, &mmn, &n, &k, &alpha, pA+n, &lda, pB, &ldb, &beta, pD+n, &ldd);
#endif
		}
	else
		{
#if defined(REF_BLAS_MKL)
		sgemm(&cn, &ct, &m, &n, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
#else
		sgemm_(&cn, &ct, &m, &n, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
#endif
		}
	return;
	}

#else

#error : wrong LA choice

#endif




