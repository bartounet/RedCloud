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