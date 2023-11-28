#include "ColorTables.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setfill, std::setw
#include <sstream>
#include <vector>


std::string ColorTables::NumberToHexString(const int n) {
   std::stringstream ostr;
   ostr << std::hex << std::setfill('0') << std::setw(6) << n;
   return ostr.str();
}

void ColorTables::ConvertHexColorToRGB(const unsigned long color, 
   unsigned long& red, unsigned long& green, unsigned long& blue) {
   const std::string hexColor = NumberToHexString(color);

   // Convert hex digits to integers
//     int red, green, blue;
   if (hexColor.length() == 6) {
      red = std::stoi(hexColor.substr(0, 2), nullptr, 16);
      green = std::stoi(hexColor.substr(2, 2), nullptr, 16);
      blue = std::stoi(hexColor.substr(4, 2), nullptr, 16);
   }
   else {
      red = std::stoi(hexColor.substr(0, 1), nullptr, 16) * 16 + std::stoi(hexColor.substr(1, 1), nullptr, 16);
      green = std::stoi(hexColor.substr(1, 1), nullptr, 16) * 16 + std::stoi(hexColor.substr(2, 1), nullptr, 16);
      blue = std::stoi(hexColor.substr(2, 1), nullptr, 16) * 16 + std::stoi(hexColor.substr(2, 1), nullptr, 16);
      blue /= 2; // Divide blue by 2 to handle shorthand hex format
   }
}

const std::string ColorTables::BASIC_COLORS[] = { "red", "lightblue", "turquoise", "slategrey",
                  "orange", "blueviolet", "coral", "saddlebrown", "blue", "pink", "violet",
                  "deeppink", "mediumvioletred", "tomato", "greenyellow", "olive" };

ColorRange::ColorRange(const unsigned long mincol, const unsigned long maxcol)
   : m_minhex(std::min(mincol, maxcol)), 
   m_maxhex(std::max(mincol, maxcol)) 
{
   //std::cout << std::hex << std::setfill('0') << std::setw(6) << mincol << std::endl;;
   //std::cout << std::hex << std::setfill('0') << std::setw(6) << maxcol << std::endl;;

   //unsigned long rmin = 0;
   //unsigned long gmin = 0;
   //unsigned long bmin = 0;
   //ColorTables::ConvertHexColorToRGB(m_minhex, rmin, gmin, bmin);

   //unsigned long rmax = 0;
   //unsigned long gmax = 0;
   //unsigned long bmax = 0;
   //ColorTables::ConvertHexColorToRGB(m_maxhex, rmax, gmax, bmax);

   const CHSV hsvmin(m_minhex);
   const CHSV hsvmax(m_maxhex);
   m_maxh = hsvmax.m_nHue;
   m_maxs = hsvmax.m_nSaturation;
   m_maxv = hsvmax.m_nValue;
   m_minh = hsvmin.m_nHue;
   m_mins = hsvmin.m_nSaturation;
   m_minv = hsvmin.m_nValue;
   const int i19191 = 19191;
}

double ColorRange::ColorFraction(const double color){
   const double index = (color - m_minhex) / (m_maxhex - m_minhex);
   return index;
}

int ColorRange::GetColorFromRangeFraction(const double frac) const {
   return m_minhex + fmod(frac, 1.0) * (m_maxhex - m_minhex);
}

void ColorRange::GetHSVFromRangeFraction(const double frac, unsigned long& h,
    unsigned long& s,  unsigned long& v) const
{
   h = m_minh + fmod(frac, 1.0) * (m_maxh - m_minh);
   s = m_mins + fmod(frac, 1.0) * (m_maxs - m_mins);
   v = m_minv + fmod(frac, 1.0) * (m_maxv - m_minv);
}

void ColorRange::GetRGBFromRangeFraction(const double frac, unsigned long& r,
   unsigned long& g, unsigned long& b) const
{
   CHSV minhsv;
   minhsv.m_nHue = m_minh;
   minhsv.m_nSaturation = m_mins;
   minhsv.m_nValue = m_minv;
   CHSV maxhsv;
   maxhsv.m_nHue = m_maxh;
   maxhsv.m_nSaturation = m_maxs;
   maxhsv.m_nValue = m_maxv;
   const CRGB rgbmin(minhsv);
   const CRGB rgbmax(maxhsv);
   r = fmod(frac, 1.0) * (rgbmax.m_nRed - rgbmin.m_nRed) + rgbmin.m_nRed;
   g = fmod(frac, 1.0) * (rgbmax.m_nGreen - rgbmin.m_nGreen) + rgbmin.m_nGreen;
   b = fmod(frac, 1.0) * (rgbmax.m_nBlue - rgbmin.m_nBlue) + rgbmin.m_nBlue;
}

int ColorRange::ColorIndex( const double color ) {
   return int(255 * ColorFraction(color) + 0.499999);
}

std::string ColorTables::GetCividisHEX(const size_t n) {
   int r, g, b;
   GetCividisRGB(n, r, g, b);
   std::ostringstream ostr;
   ostr << std::setw(2) << std::setfill('0') << std::hex << r << g << b;
   return (ostr.str());
}

void ColorTables::GetCividisRGB(const size_t n, int& r, int& g, int& b) {

   int nn = n;
   if (nn < 0) nn = 0;
   if (nn > 255) nn = 255;
   switch (nn) {
   case (0):	 r = 0; g = 32; b = 76; break;
   case (1):	 r = 0; g = 32; b = 78; break;
   case (2):	 r = 0; g = 33; b = 80; break;
   case (3):	 r = 0; g = 34; b = 81; break;
   case (4):	 r = 0; g = 35; b = 83; break;
   case (5):	 r = 0; g = 35; b = 85; break;
   case (6):	 r = 0; g = 36; b = 86; break;
   case (7):	 r = 0; g = 37; b = 88; break;
   case (8):	 r = 0; g = 38; b = 90; break;
   case (9):	 r = 0; g = 38; b = 91; break;
   case (10):	 r = 0; g = 39; b = 93; break;
   case (11):	 r = 0; g = 40; b = 95; break;
   case (12):	 r = 0; g = 40; b = 97; break;
   case (13):	 r = 0; g = 41; b = 99; break;
   case (14):	 r = 0; g = 42; b = 100; break;
   case (15):	 r = 0; g = 42; b = 102; break;
   case (16):	 r = 0; g = 43; b = 104; break;
   case (17):	 r = 0; g = 44; b = 106; break;
   case (18):	 r = 0; g = 45; b = 108; break;
   case (19):	 r = 0; g = 45; b = 109; break;
   case (20):	 r = 0; g = 46; b = 110; break;
   case (21):	 r = 0; g = 46; b = 111; break;
   case (22):	 r = 0; g = 47; b = 111; break;
   case (23):	 r = 0; g = 47; b = 111; break;
   case (24):	 r = 0; g = 48; b = 111; break;
   case (25):	 r = 0; g = 49; b = 111; break;
   case (26):	 r = 0; g = 49; b = 111; break;
   case (27):	 r = 0; g = 50; b = 110; break;
   case (28):	 r = 0; g = 51; b = 110; break;
   case (29):	 r = 0; g = 52; b = 110; break;
   case (30):	 r = 0; g = 52; b = 110; break;
   case (31):	 r = 1; g = 53; b = 110; break;
   case (32):	 r = 6; g = 54; b = 110; break;
   case (33):	 r = 10; g = 55; b = 109; break;
   case (34):	 r = 14; g = 55; b = 109; break;
   case (35):	 r = 18; g = 56; b = 109; break;
   case (36):	 r = 21; g = 57; b = 109; break;
   case (37):	 r = 23; g = 57; b = 109; break;
   case (38):	 r = 26; g = 58; b = 108; break;
   case (39):	 r = 28; g = 59; b = 108; break;
   case (40):	 r = 30; g = 60; b = 108; break;
   case (41):	 r = 32; g = 60; b = 108; break;
   case (42):	 r = 34; g = 61; b = 108; break;
   case (43):	 r = 36; g = 62; b = 108; break;
   case (44):	 r = 38; g = 62; b = 108; break;
   case (45):	 r = 39; g = 63; b = 108; break;
   case (46):	 r = 41; g = 64; b = 107; break;
   case (47):	 r = 43; g = 65; b = 107; break;
   case (48):	 r = 44; g = 65; b = 107; break;
   case (49):	 r = 46; g = 66; b = 107; break;
   case (50):	 r = 47; g = 67; b = 107; break;
   case (51):	 r = 49; g = 68; b = 107; break;
   case (52):	 r = 50; g = 68; b = 107; break;
   case (53):	 r = 51; g = 69; b = 107; break;
   case (54):	 r = 53; g = 70; b = 107; break;
   case (55):	 r = 54; g = 70; b = 107; break;
   case (56):	 r = 55; g = 71; b = 107; break;
   case (57):	 r = 56; g = 72; b = 107; break;
   case (58):	 r = 58; g = 73; b = 107; break;
   case (59):	 r = 59; g = 73; b = 107; break;
   case (60):	 r = 60; g = 74; b = 107; break;
   case (61):	 r = 61; g = 75; b = 107; break;
   case (62):	 r = 62; g = 75; b = 107; break;
   case (63):	 r = 64; g = 76; b = 107; break;
   case (64):	 r = 65; g = 77; b = 107; break;
   case (65):	 r = 66; g = 78; b = 107; break;
   case (66):	 r = 67; g = 78; b = 107; break;
   case (67):	 r = 68; g = 79; b = 107; break;
   case (68):	 r = 69; g = 80; b = 107; break;
   case (69):	 r = 70; g = 80; b = 107; break;
   case (70):	 r = 71; g = 81; b = 107; break;
   case (71):	 r = 72; g = 82; b = 107; break;
   case (72):	 r = 73; g = 83; b = 107; break;
   case (73):	 r = 74; g = 83; b = 107; break;
   case (74):	 r = 75; g = 84; b = 107; break;
   case (75):	 r = 76; g = 85; b = 107; break;
   case (76):	 r = 77; g = 85; b = 107; break;
   case (77):	 r = 78; g = 86; b = 107; break;
   case (78):	 r = 79; g = 87; b = 108; break;
   case (79):	 r = 80; g = 88; b = 108; break;
   case (80):	 r = 81; g = 88; b = 108; break;
   case (81):	 r = 82; g = 89; b = 108; break;
   case (82):	 r = 83; g = 90; b = 108; break;
   case (83):	 r = 84; g = 90; b = 108; break;
   case (84):	 r = 85; g = 91; b = 108; break;
   case (85):	 r = 86; g = 92; b = 108; break;
   case (86):	 r = 87; g = 93; b = 109; break;
   case (87):	 r = 88; g = 93; b = 109; break;
   case (88):	 r = 89; g = 94; b = 109; break;
   case (89):	 r = 90; g = 95; b = 109; break;
   case (90):	 r = 91; g = 95; b = 109; break;
   case (91):	 r = 92; g = 96; b = 109; break;
   case (92):	 r = 93; g = 97; b = 110; break;
   case (93):	 r = 94; g = 98; b = 110; break;
   case (94):	 r = 95; g = 98; b = 110; break;
   case (95):	 r = 95; g = 99; b = 110; break;
   case (96):	 r = 96; g = 100; b = 110; break;
   case (97):	 r = 97; g = 101; b = 111; break;
   case (98):	 r = 98; g = 101; b = 111; break;
   case (99):	 r = 99; g = 102; b = 111; break;
   case (100):	 r = 100; g = 103; b = 111; break;
   case (101):	 r = 101; g = 103; b = 111; break;
   case (102):	 r = 102; g = 104; b = 112; break;
   case (103):	 r = 103; g = 105; b = 112; break;
   case (104):	 r = 104; g = 106; b = 112; break;
   case (105):	 r = 104; g = 106; b = 112; break;
   case (106):	 r = 105; g = 107; b = 113; break;
   case (107):	 r = 106; g = 108; b = 113; break;
   case (108):	 r = 107; g = 109; b = 113; break;
   case (109):	 r = 108; g = 109; b = 114; break;
   case (110):	 r = 109; g = 110; b = 114; break;
   case (111):	 r = 110; g = 111; b = 114; break;
   case (112):	 r = 111; g = 111; b = 114; break;
   case (113):	 r = 111; g = 112; b = 115; break;
   case (114):	 r = 112; g = 113; b = 115; break;
   case (115):	 r = 113; g = 114; b = 115; break;
   case (116):	 r = 114; g = 114; b = 116; break;
   case (117):	 r = 115; g = 115; b = 116; break;
   case (118):	 r = 116; g = 116; b = 117; break;
   case (119):	 r = 117; g = 117; b = 117; break;
   case (120):	 r = 117; g = 117; b = 117; break;
   case (121):	 r = 118; g = 118; b = 118; break;
   case (122):	 r = 119; g = 119; b = 118; break;
   case (123):	 r = 120; g = 120; b = 118; break;
   case (124):	 r = 121; g = 120; b = 119; break;
   case (125):	 r = 122; g = 121; b = 119; break;
   case (126):	 r = 123; g = 122; b = 119; break;
   case (127):	 r = 123; g = 123; b = 120; break;
   case (128):	 r = 124; g = 123; b = 120; break;
   case (129):	 r = 125; g = 124; b = 120; break;
   case (130):	 r = 126; g = 125; b = 120; break;
   case (131):	 r = 127; g = 126; b = 120; break;
   case (132):	 r = 128; g = 126; b = 120; break;
   case (133):	 r = 129; g = 127; b = 120; break;
   case (134):	 r = 130; g = 128; b = 120; break;
   case (135):	 r = 131; g = 129; b = 120; break;
   case (136):	 r = 132; g = 129; b = 120; break;
   case (137):	 r = 133; g = 130; b = 120; break;
   case (138):	 r = 134; g = 131; b = 120; break;
   case (139):	 r = 135; g = 132; b = 120; break;
   case (140):	 r = 136; g = 133; b = 120; break;
   case (141):	 r = 137; g = 133; b = 120; break;
   case (142):	 r = 138; g = 134; b = 120; break;
   case (143):	 r = 139; g = 135; b = 120; break;
   case (144):	 r = 140; g = 136; b = 120; break;
   case (145):	 r = 141; g = 136; b = 120; break;
   case (146):	 r = 142; g = 137; b = 120; break;
   case (147):	 r = 143; g = 138; b = 120; break;
   case (148):	 r = 144; g = 139; b = 120; break;
   case (149):	 r = 145; g = 140; b = 120; break;
   case (150):	 r = 146; g = 140; b = 120; break;
   case (151):	 r = 147; g = 141; b = 120; break;
   case (152):	 r = 148; g = 142; b = 120; break;
   case (153):	 r = 149; g = 143; b = 120; break;
   case (154):	 r = 150; g = 143; b = 119; break;
   case (155):	 r = 151; g = 144; b = 119; break;
   case (156):	 r = 152; g = 145; b = 119; break;
   case (157):	 r = 153; g = 146; b = 119; break;
   case (158):	 r = 154; g = 147; b = 119; break;
   case (159):	 r = 155; g = 147; b = 119; break;
   case (160):	 r = 156; g = 148; b = 119; break;
   case (161):	 r = 157; g = 149; b = 119; break;
   case (162):	 r = 158; g = 150; b = 118; break;
   case (163):	 r = 159; g = 151; b = 118; break;
   case (164):	 r = 160; g = 152; b = 118; break;
   case (165):	 r = 161; g = 152; b = 118; break;
   case (166):	 r = 162; g = 153; b = 118; break;
   case (167):	 r = 163; g = 154; b = 117; break;
   case (168):	 r = 164; g = 155; b = 117; break;
   case (169):	 r = 165; g = 156; b = 117; break;
   case (170):	 r = 166; g = 156; b = 117; break;
   case (171):	 r = 167; g = 157; b = 117; break;
   case (172):	 r = 168; g = 158; b = 116; break;
   case (173):	 r = 169; g = 159; b = 116; break;
   case (174):	 r = 170; g = 160; b = 116; break;
   case (175):	 r = 171; g = 161; b = 116; break;
   case (176):	 r = 172; g = 161; b = 115; break;
   case (177):	 r = 173; g = 162; b = 115; break;
   case (178):	 r = 174; g = 163; b = 115; break;
   case (179):	 r = 175; g = 164; b = 115; break;
   case (180):	 r = 176; g = 165; b = 114; break;
   case (181):	 r = 177; g = 166; b = 114; break;
   case (182):	 r = 178; g = 166; b = 114; break;
   case (183):	 r = 180; g = 167; b = 113; break;
   case (184):	 r = 181; g = 168; b = 113; break;
   case (185):	 r = 182; g = 169; b = 113; break;
   case (186):	 r = 183; g = 170; b = 112; break;
   case (187):	 r = 184; g = 171; b = 112; break;
   case (188):	 r = 185; g = 171; b = 112; break;
   case (189):	 r = 186; g = 172; b = 111; break;
   case (190):	 r = 187; g = 173; b = 111; break;
   case (191):	 r = 188; g = 174; b = 110; break;
   case (192):	 r = 189; g = 175; b = 110; break;
   case (193):	 r = 190; g = 176; b = 110; break;
   case (194):	 r = 191; g = 177; b = 109; break;
   case (195):	 r = 192; g = 177; b = 109; break;
   case (196):	 r = 193; g = 178; b = 108; break;
   case (197):	 r = 194; g = 179; b = 108; break;
   case (198):	 r = 195; g = 180; b = 108; break;
   case (199):	 r = 197; g = 181; b = 107; break;
   case (200):	 r = 198; g = 182; b = 107; break;
   case (201):	 r = 199; g = 183; b = 106; break;
   case (202):	 r = 200; g = 184; b = 106; break;
   case (203):	 r = 201; g = 184; b = 105; break;
   case (204):	 r = 202; g = 185; b = 105; break;
   case (205):	 r = 203; g = 186; b = 104; break;
   case (206):	 r = 204; g = 187; b = 104; break;
   case (207):	 r = 205; g = 188; b = 103; break;
   case (208):	 r = 206; g = 189; b = 103; break;
   case (209):	 r = 208; g = 190; b = 102; break;
   case (210):	 r = 209; g = 191; b = 102; break;
   case (211):	 r = 210; g = 192; b = 101; break;
   case (212):	 r = 211; g = 192; b = 101; break;
   case (213):	 r = 212; g = 193; b = 100; break;
   case (214):	 r = 213; g = 194; b = 99; break;
   case (215):	 r = 214; g = 195; b = 99; break;
   case (216):	 r = 215; g = 196; b = 98; break;
   case (217):	 r = 216; g = 197; b = 97; break;
   case (218):	 r = 217; g = 198; b = 97; break;
   case (219):	 r = 219; g = 199; b = 96; break;
   case (220):	 r = 220; g = 200; b = 96; break;
   case (221):	 r = 221; g = 201; b = 95; break;
   case (222):	 r = 222; g = 202; b = 94; break;
   case (223):	 r = 223; g = 203; b = 93; break;
   case (224):	 r = 224; g = 203; b = 93; break;
   case (225):	 r = 225; g = 204; b = 92; break;
   case (226):	 r = 227; g = 205; b = 91; break;
   case (227):	 r = 228; g = 206; b = 91; break;
   case (228):	 r = 229; g = 207; b = 90; break;
   case (229):	 r = 230; g = 208; b = 89; break;
   case (230):	 r = 231; g = 209; b = 88; break;
   case (231):	 r = 232; g = 210; b = 87; break;
   case (232):	 r = 233; g = 211; b = 86; break;
   case (233):	 r = 235; g = 212; b = 86; break;
   case (234):	 r = 236; g = 213; b = 85; break;
   case (235):	 r = 237; g = 214; b = 84; break;
   case (236):	 r = 238; g = 215; b = 83; break;
   case (237):	 r = 239; g = 216; b = 82; break;
   case (238):	 r = 240; g = 217; b = 81; break;
   case (239):	 r = 241; g = 218; b = 80; break;
   case (240):	 r = 243; g = 219; b = 79; break;
   case (241):	 r = 244; g = 220; b = 78; break;
   case (242):	 r = 245; g = 221; b = 77; break;
   case (243):	 r = 246; g = 222; b = 76; break;
   case (244):	 r = 247; g = 223; b = 75; break;
   case (245):	 r = 249; g = 224; b = 73; break;
   case (246):	 r = 250; g = 224; b = 72; break;
   case (247):	 r = 251; g = 225; b = 71; break;
   case (248):	 r = 252; g = 226; b = 70; break;
   case (249):	 r = 253; g = 227; b = 69; break;
   case (250):	 r = 255; g = 228; b = 67; break;
   case (251):	 r = 255; g = 229; b = 66; break;
   case (252):	 r = 255; g = 230; b = 66; break;
   case (253):	 r = 255; g = 231; b = 67; break;
   case (254):	 r = 255; g = 232; b = 68; break;
   case (255):	 r = 255; g = 233; b = 69; break;
   }
}

std::string OrdinalToCividisHexString(const size_t n) {

   int nn = n;
   nn = std::max(0, nn);
   nn = std::min(255, nn);

   int r, g, b;
   ColorTables::GetCividisRGB(nn, r, g, b);
   std::ostringstream ostr;
   ostr.setf(std::ios::hex, std::ios::basefield);
   ostr << r << g << b;

   return "#" + ostr.str();
}