// From paciorek and bonin, 1992");
/*

class G6Types_Base {
public:

   std::string TestSubtype() const;
   MatG6 GetMatrix() const { return m_projector; }
   virtual std::vector<G6> CreateCells(const size_t n/* = 0* /);
   std::string GetName() const { return m_name; }
   virtual bool IsMember(const G6& g) {
      return true;
   }

protected:
   std::string m_name;
   std::string m_ITnumber;
   std::string m_character;
   std::string m_latticeType;
   MatG6 m_projector;
};

//std::vector< S6(*)(const S6&)> S6Dist::m_reductionFunctions;


std::vector<G6> G6Types_Base::CreateCells(const size_t n/* = 0 * /) {
   std::vector<G6> cells;
   for (size_t i = 0; i < g_maxgen; ++i) {
      const G6 g6 = m_projector * (G6().randDeloneUnreduced());
      if (IsMember(g6) && LRL_Cell(g6).IsValid()) cells.push_back(g6);
      if (cells.size() >= n) break;
   }
   return cells;
}

class Gen44A : public G6Types_Base {
public:
   Gen44A();
};

Gen44A::Gen44A() : G6Types_Base() {
   m_name = "44A";
   m_character = "[r,r,r,0,0,0]";
   m_projector = MatG6(" 1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0");
}

class Gen44C : public G6Types_Base {
public:
   Gen44C();
};

Gen44C::Gen44C() : G6Types_Base() {
   m_name = "44C";
   m_character = "[r,r,r,0,0,0]";
   m_projector = MatG6(" 1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1");
}

class Gen44B : public G6Types_Base {
public:
   Gen44B();
};

std::string minustwothirds = "-.6666666666666667";

Gen44B::Gen44B() : G6Types_Base() {
   m_name = "44B";
   m_character = "[r,r,r,0,0,0]";
   m_projector = MatG6(" 1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0"
      " 0 0 0  " + minustwothirds + " " + minustwothirds + " " + minustwothirds + ""
      " 0 0 0  " + minustwothirds + " " + minustwothirds + " " + minustwothirds + ""
      " 0 0 0  " + minustwothirds + " " + minustwothirds + " " + minustwothirds);
}

class Gen45Aa : public G6Types_Base {
public:
   Gen45Aa();
   bool IsMember(const G6& g) { return g[0] < (g[2] / 2.0); }
};

Gen45Aa::Gen45Aa() {
   m_name = "45A";
   m_character = "[r,r,s,0,0,0]";
   m_projector = MatG6("1 1 0 0 0 0  1 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0");
}

class Gen45Ab : public G6Types_Base {
public:
   Gen45Ab();
   bool IsMember(const G6& g) { return g[0] >= (g[2] / 2.0); }
};

Gen45Ab::Gen45Ab() {
   m_name = "45Ab";
   m_character = "[r,r,s,0,0,0]";
   m_projector = MatG6("1 1 0 0 0 0  1 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0");
}

class Gen45B : public G6Types_Base {
public:
   Gen45B();
   bool IsMember(const G6& g) { return g[0] >= (g[2] / 2.0); }
};

Gen45B::Gen45B() {
   m_name = "45B";
   m_character = "[r,s,s,0,0,0]";
   m_projector = MatG6("1 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0");
}

class Gen45Da : public G6Types_Base {
public:
   Gen45Da();
   bool IsMember(const G6& g) { return g[3] > g[0] / 3.0; }
};

Gen45Da::Gen45Da() {
   m_name = "45Da";
   m_character = "[r,r,r, s,s, -2r-2s]";
   //m_projector = MatG6("1 1 1  0 0 0"
   //   " 1 1 1  0 0 0"
   //   " 1 1 1  0 0 0"
   //   " 0 0 0 1 1 0"
   //   " 0 0 0 1 1 0"
   //   " -2 0 0 -2 0 0");
   m_projector = MatS6(
      " 1 1 1 0 0 0 "
      " 1 1 1 0 0 0 "
      " 1 1 1 0 0 0 "
      "-0.5 0 0 0.5 0 0 "
      "-0.5 0 0 0.5 0 0 "
      " 0 0 0 -1 0 0 "
   );
}

class Gen45b : public G6Types_Base {
public:
   Gen45b();
   bool IsMember(const G6& g) { return g[3] <= g[0] / 3.0; }
};

Gen45b::Gen45b() {
   m_name = "45b";
   m_character = "[r,r,r, s,s, -2r-2s]";
   m_projector = MatG6("1 1 1  0 0 0"
      " 1 1 1  0 0 0"
      " 1 1 1  0 0 0"
      " 0 0 0 -1 0 0 "
      " -1 0 0 -0.5 0 0"
      " -1 0 0 -0.5 0 0"
   );
}

void CreateListOfTypes(std::vector<std::unique_ptr<G6Types_Base> >& vfpg) {
   std::cout << "CreateListOfTypes" << std::endl;
   std::unique_ptr<G6Types_Base> fpg;

   vfpg.push_back(std::move(std::unique_ptr<G6Types_Base>(new Gen44A)));
   vfpg.push_back(std::move(std::unique_ptr<G6Types_Base>(new Gen44C)));
   vfpg.push_back(std::move(std::unique_ptr<G6Types_Base>(new Gen44B)));
   vfpg.push_back(std::move(std::unique_ptr<G6Types_Base>(new Gen45Aa)));
   vfpg.push_back(std::move(std::unique_ptr<G6Types_Base>(new Gen45Ab)));
   vfpg.push_back(std::move(std::unique_ptr<G6Types_Base>(new Gen45B)));
   vfpg.push_back(std::move(std::unique_ptr<G6Types_Base>(new Gen45Da)));

}

void Generate(std::vector<std::unique_ptr<G6Types_Base> >& vfpg) {
   for (size_t i = 0; i < vfpg.size(); ++i) {
      const std::vector<G6> cells = vfpg[i]->CreateCells(10);
      std::cout << "; case " << vfpg[i]->GetName() << std::endl;
      for (size_t k = 0; k < cells.size(); ++k) {
         std::cout << "g " << cells[k] << (vfpg[i]->IsMember(cells[k]) ? " valid " : " invalid ") << std::endl;
      }
      std::cout << std::endl;
   }
}
*/



/*

IT_Lat_Char_Base::IT_Lat_Char_Base() {

}



class Gen_IT_Types {
public:
   std::string m_name;
   std::string m_latticeType;
   std::string m_ITnumber;
   std::string m_character;
   double m_prjPrefix;
   std::string m_strProjector;
   MatG6 m_projector;  // from Paciorek & Bonin, 1992
   void write() {

      const std::string clsName = "IT_Lat_Char_" + m_ITnumber;
      std::cout << "class " << clsName << " : public IT_Lat_Char_Base {" << std::endl;
      std::cout << "public:" << std::endl;
      std::cout << "   " << clsName << "();" << std::endl;
      std::cout << "};" << std::endl; //   END OF CLASS
      std::cout << std::endl;

      std::cout << clsName << "::" << clsName << "()   : IT_Lat_Char_Base() {" << std::endl;
      std::cout << "   m_name = \"" << m_name << "\";" << std::endl;
      std::cout << "   m_latticeType = \"" << m_latticeType << "\";\n";
      std::cout << "   m_ITnumber = \"" << m_ITnumber << "\";\n";
      std::cout << "   m_character = \"" << m_character << "\";\n";
      std::cout << "   m_prjPrefix = " << m_prjPrefix << ";\n";
      std::cout << "   m_strProjector = \"" << m_strProjector << "\";\n";
      m_projector = m_prjPrefix * MatG6(m_strProjector);
      std::cout << "   m_projector = m_prjPrefix * MatG6(m_strProjector);\n";
      std::cout << "/*\n";
      std::cout << "   m_projector = " << m_projector << ";\n";
      std::cout << "* /\n";
         std::cout << "} // end of class " << clsName << " constructor\n\n\n";

   }




   Gen_IT_Types() {
      m_name = "44C";
      m_latticeType = "cF";
      m_ITnumber = "1";
      m_character = "(r, r, r, r, r, r)";
      m_prjPrefix = 1. / 3.;
      m_strProjector = "1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1";
      write();

      m_name = "49C";
      m_latticeType = "hR";
      m_ITnumber = "2";
      m_character = "(r, r, r, s, s, s)";
      m_prjPrefix = 1. / 3.;
      m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
      write();

      m_name = "44A";
      m_latticeType = "cP";
      m_ITnumber = "3";
      m_character = "(r, r, r, 0, 0, 0)";
      m_prjPrefix = 1. / 3.;
      m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "49D";
      m_latticeType = "hR";
      m_ITnumber = "4";
      m_character = "(r, r, r, s, s, s)";
      m_prjPrefix = 1. / 3.;
      m_strProjector = "1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1";
      write();

      m_name = "44B";
      m_latticeType = "cI";
      m_ITnumber = "5";
      m_character = "(3r, 3r, 3r, -2r, -2r, -2r)";
      m_prjPrefix = 1. / 39.;
      m_strProjector = "9 9 9 -6 -6 -6  9 9 9 -6 -6 -6  9 9 9 -6 -6 -6 -6 -6 -6 4 4 4 -6 -6 -6 4 4 4 -6 -6 -6 4 4 4";
      write();

      m_name = "45D";
      m_latticeType = "tI";
      m_ITnumber = "6";
      m_character = "(r, r, r, -r -s, -r -s, 2s)";
      m_prjPrefix = 1. / 26.;
      m_strProjector = "6 6 6 -4 -4 -4  6 6 6 -4 -4 -4  6 6 6 -4 -4 -4 -4 -4 -4 7 7 -6 -4 -4 -4 7 7 -6 -4 -4 -4 -6 -6 20";
      write();

      m_name = "45D";
      m_latticeType = "tI";
      m_ITnumber = "7";
      m_character = "(r, r, r, 2s, -r -s, -r -s)";
      m_prjPrefix = 1. / 26.;
      m_strProjector = "6 6 6 -4 -4 -4  6 6 6 -4 -4 -4  6 6 6 -4 -4 -4 -4 -4 -4 20 -6 -6 -4 -4 -4 -6 7 7 -4 -4 -4 -6 7 7";
      write();

      m_name = "52A";
      m_latticeType = "oI";
      m_ITnumber = "8";
      m_character = "(r, r, r, s, t, -2r -s -t)";
      m_prjPrefix = 1. / 13.;
      m_strProjector = "3 3 3 -2 -2 -2  3 3 3 -2 -2 -2  3 3 3 -2 -2 -2 -2 -2 -2 10 -3 -3 -2 -2 -2 -3 10 -3 -2 -2 -2 -3 -3 10";
      write();

      m_name = "49B";
      m_latticeType = "hR";
      m_ITnumber = "9";
      m_character = "(r, r, s, r, r, r)";
      m_prjPrefix = 1. / 5.;
      m_strProjector = "1 1 0 1 1 1  1 1 0 1 1 1  0 0 5 0 0 0  1 1 0 1 1 1  1 1 0 1 1 1  1 1 0 1 1 1";
      write();

      m_name = "55A";
      m_latticeType = "mC";
      m_ITnumber = "10";
      m_character = "(r, r, s, t, t, u)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
      write();

      m_name = "45A";
      m_latticeType = "tP";
      m_ITnumber = "11";
      m_character = "(r, r, s, 0,0,0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "48A";
      m_latticeType = "hP";
      m_ITnumber = "12";
      m_character = "(r, r, s, 0, 0, -r)";
      m_prjPrefix = 1. / 3.;
      m_strProjector = "1 1 0 0 0 -1  1 1 0 0 0 -1  0 0 3 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0 -1 -1 0 0 0 1";
      write();

      m_name = "50D";
      m_latticeType = "oC";
      m_ITnumber = "13";
      m_character = "(r, r, s, 0, 0, t)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 2";
      write();

      m_name = "55A";
      m_latticeType = "mC";
      m_ITnumber = "14";
      m_character = "(r, r, s, t, t, u)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 1 0 0 0 0  1 1 0 0 0 0  0 0 2 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 2";
      write();

      m_name = "45C";
      m_latticeType = "tI";
      m_ITnumber = "15";
      m_character = "(r, r, s, -r, -r, 0)";
      m_prjPrefix = 1. / 4.;
      m_strProjector = "1 1 0 -1 -1 0  1 1 0 -1 -1 0  0 0 4 0 0 0  -1 -1 0 1 1 0  -1 -1 0 1 1 0  0 0 0 0 0 0";
      write();

      m_name = "51A";
      m_latticeType = "oF";
      m_ITnumber = "16";
      m_character = "(r, r, s, t, t, -2r -2t)";
      m_prjPrefix = 1. / 10.;
      m_strProjector = "3 3 0 -2 -2 -2  3 3 0 -2 -2 -2  0 0 10 0 0 0  -2 -2 0 3 3 -2  -2 -2 0 3 3 -2  -2 -2 0 -2 -2 8";
      write();

      m_name = "57B";
      m_latticeType = "mC";
      m_ITnumber = "17";
      m_character = "(r, r, s, t, u, -2r -t -u)";
      m_prjPrefix = 1. / 10.;
      m_strProjector = "3 3 0 -2 -2 -2  3 3 0 -2 -2 -2  0 0 10 0 0 0 -2 -2 0 8 -2 -2 -2 -2 0 -2 8 -2 -2 -2 0 -2 -2 8";
      write();

      m_name = "45E";
      m_latticeType = "tI";
      m_ITnumber = "18";
      m_character = "(2r, s, s, r, 2r, 2r)";
      m_prjPrefix = 1. / 26.;
      m_strProjector = "8 0 0 4 8 8  0 13 13 0 0 0  0 13 13 0 0 0  4 0 0 2 4 4  8 0 0 4 8 8  8 0 0 4 8 8";
      write();

      m_name = "52B";
      m_latticeType = "oI";
      m_ITnumber = "19";
      m_character = "(r, s, s, t, r, r)";
      m_prjPrefix = 1. / 6.;
      m_strProjector = "2 0 0 0 2 2  0 3 3 0 0 0  0 3 3 0 0 0  0 0 0 6 0 0  2 0 0 0 2 2  2 0 0 0 2 2";
      write();

      m_name = "55B";
      m_latticeType = "mC";
      m_ITnumber = "20";
      m_character = "(r, s, s, t, u, u)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 1 1  0 0 0 0 1 1";
      write();

      m_name = "45B";
      m_latticeType = "tP";
      m_ITnumber = "21";
      m_character = "(r, s, s, 0, 0, 0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "48B";
      m_latticeType = "hP";
      m_ITnumber = "22";
      m_character = "(r, s, s, -s, 0, 0)";
      m_strProjector = "3 0 0 0 0 0  0 1 1 -1 0 0  0 1 1 -1 0 0  0 -1 -1 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "50E";
      m_latticeType = "oC";
      m_ITnumber = "23";
      m_character = "(r, s, s, t, 0, 0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "49E";
      m_latticeType = "hR";
      m_ITnumber = "24";
      m_character = "(3r, s, s, -s, -s+r, -2r, -2r)";
      m_prjPrefix = 1. / 53.;
      m_strProjector = "27 3 3 6 -18 -18  3 18 18 -17 -2 -2  3 18 18 -17 -2 -2  6 -17 -17 19 -4 -4  18 -2 -2 -4 12 12  -18 -2 -2 -4 12 12";
      write();

      m_name = "55B";
      m_latticeType = "mC";
      m_ITnumber = "25";
      m_character = "(r, s, s, t, u, u)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "2 0 0 0 0 0  0 1 1 0 0 0  0 1 1 0 0 0  0 0 0 2 0 0  0 0 0 0 1 1  0 0 0 0 1 1";
      write();

      m_name = "51B";
      m_latticeType = "oF";
      m_ITnumber = "26";
      m_character = "(2r, s, t, r, 2r, 2r)";
      m_prjPrefix = 1. / 13.;
      m_strProjector = "4 0 0 2 4 4  0 13 0 0 0 0  0 0 13 0 0 0  2 0 0 1 2 2  4 0 0 2 4 4  4 0 0 2 4 4";
      write();

      m_name = "57C";
      m_latticeType = "mC";
      m_ITnumber = "27";
      m_character = "(r, s, t, u, r, r)";
      m_prjPrefix = 1. / 3.;
      m_strProjector = "1 0 0 0 1 1  0 3 0 0 0 0  0 0 3 0 0 0  0 0 0 3 0 0  1 0 0 0 1 1  1 0 0 0 1 1";
      write();

      m_name = "56A";
      m_latticeType = "mC";
      m_ITnumber = "28";
      m_character = "(r, s, t, u, r, 2u)";
      m_prjPrefix = 1. / 10.;
      m_strProjector = "5 0 0 0 5 0  0 10 0 0 0 0  0 0 10 0 0 0  0 0 0 2 0 4  5 0 0 0 5 0  0 0 0 4 0 8";
      write();

      m_name = "56C";
      m_latticeType = "mC";
      m_ITnumber = "29";
      m_character = "(r, s, t, u, 2u, r)";
      m_prjPrefix = 1. / 10.;
      m_strProjector = "5 0 0 0 0 5  0 10 0 0 0 0  0 0 10 0 0 0  0 0 0 2 4 0  0 0 0 4 8 0  5 0 0 0 0 5";
      write();

      m_name = "56B";
      m_latticeType = "mC";
      m_ITnumber = "30";
      m_character = "(r, s, t, s, u, 2u)";
      m_prjPrefix = 1. / 10.;
      m_strProjector = "10 0 0 0 0 0  0 5 0 5 0 0  0 0 10 0 0 0  0 5 0 5 0 0  0 0 0 0 2 4  0 0 0 0 4 8";
      write();

      m_name = "50C";
      m_latticeType = "oP";
      m_ITnumber = "32";
      m_character = "(r, s, t, 0, 0, 0)";
      m_prjPrefix = 1. / 1.;
      m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "53A";
      m_latticeType = "mP";
      m_ITnumber = "33";
      m_character = "(r, s, t, 0, u, 0)";
      m_prjPrefix = 1. / 1.;
      m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 0";
      write();

      m_name = "53C";
      m_latticeType = "mP";
      m_ITnumber = "34";
      m_character = "(r, s, t, 0, 0, u)";
      m_prjPrefix = 1. / 1.;
      m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 1";
      write();

      m_name = "53B";
      m_latticeType = "mP";
      m_ITnumber = "35";
      m_character = "(r, s, t, u, 0, 0)";
      m_prjPrefix = 1. / 1.;
      m_strProjector = "1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "50A";
      m_latticeType = "oC";
      m_ITnumber = "36";
      m_character = "(r,s, t, 0, -r, 0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 0 0 0 -1 0  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  -1 0 0 0 1 0  0 0 0 0 0 0";
      write();

      m_name = "54C";
      m_latticeType = "mC";
      m_ITnumber = "37";
      m_character = "(r, s, t, u, -r, 0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 0 0 0 -1 0  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 2 0 0 -1 0 0 0 1 0  0 0 0 0 0 0";
      write();

      m_name = "50B";
      m_latticeType = "oC";
      m_ITnumber = "38";
      m_character = "(r, s, t, 0, 0, -r)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 0 0 0 0 -1  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0 -1 0 0 0 0 1";
      write();

      m_name = "54A";
      m_latticeType = "mC";
      m_ITnumber = "39";
      m_character = "(r, s, t, u, 0, -r)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 0 0 0 0 -1  0 2 0 0 0 0  0 0 2 0 0 0  0 0 0 2 0 0  0 0 0 0 0 0 -1 0 0 0 0 1";
      write();

      m_name = "50F";
      m_latticeType = "oC";
      m_ITnumber = "40";
      m_character = "(r, s, t, -s, 0, 0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "2 0 0 0 0 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0  0 0 0 0 0 0  0 0 0 0 0 0";
      write();

      m_name = "54B";
      m_latticeType = "mC";
      m_ITnumber = "41";
      m_character = "(r, s, t, -s, u, 0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "2 0 0 0 0 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0  0 0 0 0 2 0  0 0 0 0 0 0";
      write();

      m_name = "52C";
      m_latticeType = "oI";
      m_ITnumber = "42";
      m_character = "(r, s, t, -s, -r, 0)";
      m_prjPrefix = 1. / 2.;
      m_strProjector = "1 0 0 0 -1 0  0 1 0 -1 0 0  0 0 2 0 0 0  0 -1 0 1 0 0 -1 0 0 0 1 0  0 0 0 0 0 0";
      write();

      m_name = "57A";
      m_latticeType = "mC";
      m_ITnumber = "43";
      m_character = "(r, s, t, -s -u, -r -u, 2u)";
      m_prjPrefix = 1. / 20.;
      m_strProjector = "11 1 0 1 -9 -4  1 11 0 -9 1 -4  0 0 20 0 0 0  1 -9 0 11 1 -4 -9 1 0 1 11 -4 -4 -4 0 -4 -4 16";
      write();

   }
};
*/
