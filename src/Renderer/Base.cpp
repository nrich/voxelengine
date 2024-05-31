#include "Renderer/Base.h"

using namespace Renderer;

#include "lodepng.h"

static const char *notexturebitmap =
    "11  11  "
    "11  11  "
    "  11  11"
    "  11  11"
    "11  11  "
    "11  11  "
    "  11  11"
    "  11  11";

GLuint Base::loadNoTexture() {
    if (notexture)
        return notexture;

    std::array<char, 256> notexturepixels{0};

    for (int i = 0, z = 0; i < 64; i++, z+=4) {
        auto pixel = notexturebitmap[i] != ' ' ? 255 : 0;
        notexturepixels[z+0] = pixel;
        notexturepixels[z+1] = 0;
        notexturepixels[z+2] = 0;
        notexturepixels[z+3] = 255;
    }

    glGenTextures(1, &notexture);
    glBindTexture(GL_TEXTURE_2D, notexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, notexturepixels.data());

    return notexture;
}

GLuint Base::loadTexture(const std::string &filename) {
    if (textures[filename])
        return textures[filename];

    GLuint texture;

    std::vector<unsigned char> image;
    unsigned width, height;

    auto err = lodepng::decode(image, width, height, filename);

    if (!err) {
        size_t u2 = 1; while(u2 < width) u2 *= 2;
        size_t v2 = 1; while(v2 < height) v2 *= 2;

        std::vector<unsigned char> image2(u2 * v2 * 4);
        for(size_t y = 0; y < height; y++) {
            for(size_t x = 0; x < width; x++) {
                for(size_t c = 0; c < 4; c++) {
                    image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
                }
            }
        }
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, mode, GL_UNSIGNED_BYTE, image.data());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data());

        textures[filename] = texture;
        return texture;
    } else {
        return loadNoTexture();
    }
}

GLuint Base::loadTexture(const std::string &filename, const Common::Colour &transparent) {
    if (textures[filename])
        return textures[filename];

    GLuint texture;

    std::vector<unsigned char> image;
    unsigned width, height;

    auto err = lodepng::decode(image, width, height, filename);

    if (!err) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum mode = GL_RGBA;

        for (size_t i = 0; i < image.size(); i += 4) {
            auto r = image[i+0];
            auto g = image[i+1];
            auto b = image[i+2];
            //auto a = image[i+3];

            if (r == transparent.R() && g == transparent.G() && b == transparent.B()) {
                image[i+3] = 0;
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, mode, GL_UNSIGNED_BYTE, image.data());

        textures[filename] = texture;
        return texture;
    } else {
        return loadNoTexture();
    }
}

void Base::configureVirtualDisplay() {
    if (ratio == Common::AspectRatio::_4x3) {
        switch (displayMode.Ratio()) {
            case Common::AspectRatio::_4x3:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 240;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x9:
                virtualWidth = 320;
                horizontalOffset = 40;
                virtualHeight = 180;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x10:
                virtualWidth = 320;
                horizontalOffset = 28;
                virtualHeight = 200;
                verticalOffset = 0;
                break;
            default:
                std::cerr << "Invalid aspect ratio" << std::endl;
                exit(-1);
        }

        horizontalRatio = (float)(virtualWidth-2*horizontalOffset)/320.0f;
        verticalRatio = (float)(virtualHeight-2*verticalOffset)/240.0f;
    } else if (ratio == Common::AspectRatio::_16x9) {
        switch (displayMode.Ratio()) {
            case Common::AspectRatio::_4x3:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 240;
                verticalOffset = 30;
                break;
            case Common::AspectRatio::_16x9:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 180;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x10:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 200;
                verticalOffset = 10;
                break;
            default:
                std::cerr << "Invalid aspect ratio" << std::endl;
                exit(-1);
        }

        horizontalRatio = (float)(virtualWidth-2*horizontalOffset)/320.0f;
        verticalRatio = (float)(virtualHeight-2*verticalOffset)/180.0f;
    } else if (ratio == Common::AspectRatio::_16x10) {
        switch (displayMode.Ratio()) {
            case Common::AspectRatio::_4x3:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 240;
                verticalOffset = 10;
                break;
            case Common::AspectRatio::_16x9:
                virtualWidth = 200;
                horizontalOffset = 16;
                virtualHeight = 180;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x10:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 200;
                verticalOffset = 0;
                break;
            default:
                std::cerr << "Invalid aspect ratio" << std::endl;
                exit(-1);
        }

        horizontalRatio = (float)(virtualWidth-2*horizontalOffset)/320.0f;
        verticalRatio = (float)(virtualHeight-2*verticalOffset)/200.0f;
    } else {
        std::cerr << "Invalid aspect ratio" << std::endl;
        exit(-1);
    }

    virtualWidth *= resolutionScale;
    horizontalOffset *= resolutionScale * interfaceScale;
    virtualHeight *= resolutionScale;
    verticalOffset *= resolutionScale * interfaceScale;

    interfaceRatio = (float)virtualWidth/(float)virtualHeight;
}

void Base::changeDisplayMode(const Common::DisplayMode &displayMode) {
    this->displayMode = displayMode;
    configureVirtualDisplay();
}

bool Base::translatePoint(Dot &screen, const Dot &real) {
    float hratio = ((float)virtualWidth) / (float)displayMode.Width();
    float vratio = ((float)virtualHeight) / (float)displayMode.Height();

    auto tx = real.X() * hratio;
    auto ty = real.Y() * vratio;

    if (tx < horizontalOffset)
        return false;
    if (tx > (virtualWidth - horizontalOffset))
        return false;
    if (ty < verticalOffset)
        return false;
    if (ty > (virtualHeight - verticalOffset))
        return false;

    screen = Dot((tx-horizontalOffset)/horizontalRatio, (ty-verticalOffset)/verticalRatio);

    return true;
}

MatrixF Base::screenTransformation(const float x, const float y, const float w, const float h) const {
    float xr = 2.0/virtualWidth;
    float yr = 2.0/virtualHeight;

    float xmid = x - virtualWidth/2 + horizontalOffset;
    float ymid = virtualHeight/2 - y - h - verticalOffset;

    auto screen = MatrixF::Scaling(w, h, 0) * MatrixF::Translation(xmid, ymid, 0) * MatrixF::Scaling(xr, yr, 0);

    return screen;
}
