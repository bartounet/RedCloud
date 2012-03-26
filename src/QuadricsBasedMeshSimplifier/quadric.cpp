//     This file is part of RedCloud tool.
// 
//     Copyright (C) 2012 Munzer Thibaut, Jonathan Aigrain, Adrien Chey, Nicolas Fabretti
//     Original idea from O.R.A. by Adrien Normier
//     Contacts : ora.ctc@gmail.com (+336 77 09 31 16)
//                redcloud.contact@gmail.com   
// 
//     Redcloud is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     Geoscale is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with Geoscale.  If not, see <http://www.gnu.org/licenses/>.

#include "quadric.h"
#include "../common/types.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
Quadric::Quadric()
{
	for (uint val = 0; val < MAX_VALUES; ++val)
		values_[val] = 0.f;
}
// ----------------------------------------------------------------------------
Quadric::Quadric(double parValues[])
{
	for (uint val = 0; val < MAX_VALUES; ++val)
		values_[val] = parValues[val];
}
// ----------------------------------------------------------------------------
Quadric::~Quadric()
{
}
// ----------------------------------------------------------------------------
void Quadric::Add(const Quadric& parQuadric)
{
	Add(parQuadric.Values());
}
// ----------------------------------------------------------------------------
void Quadric::Add(const double parValues[])
{
	for (uint val = 0; val < MAX_VALUES; ++val)
		values_[val] += parValues[val];
}
// ----------------------------------------------------------------------------
void Quadric::Init()
{
	for (uint val = 0; val < MAX_VALUES; ++val)
		values_[val] = 0.0;
}
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================