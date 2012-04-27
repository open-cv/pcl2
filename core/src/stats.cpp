/**
 * Software License Agreement (BSD License)
 * 
 * Point Cloud Library (PCL) - www.pointclouds.org
 * Copyright (c) 2009-2012, Willow Garage, Inc.
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met: 
 * 
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \file stats.cpp 
 * \brief Instantiate functions for computing statistical properties of a matrix
 */

#include "pcl2/impl/stats.hpp"

template pcl2::TypedMat<float> pcl2::computeMean<float> (const TypedMat<float> &);
template pcl2::TypedMat<double> pcl2::computeMean<double> (const TypedMat<double> &);

template pcl2::TypedMat<float> pcl2::computeCovariance<float> (const TypedMat<float> &);
template pcl2::TypedMat<double> pcl2::computeCovariance<double> (const TypedMat<double> &);

template pcl2::TypedMat<float> pcl2::computeCovariance<float> (const TypedMat<float> &, const TypedMat<float> &);
template pcl2::TypedMat<double> pcl2::computeCovariance<double> (const TypedMat<double> &, const TypedMat<double> &);

template void pcl2::computeMeanAndCovariance<float> (const TypedMat<float> &, TypedMat<float> &, TypedMat<float> &);
template void pcl2::computeMeanAndCovariance<double> (const TypedMat<double> &, TypedMat<double> &, TypedMat<double> &);
