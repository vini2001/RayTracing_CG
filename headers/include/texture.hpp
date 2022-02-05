#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "common.hpp"

class Texture {
    public:
        virtual vec3 value(vec2 uv, const p3& p) const = 0;
};
typedef shared_ptr<Texture> TexturePtr;

class SolidColor : public Texture {
    private:
        color col;

    public:
        SolidColor() {}
        SolidColor(const color& c) : col(c) {}

        SolidColor(double red, double green, double blue) : SolidColor(color(red, green, blue)) {}

        virtual color value(vec2 uv, const p3& p) const override {
            return col;
        }
};

class CheckerTexture : public Texture {
    private:
        TexturePtr odd;
        TexturePtr even;
        double scale = 8.0;

    public:
        CheckerTexture() {}
        CheckerTexture(const TexturePtr& odd, const TexturePtr& even) : odd(odd), even(even) {}
        CheckerTexture(const color& odd, const color& even) : CheckerTexture(make_shared<SolidColor>(odd), make_shared<SolidColor>(even)) {}

        
        void setScale(double s) {
            scale = 8.0 / s;
        }

        virtual color value(vec2 uv, const p3& p) const override {
            double sines = sin(scale * p.x()) * sin(scale * p.y()) * sin(scale * p.z());
            return (sines < 0 ? odd : even)->value(uv, p);
        }
};
typedef shared_ptr<CheckerTexture> CheckerTexturePtr;

#endif // !TEXTURE_HPP