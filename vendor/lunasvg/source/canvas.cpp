#include "canvas.h"

#include <blend2d.h>

namespace lunasvg {

static BLRgba32 to_bl_color(const Color& color);
static BLMatrix2D to_bl_matrix(const Transform& matrix);
static BLPath to_bl_path(const Path& path);
static BLFillRule to_bl_fill_rule(WindRule fillRule);
static BLStrokeCap to_bl_line_cap(LineCap cap);
static BLStrokeJoin to_bl_line_join(LineJoin join);
static BLCompOp to_bl_comp_op(BlendMode mode);

class CanvasImpl
{
public:
    CanvasImpl(int width, int height);
    CanvasImpl(unsigned char* data, int width, int height, int stride);
    ~CanvasImpl();

public:
    BLImage image;
    BLContext context;
    BLStyle style;
};

CanvasImpl::CanvasImpl(int width, int height)
{
    image.create(width, height, BL_FORMAT_PRGB32);
    context.begin(image);
    context.clearAll();
}

CanvasImpl::CanvasImpl(unsigned char* data, int width, int height, int stride)
{
    image.createFromData(width, height, BL_FORMAT_PRGB32, data, stride);
    context.begin(image);
    context.clearAll();
}

CanvasImpl::~CanvasImpl()
{
}

void Canvas::setMatrix(const Transform& matrix)
{
    d->context.setMatrix(to_bl_matrix(matrix));
}

void Canvas::setOpacity(double opacity)
{
    d->context.setGlobalAlpha(opacity);
}

void Canvas::setColor(const Color& color)
{
    d->style = to_bl_color(color);
}

void Canvas::setGradient(const LinearGradientValues& values, const Transform& matrix, SpreadMethod spread, const GradientStops& stops)
{
    BLGradient gradient;
    gradient.setType(BL_GRADIENT_TYPE_LINEAR);
    gradient.setValues(BLLinearGradientValues(values.x1, values.y1, values.x2, values.y2));
    for(auto& stop : stops)
    {
        auto offset = std::get<0>(stop);
        auto& color = std::get<1>(stop);
        gradient.addStop(offset, to_bl_color(color));
    }

    gradient.setMatrix(to_bl_matrix(matrix));
    switch(spread) {
    case SpreadMethod::Reflect:
        gradient.setExtendMode(BL_EXTEND_MODE_REFLECT);
        break;
    case SpreadMethod::Repeat:
        gradient.setExtendMode(BL_EXTEND_MODE_REPEAT);
        break;
    default:
        gradient.setExtendMode(BL_EXTEND_MODE_PAD);
        break;
    }

    d->style = gradient;
}

void Canvas::setGradient(const RadialGradientValues& values, const Transform& matrix, SpreadMethod spread, const GradientStops& stops)
{
    BLGradient gradient;
    gradient.setType(BL_GRADIENT_TYPE_RADIAL);
    gradient.setValues(BLRadialGradientValues(values.cx, values.cy, values.fx, values.fy, values.r));
    for(auto& stop : stops)
    {
        auto offset = std::get<0>(stop);
        auto& color = std::get<1>(stop);
        gradient.addStop(offset, to_bl_color(color));
    }

    gradient.setMatrix(to_bl_matrix(matrix));
    switch(spread) {
    case SpreadMethod::Reflect:
        gradient.setExtendMode(BL_EXTEND_MODE_REFLECT);
        break;
    case SpreadMethod::Repeat:
        gradient.setExtendMode(BL_EXTEND_MODE_REPEAT);
        break;
    default:
        gradient.setExtendMode(BL_EXTEND_MODE_PAD);
        break;
    }

    d->style = gradient;
}

void Canvas::setPattern(const Canvas& tile, const Transform& matrix, TileMode mode)
{
    BLPattern pattern = BLPattern(tile.d->image);
    pattern.setMatrix(to_bl_matrix(matrix));
    switch(mode) {
    case TileMode::Tiled:
        pattern.setExtendMode(BL_EXTEND_MODE_REPEAT);
        break;
    default:
        pattern.setExtendMode(BL_EXTEND_MODE_PAD);
        break;
    }

    d->style = pattern;
}

void Canvas::setWinding(WindRule winding)
{
    d->context.setFillRule(to_bl_fill_rule(winding));
}

void Canvas::setLineWidth(double width)
{
    d->context.setStrokeWidth(width);
}

void Canvas::setLineCap(LineCap cap)
{
    d->context.setStrokeCaps(to_bl_line_cap(cap));
}

void Canvas::setLineJoin(LineJoin join)
{
    d->context.setStrokeJoin(to_bl_line_join(join));
}

void Canvas::setMiterlimit(double miterlimit)
{
    d->context.setStrokeMiterLimit(miterlimit);
}

void Canvas::setDash(const DashData& dash)
{
    // Not supported
}

void Canvas::fill(const Path& path)
{
    d->context.save();
    d->context.setFillStyle(d->style);
    d->context.fillPath(to_bl_path(path));
    d->context.restore();
}

void Canvas::stroke(const Path& path)
{
    d->context.save();
    d->context.setStrokeStyle(d->style);
    d->context.strokePath(to_bl_path(path));
    d->context.restore();
}

void Canvas::blend(const Canvas& source, BlendMode mode, double opacity)
{
    d->context.save();
    d->context.setCompOp(to_bl_comp_op(mode));
    d->context.setGlobalAlpha(opacity);
    d->context.blitImage(BLPoint(0, 0), source.d->image);
    d->context.restore();
}

void Canvas::clear(unsigned int value)
{
    auto r = (value >> 24) & 0xFF;
    auto g = (value >> 16) & 0xFF;
    auto b = (value >> 8) & 0xFF;
    auto a = (value >> 0) & 0xFF;

    d->context.save();
    d->context.setFillStyle(BLRgba32(r, g, b, a));
    d->context.fillAll();
    d->context.restore();
}

void Canvas::rgba()
{
    auto data = this->data();
    auto width = this->width();
    auto height = this->height();
    auto stride = this->stride();
    for(auto y = 0;y < height;y++)
    {
        auto row = reinterpret_cast<uint32_t*>(data + stride * y);
        for(int x = 0;x < width;x++)
        {
            auto pixel = row[x];
            auto a = (pixel >> 24) & 0xFF;
            if(a == 0)
                continue;

            auto r = (pixel >> 16) & 0xFF;
            auto g = (pixel >> 8) & 0xFF;
            auto b = (pixel >> 0) & 0xFF;
            if(a != 255)
            {
                r = (r * 255) / a;
                g = (g * 255) / a;
                b = (b * 255) / a;
            }

            row[x] = (a << 24) | (b << 16) | (g << 8) | r;
        }
    }
}

void Canvas::luminance()
{
    auto data = this->data();
    auto width = this->width();
    auto height = this->height();
    auto stride = this->stride();
    for(auto y = 0;y < height;y++)
    {
        auto row = reinterpret_cast<uint32_t*>(data + stride * y);
        for(int x = 0;x < width;x++)
        {
            auto pixel = row[x];
            auto r = (pixel >> 16) & 0xFF;
            auto g = (pixel >> 8) & 0xFF;
            auto b = (pixel >> 0) & 0xFF;
            auto l = (2*r + 3*g + b) / 6;

            row[x] = l << 24;
        }
    }
}

unsigned char* Canvas::data() const
{
    BLImageData imaged;
    d->image.getData(&imaged);

    return static_cast<std::uint8_t*>(imaged.pixelData);
}

unsigned int Canvas::width() const
{
    return std::uint32_t(d->image.width());
}

unsigned int Canvas::height() const
{
    return std::uint32_t(d->image.height());
}

unsigned int Canvas::stride() const
{
    BLImageData imaged;
    d->image.getData(&imaged);

    return std::uint32_t(imaged.stride);
}

std::shared_ptr<Canvas> Canvas::create(unsigned int width, unsigned int height)
{
    return std::shared_ptr<Canvas>(new Canvas(width, height));
}

std::shared_ptr<Canvas> Canvas::create(unsigned char* data, unsigned int width, unsigned int height, unsigned int stride)
{
    return std::shared_ptr<Canvas>(new Canvas(data, width, height, stride));
}

Canvas::~Canvas()
{
}

Canvas::Canvas(unsigned int width, unsigned int height)
    : d(new CanvasImpl(static_cast<int>(width), static_cast<int>(height)))
{
}

Canvas::Canvas(unsigned char* data, unsigned int width, unsigned int height, unsigned int stride)
    : d(new CanvasImpl(data, static_cast<int>(width), static_cast<int>(height), static_cast<int>(stride)))
{
}

LinearGradientValues::LinearGradientValues(double x1, double y1, double x2, double y2)
    : x1(x1), y1(y1), x2(x2), y2(y2)
{
}

RadialGradientValues::RadialGradientValues(double cx, double cy, double r, double fx, double fy)
    : cx(cx), cy(cy), r(r), fx(fx), fy(fy)
{
}

BLPath to_bl_path(const Path& path)
{
    BLPath result;

    PathIterator it(path);
    std::array<Point, 3> p;
    while(!it.isDone())
    {
        switch(it.currentSegment(p)) {
        case PathCommand::MoveTo:
            result.moveTo(p[0].x, p[0].y);
            break;
        case PathCommand::LineTo:
            result.lineTo(p[0].x, p[0].y);
            break;
        case PathCommand::CubicTo:
            result.cubicTo(p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y);
            break;
        case PathCommand::Close:
            result.close();
            break;
        }

        it.next();
    }

    return result;
}

BLMatrix2D to_bl_matrix(const Transform& matrix)
{
    return BLMatrix2D(matrix.m00, matrix.m10, matrix.m01, matrix.m11, matrix.m02, matrix.m12);
}

BLRgba32 to_bl_color(const Color& color)
{
    return BLRgba32(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

BLStrokeCap to_bl_line_cap(LineCap cap)
{
    return cap == LineCap::Butt ? BL_STROKE_CAP_BUTT : cap == LineCap::Round ? BL_STROKE_CAP_ROUND : BL_STROKE_CAP_SQUARE;
}

BLStrokeJoin to_bl_line_join(LineJoin join)
{
    return join == LineJoin::Miter ? BL_STROKE_JOIN_MITER_BEVEL : join == LineJoin::Round ? BL_STROKE_JOIN_ROUND : BL_STROKE_JOIN_BEVEL;
}

BLFillRule to_bl_fill_rule(WindRule fillRule)
{
    return fillRule == WindRule::EvenOdd ? BL_FILL_RULE_EVEN_ODD : BL_FILL_RULE_NON_ZERO;
}

BLCompOp to_bl_comp_op(BlendMode mode)
{
    return mode == BlendMode::Dst_In ? BL_COMP_OP_DST_IN : BL_COMP_OP_SRC_OVER;
}

} // namespace lunasvg
