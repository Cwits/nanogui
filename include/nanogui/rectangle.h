#pragma once
#include <nanogui/widget.h>

NAMESPACE_BEGIN(nanogui)

class NANOGUI_EXPORT Rectangle : public Widget {
    public:
    Rectangle(Widget * parent);

    const Color &background_color() const { return m_background_color; }
    void set_background_color(const Color &background_color) { m_background_color = background_color; }
    void fill_parent() { set_size(parent()->size()); set_position(Vector2i(0, 0)); }
    
    virtual void draw(NVGcontext * ctx) override;

    private:
    Color m_background_color;
};

NAMESPACE_END(nanogui)