// NiceAxisScaling.h
#if !defined(NICEAXISSCALING_H)
#define NICEAXISSCALING_H

#include <vector>
#include <string>
#include <cfloat>
#include <climits>
#include <cmath>
#include <iostream>

struct AxisConfig {
   size_t minTicks = 4;
   size_t maxTicks = 11;
   double scientificThreshold = 1000.0;
   int precision = 1;
};

struct TickInfo {
   double position;
   std::string label;
};

class AxisLimits {
public:
   AxisLimits(const double lowerLimit = std::numeric_limits<double>::max(),
      const double maxDataValue = std::numeric_limits<double>::max(),
      const double minDataValue = std::numeric_limits<double>::max(),
      const double upperLimit = std::numeric_limits<double>::max(),
      const size_t& numSteps = std::numeric_limits<size_t>::max(),
      const size_t& objectScaleFactor = std::numeric_limits<size_t>::max());

   double GetMin() const { return m_lowerLimit; }
   double GetMax() const { return m_upperLimit; }
   double GetStepSize() const { return (m_upperLimit - m_lowerLimit) / m_numSteps; }
   std::vector<TickInfo> GetTicksWithLabels() const;

   const double m_lowerLimit;
   const double m_maxDataValue;
   const double m_minDataValue;
   const double m_upperLimit;
   const size_t m_numSteps;
   const size_t m_objectScaleFactor;

private:
   AxisLimits& operator=(const AxisLimits&) const; // no assignment operator allowed
};

class StepSizeList {
public:
   StepSizeList();
   virtual ~StepSizeList();

   void clear();
   size_t size() const;
   void add(double step);
   void addStandardSteps();
   double operator[](const size_t& i) const;

protected:
   std::vector<double> m_vStepSizes;

private:
   size_t sort();
};

class NiceAxisScaling : protected StepSizeList {
public:
   class Scale {
      friend class NiceAxisScaling;
      friend std::ostream& operator<<(std::ostream& os, const NiceAxisScaling& la);

   public:
      ~Scale();
      bool operator<(const Scale& cs) const;
      double GetModulus() const;
      double GetStepSize() const;
      double GetRange() const;

   private:
      Scale(double modulus, double range, double stepSize);
      double m_dModulus;
      double m_dRange;
      double m_dStepSize;
   };

public:
   NiceAxisScaling(const AxisConfig& config = AxisConfig());
   virtual ~NiceAxisScaling();

   AxisLimits LinearAxisLimits(const double dMin, const double dMax);
   size_t SetAxisConstants();
   const std::vector<Scale>& GetScale() const { return m_vScale; }

private:
   AxisLimits FindActualLimits(const double dDataMin, const double dDataMax);
   std::vector<Scale> m_vScale;
   AxisConfig m_config;
};

std::ostream& operator<<(std::ostream& os, const NiceAxisScaling& nas);

#endif