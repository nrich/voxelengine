#include <Common/Colour.h>

#include <cmath>

using namespace Common;

const Colour Colour::AliceBlue(240,248,255);
const Colour Colour::AntiqueWhite(250,235,215);
const Colour Colour::Aqua(0,255,255);
const Colour Colour::Aquamarine(127,255,212);
const Colour Colour::Azure(240,255,255);
const Colour Colour::Beige(245,245,220);
const Colour Colour::Bisque(255,228,196);
const Colour Colour::Black(0,0,0);
const Colour Colour::BlanchedAlmond(255,235,205);
const Colour Colour::Blue(0,0,255);
const Colour Colour::BlueViolet(138,43,226);
const Colour Colour::Brown(165,42,42);
const Colour Colour::BurlyWood(222,184,135);
const Colour Colour::CadetBlue(95,158,160);
const Colour Colour::Chartreuse(127,255,0);
const Colour Colour::Chocolate(210,105,30);
const Colour Colour::Coral(255,127,80);
const Colour Colour::CornflowerBlue(100,149,237);
const Colour Colour::Cornsilk(255,248,220);
const Colour Colour::Crimson(220,20,60);
const Colour Colour::Cyan(0,255,255);
const Colour Colour::DarkBlue(0,0,139);
const Colour Colour::DarkCyan(0,139,139);
const Colour Colour::DarkGoldenRod(184,134,11);
const Colour Colour::DarkGray(169,169,169);
const Colour Colour::DarkGreen(0,100,0);
const Colour Colour::DarkGrey(169,169,169);
const Colour Colour::DarkKhaki(189,183,107);
const Colour Colour::DarkMagenta(139,0,139);
const Colour Colour::DarkOliveGreen(85,107,47);
const Colour Colour::DarkOrange(255,140,0);
const Colour Colour::DarkOrchid(153,50,204);
const Colour Colour::DarkRed(139,0,0);
const Colour Colour::DarkSalmon(233,150,122);
const Colour Colour::DarkSeaGreen(143,188,143);
const Colour Colour::DarkSlateBlue(72,61,139);
const Colour Colour::DarkSlateGray(47,79,79);
const Colour Colour::DarkSlateGrey(47,79,79);
const Colour Colour::DarkTurquoise(0,206,209);
const Colour Colour::DarkViolet(148,0,211);
const Colour Colour::DeepPink(255,20,147);
const Colour Colour::DeepSkyBlue(0,191,255);
const Colour Colour::DimGray(105,105,105);
const Colour Colour::DimGrey(105,105,105);
const Colour Colour::DodgerBlue(30,144,255);
const Colour Colour::FireBrick(178,34,34);
const Colour Colour::FloralWhite(255,250,240);
const Colour Colour::ForestGreen(34,139,34);
const Colour Colour::Fuchsia(255,0,255);
const Colour Colour::Gainsboro(220,220,220);
const Colour Colour::GhostWhite(248,248,255);
const Colour Colour::Gold(255,215,0);
const Colour Colour::GoldenRod(218,165,32);
const Colour Colour::Gray(128,128,128);
const Colour Colour::Green(0,128,0);
const Colour Colour::GreenYellow(173,255,47);
const Colour Colour::Grey(128,128,128);
const Colour Colour::Honeydew(240,255,240);
const Colour Colour::HotPink(255,105,180);
const Colour Colour::IndianRed(205,92,92);
const Colour Colour::Indigo(75,0,130);
const Colour Colour::Ivory(255,255,240);
const Colour Colour::Khaki(240,230,140);
const Colour Colour::Lavender(230,230,250);
const Colour Colour::LavenderBlush(255,240,245);
const Colour Colour::LawnGreen(124,252,0);
const Colour Colour::LemonChiffon(255,250,205);
const Colour Colour::LightBlue(173,216,230);
const Colour Colour::LightCoral(240,128,128);
const Colour Colour::LightCyan(224,255,255);
const Colour Colour::LightGoldenRodYellow(250,250,210);
const Colour Colour::LightGray(211,211,211);
const Colour Colour::LightGreen(144,238,144);
const Colour Colour::LightGrey(211,211,211);
const Colour Colour::LightPink(255,182,193);
const Colour Colour::LightSalmon(255,160,122);
const Colour Colour::LightSeaGreen(32,178,170);
const Colour Colour::LightSkyBlue(135,206,250);
const Colour Colour::LightSlateGray(119,136,153);
const Colour Colour::LightSlateGrey(119,136,153);
const Colour Colour::LightSteelBlue(176,196,222);
const Colour Colour::LightYellow(255,255,224);
const Colour Colour::Lime(0,255,0);
const Colour Colour::LimeGreen(50,205,50);
const Colour Colour::Linen(250,240,230);
const Colour Colour::Magenta(255,0,255);
const Colour Colour::Maroon(128,0,0);
const Colour Colour::MediumAquamarine(102,205,170);
const Colour Colour::MediumBlue(0,0,205);
const Colour Colour::MediumOrchid(186,85,211);
const Colour Colour::MediumPurple(147,112,219);
const Colour Colour::MediumSeaGreen(60,179,113);
const Colour Colour::MediumSlateBlue(123,104,238);
const Colour Colour::MediumSpringgreen(0,250,154);
const Colour Colour::MediumTurquoise(72,209,204);
const Colour Colour::MediumVioletRed(199,21,133);
const Colour Colour::MidnightBlue(25,25,112);
const Colour Colour::MintCream(245,255,250);
const Colour Colour::MistyRose(255,228,225);
const Colour Colour::Moccasin(255,228,181);
const Colour Colour::NavajoWhite(255,222,173);
const Colour Colour::Navy(0,0,128);
const Colour Colour::OldLace(253,245,230);
const Colour Colour::Olive(128,128,0);
const Colour Colour::OliveDrab(107,142,35);
const Colour Colour::Orange(255,165,0);
const Colour Colour::OrangeRed(255,69,0);
const Colour Colour::Orchid(218,112,214);
const Colour Colour::PaleGoldenRod(238,232,170);
const Colour Colour::PaleGreen(152,251,152);
const Colour Colour::PaleTurquoise(175,238,238);
const Colour Colour::PaleVioletRed(219,112,147);
const Colour Colour::PapayaWhip(255,239,213);
const Colour Colour::PeachPuff(255,218,185);
const Colour Colour::Peru(205,133,63);
const Colour Colour::Pink(255,192,203);
const Colour Colour::Plum(221,160,221);
const Colour Colour::PowderBlue(176,224,230);
const Colour Colour::Purple(128,0,128);
const Colour Colour::Red(255,0,0);
const Colour Colour::RosyBrown(188,143,143);
const Colour Colour::RoyalBlue(65,105,225);
const Colour Colour::SaddleBrown(139,69,19);
const Colour Colour::Salmon(250,128,114);
const Colour Colour::SandyBrown(244,164,96);
const Colour Colour::SeaGreen(46,139,87);
const Colour Colour::SeaShell(255,245,238);
const Colour Colour::Sienna(160,82,45);
const Colour Colour::Silver(192,192,192);
const Colour Colour::SkyBlue(135,206,235);
const Colour Colour::SlateBlue(106,90,205);
const Colour Colour::SlateGray(112,128,144);
const Colour Colour::SlateGrey(112,128,144);
const Colour Colour::Snow(255,250,250);
const Colour Colour::SpringGreen(0,255,127);
const Colour Colour::SteelBlue(70,130,180);
const Colour Colour::Tan(210,180,140);
const Colour Colour::Teal(0,128,128);
const Colour Colour::Thistle(216,191,216);
const Colour Colour::Tomato(255,99,71);
const Colour Colour::Turquoise(64,224,208);
const Colour Colour::Violet(238,130,238);
const Colour Colour::Wheat(245,222,179);
const Colour Colour::White(255,255,255);
const Colour Colour::WhiteSmoke(245,245,245);
const Colour Colour::Yellow(255,255,0);
const Colour Colour::YellowGreen(154,205,50);

static double Clip(double input, double min = 0, double max = 1) {
    return input < min ? min : input > max ? max : input;
}

Colour Colour::HSV(double h, double s, double v) {
    h = std::fmod(h, 360.0);

    auto c = v * s;
    auto x = c * (1 - std::abs(std::fmod((h / 60),2) - 1));
    auto m = v - c;

    double r, g, b;

    if (h >= 0 && h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h >= 60 && h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h >= 180 && h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h >= 240 && h < 300) {
        r = x;
        g = 0;
        b = c;
    } else if (h >= 300 && h < 360) {
        r = c;
        g = 0;
        b = x;
    } else {
        std::cerr << "Value not inside range 0..360";
        exit(-1);
    }

    r = Clip(r + m);
    g = Clip(g + m);
    b = Clip(b + m);

    return Colour((int)(r * 255), (int)(g * 255), (int)((b * 255)));
}

