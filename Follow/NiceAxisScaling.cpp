// NiceAxisScaling.cpp
#include "NiceAxisScaling.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

StepSizeList::StepSizeList() {
   m_vStepSizes.clear();
}

StepSizeList::~StepSizeList() {
   m_vStepSizes.clear();
}

void StepSizeList::add(double step) {
   if (step == 0.0) return;
   step = std::abs(step);

   // Scale to 1-10 range
   while (step < 1.0) step *= 10.0;
   while (step >= 10.0) step /= 10.0;

   if (std::find(m_vStepSizes.begin(), m_vStepSizes.end(), step) == m_vStepSizes.end()) {
      m_vStepSizes.push_back(step);
   }
   if (std::find(m_vStepSizes.begin(), m_vStepSizes.end(), step / 10.0) == m_vStepSizes.end()) {
      m_vStepSizes.push_back(step / 10.0);
   }
   sort();
}

size_t StepSizeList::sort() {
   std::sort(m_vStepSizes.begin(), m_vStepSizes.end());
   return m_vStepSizes.size();
}

void StepSizeList::addStandardSteps() {
   clear();
   add(2.0);  // These will automatically add their /10 versions
   add(5.0);
   add(1.0);
   add(10.0);
}

size_t StepSizeList::size() const {
   return m_vStepSizes.size();
}

double StepSizeList::operator[](const size_t& i) const {
   return m_vStepSizes[i];
}

void StepSizeList::clear() {
   m_vStepSizes.clear();
}

AxisLimits::AxisLimits(const double lowerLimit,
   const double maxDataValue,
   const double minDataValue,
   const double upperLimit,
   const size_t& numSteps,
   const size_t& objectScaleFactor)
   : m_lowerLimit(lowerLimit)
   , m_maxDataValue(maxDataValue)
   , m_minDataValue(minDataValue)
   , m_upperLimit(upperLimit)
   , m_numSteps(numSteps)
   , m_objectScaleFactor(objectScaleFactor)
{}

std::vector<TickInfo> AxisLimits::GetTicksWithLabels() const {
   std::vector<TickInfo> ticks;
   const double step = GetStepSize();
   const bool useScientific = (std::abs(m_upperLimit) >= 1000.0 ||
      std::abs(m_lowerLimit) >= 1000.0);

   for (size_t i = 0; i <= m_numSteps; ++i) {
      const double value = m_lowerLimit + i * step;
      std::ostringstream ss;
      if (useScientific) {
         ss << std::scientific << std::setprecision(1);
      }
      else {
         ss << std::fixed << std::setprecision(1);
      }
      ss << value;
      ticks.push_back({ value, ss.str() });
   }
   return ticks;
}

NiceAxisScaling::Scale::Scale(double modulus, double range, double stepSize)
   : m_dModulus(modulus)
   , m_dRange(range)
   , m_dStepSize(stepSize)
{}

NiceAxisScaling::Scale::~Scale() = default;

bool NiceAxisScaling::Scale::operator<(const Scale& cs) const {
   if (m_dRange < cs.m_dRange) return true;
   if (m_dRange > cs.m_dRange) return false;
   if (m_dStepSize > cs.m_dStepSize) return true;
   if (m_dStepSize < cs.m_dStepSize) {
      const int iStepsThis = static_cast<int>(m_dRange / m_dStepSize + 0.1);
      const int iStepscs = static_cast<int>(cs.m_dRange / cs.m_dStepSize + 0.1);
      return (iStepsThis < iStepscs);
   }
   return (m_dModulus < cs.m_dModulus);
}

double NiceAxisScaling::Scale::GetModulus() const { return m_dModulus; }
double NiceAxisScaling::Scale::GetStepSize() const { return m_dStepSize; }
double NiceAxisScaling::Scale::GetRange() const { return m_dRange; }

NiceAxisScaling::NiceAxisScaling(const AxisConfig& config)
   : m_config(config) {
   addStandardSteps();
   SetAxisConstants();
}

NiceAxisScaling::~NiceAxisScaling() {
   m_vScale.clear();
}

size_t NiceAxisScaling::SetAxisConstants() {
   m_vScale.clear();
   size_t ulCount = 0;

   for (size_t iStep = 0; iStep < m_vStepSizes.size(); ++iStep) {
      for (size_t iTicks = m_config.minTicks; iTicks <= m_config.maxTicks; ++iTicks) {
         const double dRangeTemp = iTicks * m_vStepSizes[iStep];
         if (dRangeTemp >= 1.0 && dRangeTemp <= 10 + 3 * m_vStepSizes[iStep]) {
            double dModulus = m_vStepSizes[iStep];
            ++ulCount;
            m_vScale.push_back(Scale(dModulus, dRangeTemp, m_vStepSizes[iStep]));

            if (m_vStepSizes[iStep] == 2.0) {
               dModulus = 1.0;
               ++ulCount;
               m_vScale.push_back(Scale(dModulus, dRangeTemp, m_vStepSizes[iStep]));
            }
         }
      }
   }

   if (m_vScale.empty()) {
      m_vScale.push_back(Scale(1.0, 10.0, 1.0));
      ++ulCount;
   }

   std::sort(m_vScale.begin(), m_vScale.end());
   return ulCount;
}

AxisLimits NiceAxisScaling::FindActualLimits(const double dDataMin, const double dDataMax) {
   if (dDataMax <= dDataMin) {
      return AxisLimits();
   }

   double dRexponent = floor(log10(dDataMax - dDataMin));
   double dExpTerm = pow(10.0, dRexponent);
   double dMantissa = (dDataMax - dDataMin) / dExpTerm;
   double dRangeMin = std::numeric_limits<double>::max();
   double dRangeMax = -std::numeric_limits<double>::max();
   size_t i = 0;

   while (i < m_vScale.size() && m_vScale[i].GetRange() < dMantissa) {
      ++i;
   }

   if (i >= m_vScale.size()) {
      i = m_vScale.size() - 1;
   }

   const double dMod = m_vScale[i].GetModulus() * dExpTerm;
   dRangeMin = dDataMin - fmod(dDataMin, dMod);
   if (dRangeMin > dDataMin) {
      dRangeMin -= dMod;
   }
   dRangeMax = dRangeMin + m_vScale[i].GetRange() * dExpTerm;

   const size_t numSteps = static_cast<size_t>(m_vScale[i].GetRange() / m_vScale[i].GetStepSize() + 0.5);

   return AxisLimits(dRangeMin, dDataMax, dDataMin, dRangeMax, numSteps, i);
}

AxisLimits NiceAxisScaling::LinearAxisLimits(const double dDataMin, const double dDataMax) {
   if (dDataMin < dDataMax) {
      return FindActualLimits(dDataMin, dDataMax);
   }
   else if (dDataMin > dDataMax) {
      return LinearAxisLimits(dDataMax, dDataMin);
   }
   else if ((dDataMin == 0.0) && (dDataMax == 0.0)) {
      return LinearAxisLimits(-1.0, 1.0);
   }
   else {
      return LinearAxisLimits(0.9 * dDataMax, 1.1 * dDataMax);
   }
}

std::ostream& operator<<(std::ostream& os, const NiceAxisScaling& nas) {
   const size_t iTemp = nas.GetScale().size();
   const std::streamsize precision = os.precision();
   const std::streamsize width = os.width();

   os << "i,  range,  step size, modulus, steps" << std::endl;
   for (size_t i = 0; i < iTemp; ++i) {
      os << std::setw(3) << i << "  "
         << std::setw(5) << std::setprecision(3) << nas.GetScale()[i].GetRange() << "  "
         << std::setw(6) << std::setprecision(2) << nas.GetScale()[i].GetStepSize() << "   "
         << std::setw(7) << nas.GetScale()[i].GetModulus() << " "
         << std::setw(6) << int(nas.GetScale()[i].GetRange() / nas.GetScale()[i].GetStepSize() + 0.1)
         << std::endl;
   }

   os.precision(precision);
   os.width(width);
   return os;
}