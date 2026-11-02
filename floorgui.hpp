/**
 * FloorGUI - https://github.com/boyninja1555/floorgui
 * Copyright (c) 2026 boyninja1555
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef FLOORGUI
#define FLOORGUI

#include <cstdint>
#include <cstring>

namespace FloorGUI
{
    typedef std::uint32_t color_t;

    constexpr color_t optimize_color(std::uint8_t alpha, std::uint8_t r, std::uint8_t g, std::uint8_t b)
    {
        return (static_cast<color_t>(alpha) << 24) |
               (static_cast<color_t>(r) << 16) |
               (static_cast<color_t>(g) << 8) |
               (static_cast<color_t>(b) << 0);
    }

    struct RenderContext
    {
        unsigned int width, height;
        color_t *buffer;
    };

    struct WidgetPosition
    {
        int x, y;
    };

    struct WidgetSize
    {
        unsigned int width, height;
    };

    class Widget
    {
    public:
        WidgetPosition position;
        WidgetSize size;

        Widget(const WidgetPosition position, const WidgetSize size) : position(position), size(size) {}

        Widget(int x, int y, unsigned int width, unsigned int height) : position(WidgetPosition{x, y}), size(WidgetSize{width, height}) {}

        virtual ~Widget() = default;

        WidgetPosition get_position()
        {
            return position;
        }

        void set_position(const WidgetPosition value)
        {
            position = value;
        }

        WidgetSize get_size()
        {
            return size;
        }

        void set_size(const WidgetSize value)
        {
            size = value;
        }

        virtual void update() = 0;

        virtual void render(const RenderContext &ctx) = 0;
    };

    class Rectangle : public Widget
    {
    protected:
        color_t color = optimize_color(0xFF, 0xFF, 0xFF, 0xFF);

    public:
        Rectangle(const WidgetPosition position, const WidgetSize size, color_t color) : Widget(position, size), color(color) {}

        Rectangle(int x, int y, unsigned int width, unsigned int height, color_t color) : Widget(x, y, width, height), color(color) {}

        color_t get_color()
        {
            return color;
        }

        void set_color(color_t value)
        {
            color = value;
        }

        void update() override
        {
        }

        void render(const RenderContext &ctx) override
        {
            for (int y = position.y; y < position.y + size.height; y++)
                std::memset(ctx.buffer + position.x, color, size.width);
        }
    };

    class Image : public Widget
    {
    protected:
        color_t *data = nullptr;

    public:
        Image(const WidgetPosition position, const WidgetSize size, color_t *data) : Widget(position, size), data(data) {}

        Image(int x, int y, unsigned int width, unsigned int height, color_t *data) : Widget(x, y, width, height), data(data) {}

        color_t *data_ptr()
        {
            return data;
        }

        void update() override
        {
        }

        void render(const RenderContext &ctx) override
        {
            for (int y = position.y; y < position.y + size.height; y++)
                std::memcpy(ctx.buffer + position.x, data + y * size.width, size.width * sizeof(std::uint32_t));
        }
    };
}

#endif // FLOORGUI