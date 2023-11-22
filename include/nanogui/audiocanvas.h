#pragma once

#include <nanogui/canvas.h>
#include <nanogui/shader.h>
//works for 1 channel of audio for now, need to make it for two

NAMESPACE_BEGIN(nanogui)

class NANOGUI_EXPORT AudioCanvas : public Canvas {
public:
    AudioCanvas(Widget *parent);
    virtual ~AudioCanvas();

    void set_audio_data(std::vector<float> * data);
    void set_draw_area();

    virtual void draw_contents() override;

private:
    float normalizePosition(int x);
    ref<Shader> _shader = nullptr;
    float * _positions;
    long unsigned int _positionsSize;
    
    int m_channels;
};

NAMESPACE_END(nanogui)