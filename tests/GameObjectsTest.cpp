#include <iostream>

#include "GameObjects.h"

int main()
{
    GameObjects::Rect r(10.0, 7.0, 3.5, 5.0);
    GameObjects::Rect r2(r);

    std::cout << "x, y, width, height\n";
    std::cout << r.GetX() << " " << r.GetY() << " " << r.GetWidth() << " " << r.GetHeight() << "\n\n";

    r.SetPosition(12, 40);
    std::cout << "Position changed to 12, 40\n";
    std::cout << r.GetX() << " " << r.GetY() << "\n";

    r.SetSize(12, 8);
    std::cout << "Size changed to 12x8\n";
    std::cout << r.GetWidth() << " " << r.GetHeight() << "\n";

    r.SetX(5);
    r.SetY(6);
    r.SetWidth(3);
    r.SetHeight(4);
    std::cout << "r changed to x:5, y:6, 3x4\n";
    std::cout << r.GetX() << " " << r.GetY() << " " << r.GetWidth() << " " << r.GetHeight() << "\n";
    std::cout << "Compared to r2\n";
    std::cout << r2.GetX() << " " << r2.GetY() << " " << r2.GetWidth() << " " << r2.GetHeight() << "\n\n";

    GameObjects::Ball b(10, 5, 7, 8);
    GameObjects::Ball b2(b);
    GameObjects::Ball b3(r2);
    
    b.SetXSpeed(5);
    b.SetYSpeed(6);

    std::cout << "Created balls\n";
    std::cout << "x, y, width, height, xSpeed, ySpeed\n";
    std::cout << b.GetX() << " " << b.GetY() << " " << b.GetWidth() << " " << b.GetHeight() 
              << " " << b.GetXSpeed() << " " << b.GetYSpeed() << "\n";
    std::cout << b2.GetX() << " " << b2.GetY() << " " << b2.GetWidth() << " " << b2.GetHeight() 
              << " " << b2.GetXSpeed() << " " << b2.GetYSpeed() << "\n";
    std::cout << b3.GetX() << " " << b3.GetY() << " " << b3.GetWidth() << " " << b3.GetHeight() 
              << " " << b3.GetXSpeed() << " " << b3.GetYSpeed() << "\n\n";
    
    GameObjects::Paddle p(9, 7, 5, 6, 3);
    GameObjects::Paddle p2(8, 6, 9, 4);
    GameObjects::Paddle p3(p2);
    GameObjects::Paddle p4(r2);

    p3.SetSpeed(7);

    std::cout << "Created paddles\n";
    std::cout << "x, y, width, height, speed\n";
    std::cout << p.GetX() << " " << p.GetY() << " " << p.GetWidth() << " " << p.GetHeight() << " " << p.GetSpeed() << "\n";
    std::cout << p2.GetX() << " " << p2.GetY() << " " << p2.GetWidth() << " " << p2.GetHeight() << " " << p2.GetSpeed() << "\n";    
    std::cout << p3.GetX() << " " << p3.GetY() << " " << p3.GetWidth() << " " << p3.GetHeight() << " " << p3.GetSpeed() << "\n";
    std::cout << p4.GetX() << " " << p4.GetY() << " " << p4.GetWidth() << " " << p4.GetHeight() << " " << p4.GetSpeed() << "\n\n";
    

    GameObjects::Rect e;
    GameObjects::Ball e2;
    GameObjects::Paddle e3;
    std::cout << "Empty rect\n";
    std::cout << "x, y, width, height\n";
    std::cout << e.GetX() << " " << e.GetY() << " " << e.GetWidth() << " " << e.GetHeight() << "\n";
    std::cout << "Empty ball\n";
    std::cout << "x, y, width, height, xSpeed, ySpeed\n";
    std::cout << e2.GetX() << " " << e2.GetY() << " " << e2.GetWidth() << " " << e2.GetHeight() 
              << " " << e2.GetXSpeed() << " " << e2.GetYSpeed() << "\n";
    std::cout << "Empty paddle\n";
    std::cout << "x, y, width, height, speed\n";
    std::cout << e3.GetX() << " " << e3.GetY() << " " << e3.GetWidth() << " " << e3.GetHeight() << " " << e3.GetSpeed() << "\n";

    return 0;
}