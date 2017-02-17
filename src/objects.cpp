#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objects.h"

namespace Game {
    Rect::Rect(const GLfloat &x, const GLfloat &y, const GLfloat &width, const GLfloat &height) {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;

        if (width_ < 1)
            width_ = 1;
        if (height_ < 1)
            height_ = 1;

        glGenBuffers(1, &vertex_buffer_);
        glGenBuffers(1, &color_buffer_);

        calculateBuffers();
    }

    Rect::Rect(const Rect &obj) {
        x_ = obj.x_;
        y_ = obj.y_;
        width_ = obj.width_;
        height_ = obj.height_;
        size_ = obj.size_;

        glGenBuffers(1, &vertex_buffer_);
        glGenBuffers(1, &color_buffer_);

        calculateBuffers();
    }

    Rect::~Rect(void) {
        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteBuffers(1, &color_buffer_);
    }

    void Rect::draw(const glm::mat4 &Projection, const glm::mat4 &View, const GLuint &MatrixID) {
        glm::mat4 Model = glm::translate(glm::mat4(), glm::vec3(x_, y_, 0.0f));
        glm::mat4 MVP = Projection * View * Model;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, size_);
    }

    void Rect::moveTo(const GLfloat &x, const GLfloat &y) {
        x_ = x;
        y_ = y;
    }

    GLfloat Rect::x(void) const {
        return x_;
    }

    void Rect::x(const GLfloat &x) {
        x_ = x;
    }

    GLfloat Rect::y(void) const {
        return y_;
    }

    void Rect::y(const GLfloat &y) {
        y_ = y;
    }

    unsigned int Rect::size(void) const {
        return size_;
    }

    GLfloat Rect::width(void) const {
        return width_;
    }

    GLfloat Rect::height(void) const {
        return height_;
    }

    void Rect::calculateBuffers(void) {
        GLfloat vertex_array[] = {
            0.0f, height_, 0.0f,
            width_, 0.0f, 0.0f,
            width_, height_, 0.0f,
            0.0f, height_, 0.0f,
            0.0f, 0.0f, 0.0f,
            width_, 0.0f, 0.0f
        };
        
        GLfloat color_array[sizeof(vertex_array)];
        for(int i = 0; i<18; i+=3){
            color_array[i] = 1.0f;
            color_array[i+1] = 1.0f;
            color_array[i+2] = 1.0f;
        }

        size_ = sizeof(vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array), vertex_array, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_array), color_array, GL_STATIC_DRAW);
    }

    Ball::Ball(const GLfloat &x, const GLfloat &y, const GLfloat &width, 
            const GLfloat &height): Rect(x, y, width, height)
    {
        origin_x_speed_ = 0;
        origin_y_speed_ = 0;
        x_speed_ = 0;
        y_speed_ = 0;
    }

    Ball::Ball(const GLfloat &x, const GLfloat &y, const GLfloat &width,
            const GLfloat &height, const GLfloat &x_speed, const GLfloat &y_speed
            ): Rect(x, y, width, height) {
        origin_x_speed_ = x_speed;
        origin_y_speed_ = y_speed;
        x_speed_ = x_speed;
        y_speed_ = y_speed;
    }

    Ball::Ball(const Ball &obj): Rect(static_cast<Rect>(obj)) {
        x_speed_ = obj.x_speed_;
        y_speed_ = obj.y_speed_;
    }

    void Ball::move(void) {
        x_ += x_speed_;
        y_ += y_speed_;
    }

    GLfloat Ball::x_speed(void) const {
        return x_speed_;
    }

    void Ball::x_speed(const GLfloat &x_speed) {
        x_speed_ = x_speed;
    }

    GLfloat Ball::y_speed(void) const {
        return y_speed_;
    }

    void Ball::y_speed(const GLfloat &y_speed) {
        y_speed_ = y_speed;
    }

    GLfloat Ball::origin_x_speed(void) const {
        return origin_x_speed_;
    }

    void Ball::origin_x_speed(const GLfloat &origin_x_speed) {
        origin_x_speed_ = origin_x_speed;
    }

    GLfloat Ball::origin_y_speed(void) const {
        return origin_y_speed_;
    }

    void Ball::origin_y_speed(const GLfloat &origin_y_speed) {
        origin_y_speed_ = origin_y_speed;
    }

    void Ball::init_speed(const GLfloat &origin_x_speed, const GLfloat &origin_y_speed) {
        origin_x_speed_ = origin_x_speed;
        origin_y_speed_ = origin_y_speed;
        x_speed_ = origin_x_speed_;
        y_speed_ = origin_y_speed_;
    }

    void Ball::speed_reset(void) {
        x_speed_ = origin_x_speed_;
        y_speed_ = origin_y_speed_;
    }

    Paddle::Paddle(const GLfloat &x, const GLfloat &y, const GLfloat &width, 
            const GLfloat &height): Rect(x, y, width, height)
    {}

    Paddle::Paddle(const GLfloat &x, const GLfloat &y, const GLfloat &width, 
            const GLfloat &height, const GLfloat &speed): Rect(x, y, width, height) {
        if(speed < 0) {
            speed_ = 0;
        }
        else {
            speed_ = speed;
        }
    }

    Paddle::Paddle(const Paddle &obj): Rect(static_cast<Rect>(obj)) {
        speed_ = obj.speed_;
    }

    void Paddle::moveUp(void) {
        y_ += speed_;
    }

    void Paddle::moveDown(void) {
        y_ -= speed_;
    }

    void Paddle::moveLeft(void) {
        x_ -= speed_;
    }

    void Paddle::moveRight(void) {
        x_ += speed_;
    }

    void Paddle::moveX(const GLfloat &magnitude) {
        x_ += magnitude;
    }

    void Paddle::moveY(const GLfloat &magnitude) {
        y_ += magnitude;
    }

    GLfloat Paddle::speed(void) const {
        return speed_;
    }

    void Paddle::speed(const GLfloat &speed) {
        if(speed < 0) {
            speed_ = 0;
        }
        else {
            speed_ = speed;
        }
    }
}
