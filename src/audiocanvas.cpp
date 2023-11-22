#include <nanogui/audiocanvas.h>
#include <nanogui/canvas.h>
#include <nanogui/shader.h>
#include <cmath>
#include <GLFW/glfw3.h>
#include <vector>

NAMESPACE_BEGIN(nanogui)

AudioCanvas::AudioCanvas(Widget * parent) : Canvas(parent, 1) {
    _positions = nullptr;
    _positionsSize = 0;
    set_background_color(Color(122, 122, 122, 255));
    set_draw_area();
}

AudioCanvas::~AudioCanvas() {
    if(_positions != nullptr) delete [] _positions;
}

void AudioCanvas::set_draw_area() {
    _shader = new Shader(
        render_pass(),

        "a_waveform_draw_shader",
        //Vertex
        R"(#version 310 es
        precision highp float;
        in vec3 position;
        out vec4 frag_color;
        void main() {
            frag_color = vec4(0.3, 1.0, 0.1, 1.0);
            gl_Position = vec4(position, 1.0);
        })",
        // Fragment shader
        R"(#version 310 es
        precision highp float;
        out vec4 color;
        in vec4 frag_color;
        void main() {
            color = frag_color;
        })"
    );
        _positionsSize = 10;
        //x y z? x - width, y - height
        float positions[_positionsSize*3] = {
            normalizePosition(10), -0.3f, 0.f, //right line 
            normalizePosition(10), 0.3f, 0.f,
            normalizePosition(800), -0.7f, 0.f,  //left line
            normalizePosition(800), 0.7f, 0.f,
            0.f, 1.f, 0.f, //center line
            0.f, -1.f, 0.f,
            normalizePosition(150), 0.1f, 0.f,
            normalizePosition(150), 0.5f, 0.f,
            normalizePosition(150), -0.2f, 0.f,
            normalizePosition(150), -0.9f, 0.f
        };
        // _shader->set_buffer("indices", VariableType::UInt32, {3*12}, indices);
        _shader->set_buffer("position", VariableType::Float32, {_positionsSize, 3}, positions);
}

void AudioCanvas::draw_contents() {
    if(_shader == nullptr) return;

    int count = _positionsSize;
    _shader->begin();
    _shader->draw_array(Shader::PrimitiveType::Line, 0, count, false);
    _shader->end();
}

void AudioCanvas::set_audio_data(std::vector<float> * data)
{
    int width = m_size.x();
    int channels = 1;
    
    double samplesPerPixel = ((double)data->size()/(double)width);

    if(_positions != nullptr) delete [] _positions;
    int posSize = width*2; //*2 - min and max val, *3 - 3 val per position(x, y, z) + *channels
    _positions = new float[posSize*3];
    _positionsSize = posSize;

    memset(_positions, 0, sizeof(float)*posSize);
    int writeIdx = 0;
    for(int xr=0; xr<width; ++xr) {
        int offset = samplesPerPixel*xr;
        
        float min = 1.0f;
        float max = -1.0f;
        int tmp = offset+samplesPerPixel;
        for(int off=offset; off<=tmp; off+=channels) {
            float val = data->at(off);
            min = std::min(min, val);
            max = std::max(max, val);
        }

        if(samplesPerPixel < 1.f && min > 0) {
            max = 0.f;
        } else if(samplesPerPixel < 1.f && max < 0) {
            min = 0.f;
        } else if(samplesPerPixel < 1.f) {
            max = 0.f;
            min = 0.f;
        }

        float realX = normalizePosition(xr);
        _positions[writeIdx] = realX;   writeIdx++;
        _positions[writeIdx] = min;     writeIdx++;
        _positions[writeIdx] = 0.f;     writeIdx++;
        _positions[writeIdx] = realX;   writeIdx++;
        _positions[writeIdx] = max;     writeIdx++;
        _positions[writeIdx] = 0.f;     writeIdx++;
        // _positions->push_back(Vector3f(/*x*/0.f, min, 0.f));
        // _positions->push_back(Vector3f(/*x*/0.f, max, 0.f));
        
    }

    _shader->set_buffer("position", VariableType::Float32, {_positionsSize, 3}, _positions);
}

float AudioCanvas::normalizePosition(int x) {
    //y = y0 + ((y1 - y0)/(x1 - x0)) * (x - x0)
    int width = m_size.x();
    return ( (-1.f) + ( (1.f-(-1.f))/(width-0) ) * (x-0) );
}
NAMESPACE_END(nanogui)