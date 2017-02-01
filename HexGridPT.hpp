template <class INDEX, class PIXEL>
struct HexGrid<INDEX, PIXEL, eHexGridType::PointyTopped>
{
    const INDEX index;
    const PIXEL center;
    const float radius;

    HexGrid(const INDEX& index_, float radius_)
        : index(index_)
        , center(HexConvertPT::ToPixel<PIXEL, INDEX::TType>(index_, radius_))
        , radius(radius_)
    {
    }

    float GetWidth() const
    {
        return GetWidth(radius);
    }

    float GetHeight() const
    {
        return GetHeight(radius);
    }

    PIXEL GetDistance(const INDEX& other) const
    {
        PIXEL otherCenter = HexConvertPT::ToPixel<PIXEL, INDEX::TType>(other, radius_);
        return PIXEL(otherCenter.x - center.x, otherCenter.y - center.y);
    }

    PIXEL GetCorner(int i) const
    {
        PIXEL pixel;
        float radian = (60.0f * i + 30.0f) * (static_cast<float>(M_PI) / 180.0f);
        pixel.x = center.x + radius * std::cos(radian);
        pixel.y = center.y + radius * std::sin(radian);
        return pixel;
    }

    static float GetWidth(float radius)
    {
        return std::sqrt(3.0f) * 0.5f * GetHeight(radius);
    }

    static float GetHeight(float radius)
    {
        return radius * 2.0f;
    }

    static float GetHorizontalDistance(float radius)
    {
        return GetWidth(radius);
    }

    static float GetVerticalDistance(float radius)
    {
        return GetHeight(radius) * 0.75f;
    }
};
