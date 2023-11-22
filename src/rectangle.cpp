#include <nanogui/rectangle.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Rectangle::Rectangle(Widget *parent)
    : Widget(parent) { }

void Rectangle::draw(NVGcontext * ctx) {
    Widget::draw(ctx);

    NVGcolor grad_top = m_theme->m_button_gradient_top_unfocused;
    NVGcolor grad_bot = m_theme->m_button_gradient_bot_unfocused;

    // if (m_pushed || (m_mouse_focus && (m_flags & MenuButton))) {
    //     grad_top = m_theme->m_button_gradient_top_pushed;
    //     grad_bot = m_theme->m_button_gradient_bot_pushed;
    // } else if (m_mouse_focus && m_enabled) {
    //     grad_top = m_theme->m_button_gradient_top_focused;
    //     grad_bot = m_theme->m_button_gradient_bot_focused;
    // }

    nvgBeginPath(ctx);

    nvgRoundedRect(ctx, m_pos.x() + 1, m_pos.y() + 1.0f, m_size.x() - 2,
                   m_size.y() - 2, m_theme->m_button_corner_radius - 1);

    if (m_background_color.w() != 0) {
        nvgFillColor(ctx, Color(m_background_color[0], m_background_color[1],
                                m_background_color[2], 1.f));
        nvgFill(ctx);
        // if (m_pushed) {
        //     grad_top.a = grad_bot.a = 0.8f;
        // } else {
            double v = 1 - m_background_color.w();
            grad_top.a = grad_bot.a = m_enabled ? v : v * .5f + .5f;
        // }
    }

    NVGpaint bg = nvgLinearGradient(ctx, m_pos.x(), m_pos.y(), m_pos.x(),
                                    m_pos.y() + m_size.y(), grad_top, grad_bot);

    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgStrokeWidth(ctx, 1.0f);
    nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + /*(m_pushed ? 0.5f : */1.5f/*)*/, m_size.x() - 1,
                   m_size.y() - 1 - /*(m_pushed ? 0.0f : */1.0f/*)*/, m_theme->m_button_corner_radius);
    nvgStrokeColor(ctx, m_theme->m_border_light);
    nvgStroke(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + 0.5f, m_size.x() - 1,
                   m_size.y() - 2, m_theme->m_button_corner_radius);
    nvgStrokeColor(ctx, m_theme->m_border_dark);
    nvgStroke(ctx);
}

NAMESPACE_END(nanogui)