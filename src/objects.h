#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/glew.h>

#ifdef __APPLE__
    #include <gl.h>
#else
    #include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Game {
    class Rect {
        public:
            Rect(const GLfloat &x, const GLfloat &y, 
                    const GLfloat &width, const GLfloat &height);
            Rect(const Rect &obj);
            ~Rect(void);

            void draw(const glm::mat4 &Projection, const glm::mat4 &View, const GLuint &MatrixID);
            void moveTo(const GLfloat &x, const GLfloat &y);
            
            GLfloat x(void) const;
            void x(const GLfloat &x);
            GLfloat y(void) const;
            void y(const GLfloat &y);
            unsigned int size(void) const;
            GLfloat width(void) const;
            GLfloat height(void) const;

        protected:
            GLuint vertex_buffer_, color_buffer_;
            GLfloat x_, y_, width_, height_;
            unsigned int size_;

            void calculateBuffers(void);
    };

    class Ball: public Rect {
        public:
            Ball(const GLfloat &x, const GLfloat &y,
                    const GLfloat &width, const GLfloat &height);
            Ball(const GLfloat &x, const GLfloat &y,
                    const GLfloat &width, const GLfloat &height, 
                    const GLfloat &x_speed, const GLfloat &y_speed);
            Ball(const Ball &obj);

            void move(void);

            GLfloat x_speed(void) const;
            void x_speed(const GLfloat &x_speed);
            GLfloat y_speed(void) const;
            void y_speed(const GLfloat &y_speed);

        private:
            GLfloat x_speed_, y_speed_;
    };

    class Paddle: public Rect {
        public:
            Paddle(const GLfloat &x, const GLfloat &y, 
                    const GLfloat &width, const GLfloat &height);
            Paddle(const GLfloat &x, const GLfloat &y, 
                    const GLfloat &width, const GLfloat &height,
                    const GLfloat &speed);
            Paddle(const Paddle &obj);

            void moveUp(void);
            void moveDown(void);
            void moveLeft(void);
            void moveRight(void);
            void moveX(const GLfloat &value);
            void moveY(const GLfloat &value);

            GLfloat speed(void) const;
            void speed(const GLfloat &speed);

        private:
            GLfloat speed_;
    };
}
#endif
