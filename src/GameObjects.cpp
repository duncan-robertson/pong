#include "GameObjects.h"

namespace GameObjects
{
    /*
     *  Rect implementation
     */
    
    Rect::Rect()
    {
        x_ = 0;
        y_ = 0;
        width_ = 1;
        height_ = 1;
    }

    Rect::Rect(const double &x, const double &y, const double &width, const double &height)
    {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;

        if(width_ <= 0)
            width_ = 1;
        
        if(height_ <= 0)
            height_ = 1;
    }

    Rect::Rect(const Rect &obj)
    {
        x_ = obj.GetX();
        y_ = obj.GetY();
        width_ = obj.GetWidth();
        height_ = obj.GetHeight();
    }

    double Rect::GetX(void) const
    {
        return x_;
    }

    double Rect::GetY(void) const
    {
        return y_;
    }

    double Rect::GetWidth(void) const
    {
        return width_;
    }

    double Rect::GetHeight(void) const
    {
        return height_;
    }

    void Rect::SetPosition(const double &x, const double &y)
    {
        x_ = x;
        y_ = y;
    }

    void Rect::SetSize(const double &width, const double &height)
    {
        if(width > 0)
        width_ = width;

        if(height > 0)
        height_ = height;
    }

    void Rect::SetX(const double &x)
    {
        x_ = x;
    }

    void Rect::SetY(const double &y)
    {
        y_ = y;
    }

    void Rect::SetWidth(const double &width)
    {
        width_ = width;
    }

    void Rect::SetHeight(const double &height)
    {
        height_ = height;
    }

    /*
     *  Ball implementation     
     */
    
    Ball::Ball()
    {
        xSpeed_ = 0;
        ySpeed_ = 0;
    }

    Ball::Ball(const double &x, const double &y, const double &width, const double &height,
               const double &xSpeed, const double &ySpeed)
               :Rect(x, y, width, height)
    {
        xSpeed_ = xSpeed;
        ySpeed_ = ySpeed;
    }

    Ball::Ball(const double &x, const double &y, const double &width, const double &height)
               :Rect(x, y, width, height)
    {
        xSpeed_ = 0;
        ySpeed_ = 0;
    }

    Ball::Ball(const Ball &obj)
    {
        x_ = obj.GetX();
        y_ = obj.GetY();
        width_ = obj.GetWidth();
        height_ = obj.GetHeight();
        xSpeed_ = obj.GetXSpeed();
        ySpeed_ = obj.GetYSpeed();
    }

    Ball::Ball(const Rect &obj)
    {
        x_ = obj.GetX();
        y_ = obj.GetY();
        width_ = obj.GetWidth();
        height_ = obj.GetHeight();
        xSpeed_ = 0;
        ySpeed_ = 0;
    }

    double Ball::GetXSpeed() const
    {
        return xSpeed_;
    }

    double Ball::GetYSpeed() const
    {
        return ySpeed_;
    }

    void Ball::SetXSpeed(const double &xSpeed)
    {
        xSpeed_ = xSpeed;
    }

    void Ball::SetYSpeed(const double &ySpeed)
    {
        ySpeed_ = ySpeed;
    }

    /*
     *  Paddle implementation
     */
    
    Paddle::Paddle()
    {
        speed_ = 0;
    }

    Paddle::Paddle(const double &x, const double &y, const double &width, const double &height,
           const double &speed)
           :Rect(x, y, width, height)
    {
        speed_ = speed;
    }

    Paddle::Paddle(const double &x, const double &y, const double &width, const double &height)
           :Rect(x, y, width, height)
    {
        speed_ = 0;
    }

    Paddle::Paddle(const Paddle &obj)
    {
        x_ = obj.GetX();
        y_ = obj.GetY();
        width_ = obj.GetWidth();
        height_ = obj.GetHeight();
        speed_ = obj.GetSpeed();
    }

    Paddle::Paddle(const Rect &obj)
    {
        x_ = obj.GetX();
        y_ = obj.GetY();
        width_ = obj.GetWidth();
        height_ = obj.GetHeight();
        speed_ = 0;
    }

    double Paddle::GetSpeed(void) const
    {
        return speed_;
    }
        
    void Paddle::SetSpeed(const double &speed)
    {
        speed_ = speed;
    }
}       