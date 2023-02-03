#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H


class Transformations : public S6 {
public:
   Transformations(const S6& s) : S6(s) {}
   void SetToCanon(const MatS6& m) { m_toCanon = m; }
   MatS6 GetToCanon(void) const { return m_toCanon; }

protected:
   MatS6 m_toCanon;
};


#endif // TRANSFORMATIONS_H