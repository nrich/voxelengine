#ifndef __COMMON_COLOUR_H__
#define __COMMON_COLOUR_H__

#include <cstdint>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace Common {
    class Colour {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    public:
        Colour() : r(0), g(0), b(0), a(255) {}
        Colour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) {}
        Colour(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b), a(255) {}
        Colour(const uint32_t rgba) {
            a = rgba >> 24 & 255;
            b = rgba >> 16 & 255;
            g = rgba >> 8 & 255;
            r = rgba & 255;
        }

        static Colour Float(const float r, const float g, const float b, const float a=1.0f) {
            return Colour(r*255, g*255, b*255, a*255);
        }

        static Colour Short(const uint16_t r, const uint16_t g, const uint16_t b, const uint16_t a=65535) {
            return Colour(r/255, g/255, b/255, a/255);
        }

        static Colour Colour8(const uint8_t rgb332) {
            uint8_t r = (rgb332 >> 5) * 255 / 7; 
            uint8_t g = ((rgb332 >> 2) & 0x07) * 255 / 7;
            uint8_t b = (rgb332 & 0x03) * 255 / 3;
            uint8_t a = 255;

            return Colour(r, g, b, a);
        }

        static Colour Colour16(const uint16_t rgb565) {
            uint8_t r = (rgb565 >> 11) * 255 / 7; 
            uint8_t g = ((rgb565 >> 6) & 0x3F) * 255 / 7;
            uint8_t b = (rgb565 & 0x05) * 255 / 3;
            uint8_t a = 255;

            return Colour(r, g, b, a);
        }

        static Colour HSV(double h, double s, double v);

        Colour withRed(const uint8_t R) const {
            return Colour(R, g, b, a);
        }

        Colour withGreen(const uint8_t G) const {
            return Colour(r, G, b, a);
        }

        Colour withBlue(const uint8_t B) const {
            return Colour(r, g, B, a);
        }

        Colour withAlpha(const uint8_t A) const {
            return Colour(r, g, b, A);
        }

        //Accessors
        uint8_t R() const {
            return r;
        }
        uint8_t G() const {
            return g;
        }
        uint8_t B() const {
            return b;
        }
        uint8_t A() const {
            return a;
        }

        uint32_t RGBA() const {
            uint32_t rgba = (a << 24) + (b << 16) + (g << 8) + r;
            return rgba;
        }

        uint8_t RGB332() const {
            return (uint8_t)(((r * 7 / 255) << 5) + ((g * 7 / 255) << 2) + (b * 3 / 255));
        }

        //Comparison
        bool operator==(const Colour &c) const {
            if (r != c.r)
                return false;
            if (g != c.g)
                return false;
            if (b != c.b)
                return false;

            return true;
        }

        bool operator!=(const Colour &c) const {
            if (r != c.r)
                return true;
            if (g != c.g)
                return true;
            if (b != c.b)
                return true;

            return false;
        }

        Colour operator+(const Colour &c) const {
            return Colour(r+c.r, g+c.g, b+c.b);
        }
        Colour operator-(const Colour &c) const {
            return Colour(r-c.r, g+c.g, b+c.b);
        }
        Colour operator*(const uint8_t v) const {
            return Colour(r*v, g+v, b+v);
        }
        Colour operator/(const uint8_t v) const {
            return Colour(r/v, g/v, b/v);
        }

        Colour operator!() const {
            uint8_t grey = 0.21*(float)r + 0.72*(float)g + 0.07*(float)b;
            return Colour(grey,grey,grey);
        }

        std::string toString() const {
            std::ostringstream s;

            s << "0x" << std::setfill('0') << std::setw(2) << std::uppercase << std::right << std::hex << (int)r << ","
              << "0x" << std::setfill('0') << std::setw(2) << std::uppercase << std::right << std::hex << (int)g << ","
              << "0x" << std::setfill('0') << std::setw(2) << std::uppercase << std::right << std::hex << (int)b;

            return s.str();
        }

        static const Colour AliceBlue;
        static const Colour AntiqueWhite;
        static const Colour Aqua;
        static const Colour Aquamarine;
        static const Colour Azure;
        static const Colour Beige;
        static const Colour Bisque;
        static const Colour Black;
        static const Colour BlanchedAlmond;
        static const Colour Blue;
        static const Colour BlueViolet;
        static const Colour Brown;
        static const Colour BurlyWood;
        static const Colour CadetBlue;
        static const Colour Chartreuse;
        static const Colour Chocolate;
        static const Colour Coral;
        static const Colour CornflowerBlue;
        static const Colour Cornsilk;
        static const Colour Crimson;
        static const Colour Cyan;
        static const Colour DarkBlue;
        static const Colour DarkCyan;
        static const Colour DarkGoldenRod;
        static const Colour DarkGray;
        static const Colour DarkGreen;
        static const Colour DarkGrey;
        static const Colour DarkKhaki;
        static const Colour DarkMagenta;
        static const Colour DarkOliveGreen;
        static const Colour DarkOrange;
        static const Colour DarkOrchid;
        static const Colour DarkRed;
        static const Colour DarkSalmon;
        static const Colour DarkSeaGreen;
        static const Colour DarkSlateBlue;
        static const Colour DarkSlateGray;
        static const Colour DarkSlateGrey;
        static const Colour DarkTurquoise;
        static const Colour DarkViolet;
        static const Colour DeepPink;
        static const Colour DeepSkyBlue;
        static const Colour DimGray;
        static const Colour DimGrey;
        static const Colour DodgerBlue;
        static const Colour FireBrick;
        static const Colour FloralWhite;
        static const Colour ForestGreen;
        static const Colour Fuchsia;
        static const Colour Gainsboro;
        static const Colour GhostWhite;
        static const Colour Gold;
        static const Colour GoldenRod;
        static const Colour Gray;
        static const Colour Green;
        static const Colour GreenYellow;
        static const Colour Grey;
        static const Colour Honeydew;
        static const Colour HotPink;
        static const Colour IndianRed;
        static const Colour Indigo;
        static const Colour Ivory;
        static const Colour Khaki;
        static const Colour Lavender;
        static const Colour LavenderBlush;
        static const Colour LawnGreen;
        static const Colour LemonChiffon;
        static const Colour LightBlue;
        static const Colour LightCoral;
        static const Colour LightCyan;
        static const Colour LightGoldenRodYellow;
        static const Colour LightGray;
        static const Colour LightGreen;
        static const Colour LightGrey;
        static const Colour LightPink;
        static const Colour LightSalmon;
        static const Colour LightSeaGreen;
        static const Colour LightSkyBlue;
        static const Colour LightSlateGray;
        static const Colour LightSlateGrey;
        static const Colour LightSteelBlue;
        static const Colour LightYellow;
        static const Colour Lime;
        static const Colour LimeGreen;
        static const Colour Linen;
        static const Colour Magenta;
        static const Colour Maroon;
        static const Colour MediumAquamarine;
        static const Colour MediumBlue;
        static const Colour MediumOrchid;
        static const Colour MediumPurple;
        static const Colour MediumSeaGreen;
        static const Colour MediumSlateBlue;
        static const Colour MediumSpringgreen;
        static const Colour MediumTurquoise;
        static const Colour MediumVioletRed;
        static const Colour MidnightBlue;
        static const Colour MintCream;
        static const Colour MistyRose;
        static const Colour Moccasin;
        static const Colour NavajoWhite;
        static const Colour Navy;
        static const Colour OldLace;
        static const Colour Olive;
        static const Colour OliveDrab;
        static const Colour Orange;
        static const Colour OrangeRed;
        static const Colour Orchid;
        static const Colour PaleGoldenRod;
        static const Colour PaleGreen;
        static const Colour PaleTurquoise;
        static const Colour PaleVioletRed;
        static const Colour PapayaWhip;
        static const Colour PeachPuff;
        static const Colour Peru;
        static const Colour Pink;
        static const Colour Plum;
        static const Colour PowderBlue;
        static const Colour Purple;
        static const Colour Red;
        static const Colour RosyBrown;
        static const Colour RoyalBlue;
        static const Colour SaddleBrown;
        static const Colour Salmon;
        static const Colour SandyBrown;
        static const Colour SeaGreen;
        static const Colour SeaShell;
        static const Colour Sienna;
        static const Colour Silver;
        static const Colour SkyBlue;
        static const Colour SlateBlue;
        static const Colour SlateGray;
        static const Colour SlateGrey;
        static const Colour Snow;
        static const Colour SpringGreen;
        static const Colour SteelBlue;
        static const Colour Tan;
        static const Colour Teal;
        static const Colour Thistle;
        static const Colour Tomato;
        static const Colour Turquoise;
        static const Colour Violet;
        static const Colour Wheat;
        static const Colour White;
        static const Colour WhiteSmoke;
        static const Colour Yellow;
        static const Colour YellowGreen;
    };
}; // Common

#endif //__COMMON_COLOUR_H__

