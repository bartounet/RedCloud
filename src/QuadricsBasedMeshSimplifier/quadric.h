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

#ifndef QUADRIC_H_
#define QUADRIC_H_


#define MAX_VALUES 10 // the matrix is symetric

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
namespace QBMS
{
// ----------------------------------------------------------------------------
class Quadric
{
public:
	Quadric();
	Quadric(double parValues[]);
	~Quadric();

public:
	void Add(const Quadric& parQuadric);
	void Add(const double parValues[]);
	void Init();

public:
	const double* Values() const { return values_; }

private:
	double values_[10];
};
// ----------------------------------------------------------------------------
}
// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif 