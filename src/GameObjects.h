 #ifndef GAME_OBJECTS_H
 #define GAME_OBJECTS_H

 namespace GameObjects
 {
     class Rect
     {
         public:
         Rect();
         Rect(const double &x, const double &y, const double &width, const double &height);
         Rect(const Rect &obj);

         double GetX(void) const;
         double GetY(void) const;
         double GetWidth(void) const;
         double GetHeight(void) const;

         void SetPosition(const double &x, const double &y);
         void SetSize(const double &width, const double &height);
         void SetX(const double &x);
         void SetY(const double &y);
         void SetWidth(const double &width);
         void SetHeight(const double &height);

         protected:
         double x_;
         double y_;
         double width_;
         double height_;
     };

     class Ball: public Rect
     {
         public:
         Ball();
         Ball(const double &x, const double &y, const double &width, const double &height,
              const double &xSpeed, const double &ySpeed);
         Ball(const double &x, const double &y, const double &width, const double &height);
         Ball(const Ball &obj);
         Ball(const Rect &obj);

         double GetXSpeed(void) const;
         double GetYSpeed(void) const;

         void SetXSpeed(double const &xSpeed);
         void SetYSpeed(double const &ySpeed);

         protected:
         double xSpeed_;
         double ySpeed_;
     };

     class Paddle: public Rect
     {
         public:
         Paddle();
         Paddle(const double &x, const double &y, const double &width, const double &height,
                const double &speed);
         Paddle(const double &x, const double &y, const double &width, const double &height);
         Paddle(const Paddle &obj);
         Paddle(const Rect &obj);

         double GetSpeed(void) const;
         
         void SetSpeed(const double &speed);

         protected:
         double speed_;
     };
 }
 
#endif